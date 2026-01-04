#ifndef MTB_ARENA_H
#define MTB_ARENA_H

/* Arena Allocator */

typedef struct mtb_arena_allocator MtbArenaAllocator;
struct mtb_arena_allocator
{
    void *ctx;                                      /* custom context */
    void *(*alloc)(void *ctx, void *ptr, u64 size); /* allocate if size > 0, free otherwise */
    u64 (*size)(void *ctx, void *ptr);              /* return allocation size */
};

func u64 mtb_arena_def_size(void *ctx, void *ptr);
func void *mtb_arena_def_alloc(void *ctx, void *ptr, u64 size);

func u64 mtb_arena_def_virt_size(void *ctx, void *ptr);
func void *mtb_arena_def_virt_alloc(void *ctx, void *ptr, u64 size);

global MtbArenaAllocator MTB_ARENA_DEF_ALLOCATOR = {
    .ctx = nil,
    .alloc = mtb_arena_def_alloc,
    .size = mtb_arena_def_size,
};

global MtbArenaAllocator MTB_ARENA_DEF_VIRT_ALLOCATOR = {
    .ctx = nil,
    .alloc = mtb_arena_def_virt_alloc,
    .size = mtb_arena_def_virt_size,
};


/* Arena */

#ifndef MTB_ARENA_DEF_ALIGN
#define MTB_ARENA_DEF_ALIGN sizeof(void *)
#endif

typedef struct mtb_arena MtbArena;
struct mtb_arena
{
    u8 *base;
    u64 offset;
    u64 size;
    MtbArenaAllocator *allocator;
};

typedef struct mtb_arena_save_point MtbArenaSavePoint;
struct mtb_arena_save_point
{
    MtbArena *arena;
    u64 offset;
};

typedef struct mtb_arena_bump_options MtbArenaBumpOptions;
struct mtb_arena_bump_options
{
    u64 align;
    bool no_zero;
};

func void mtb_arena_init(MtbArena *arena, u64 size, MtbArenaAllocator *allocator);
func void mtb_arena_deinit(MtbArena *arena);

func void *mtb_arena_bump_opt(MtbArena *arena, u64 size, MtbArenaBumpOptions opt);
#define mtb_arena_bump_raw(arena, size, ...) \
    mtb_arena_bump_opt(arena, size, (MtbArenaBumpOptions){ __VA_ARGS__ })
#define mtb_arena_bump(arena, type, count, ...) \
    (type *)mtb_arena_bump_opt(arena, mtb_mul_u64(sizeof(type), (count)), (MtbArenaBumpOptions){ .align = mtb_alignof(type), __VA_ARGS__ })

func MtbArenaSavePoint mtb_arena_save(MtbArena *arena);
func void mtb_arena_restore(MtbArena *arena, MtbArenaSavePoint *sp);

func void mtb_arena_clear(MtbArena *arena);

#endif //MTB_ARENA_H


#ifdef MTB_ARENA_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>


#define MTB_ARENA_DEF_ALLOCATOR_HEADER_SIZE sizeof(u64)

func u64
mtb_arena_def_size(void *ctx, void *ptr)
{
    u8 *base = (u8 *)ptr;
    u64 headerSize = MTB_ARENA_DEF_ALLOCATOR_HEADER_SIZE;
    u64 *header = (u64 *)(base - headerSize);
    u64 allocSize = *header;

    return allocSize;
}

func void *
mtb_arena_def_alloc(void *ctx, void *ptr, u64 size)
{
    u8 *base = (u8 *)ptr;
    u64 headerSize = MTB_ARENA_DEF_ALLOCATOR_HEADER_SIZE;

    if (size == 0) {
        mtb_assert_always(base != nil);
        free(base - headerSize);
        return nil;
    }

    mtb_assert_always(base == nil);

    u64 allocSize = mtb_add_u64(headerSize, size);
    u8 *mallocAddr = (u8 *)malloc(allocSize);
    mtb_assert(mallocAddr != nil);

    u64 *header = (u64 *)mallocAddr;
    *header = allocSize;

    return header + 1;
}

func u64
mtb_arena_def_virt_size(void *ctx, void *ptr)
{
    u8 *base = (u8 *)ptr;
    u64 headerSize = MTB_ARENA_DEF_ALLOCATOR_HEADER_SIZE;
    u64 *header = (u64 *)(base - headerSize);
    u64 allocSize = *header;

    return allocSize;
}

func void *
mtb_arena_def_virt_alloc(void *ctx, void *ptr, u64 size)
{
    u8 *base = (u8 *)ptr;
    u64 headerSize = MTB_ARENA_DEF_ALLOCATOR_HEADER_SIZE;
    u64 pageSize = (u64)sysconf(_SC_PAGE_SIZE);
    mtb_assert(mtb_is_pow2(pageSize));

    if (size == 0) {
        mtb_assert_always(base != nil);
        u64 allocSize = mtb_arena_def_virt_size(ctx, base);
        mtb_assert(munmap(base - headerSize - pageSize, allocSize) == 0);
        return nil;
    }

    mtb_assert_always(base == nil);

    // Allocate enough memory to fit: arena, header, 2 guard pages.
    u64 sizeWithHeader = mtb_add_u64(size, headerSize);
    u64 sizeWithHeaderAligned = mtb_align_pow2(sizeWithHeader, pageSize);
    u64 allocSize = mtb_add_u64(sizeWithHeaderAligned, 2 * pageSize);
    u8 *mmapAddr = (u8 *)mmap(nil, allocSize, PROT_NONE, MAP_ANON | MAP_PRIVATE, -1, 0);
    mtb_assert(mmapAddr != MAP_FAILED);

    u64 *header = (u64 *)(mmapAddr + pageSize);
    mtb_assert(mprotect(header, sizeWithHeaderAligned, PROT_READ | PROT_WRITE) == 0);
    *header = allocSize;

    return header + 1;
}

func void
mtb_arena_init(MtbArena *arena, u64 size, MtbArenaAllocator *allocator)
{
    arena->allocator = allocator;
    arena->base = allocator->alloc(allocator->ctx, nil, size);
    arena->offset = 0;
    arena->size = size;
}

func void
mtb_arena_deinit(MtbArena *arena)
{
    MtbArenaAllocator *allocator = arena->allocator;
    allocator->alloc(allocator->ctx, arena->base, 0);
    arena->base = nil;
    arena->offset = 0;
    arena->size = 0;
    arena->allocator = nil;
}

func void *
mtb_arena_bump_opt(MtbArena *arena, u64 size, MtbArenaBumpOptions opt)
{
    mtb_assert_always(size > 0);
    mtb_assert_always(mtb_is_pow2_or_zero(opt.align));

    u64 oldOffset = arena->offset;
    u64 align = mtb_max_u64(opt.align, MTB_ARENA_DEF_ALIGN);
    u64 padding = mtb_align_padding_pow2((u64)(arena->base + oldOffset), align);
    u64 oldOffsetAligned = mtb_add_u64(oldOffset, padding);
    u64 newOffset = mtb_add_u64(oldOffsetAligned, size);
    mtb_assert_always(newOffset <= arena->size);
    arena->offset = newOffset;

    u8 *result = arena->base + oldOffsetAligned;

    return opt.no_zero ? result : memset(result, 0, size);
}

func MtbArenaSavePoint
mtb_arena_save(MtbArena *arena)
{
    MtbArenaSavePoint sp = {0};
    sp.arena = arena;
    sp.offset = arena->offset;
    return sp;
}

func void
mtb_arena_restore(MtbArena *arena, MtbArenaSavePoint *sp)
{
    mtb_assert_always(arena == sp->arena);
    arena->offset = sp->offset;
}

func void
mtb_arena_clear(MtbArena *arena)
{
    arena->offset = 0;
}

#endif // MTB_ARENA_IMPLEMENTATION


#ifdef MTB_ARENA_TESTS

#include <assert.h>


func void
_test_mtb_allocator(MtbArenaAllocator *allocator)
{
    MtbArena arena = {0};
    u64 pageSize = (u64)sysconf(_SC_PAGE_SIZE);
    u64 arenaSize = 2 * pageSize;

    // init
    mtb_arena_init(&arena, arenaSize, allocator);
    assert(arena.size == arenaSize);
    assert(arena.allocator->size(arena.allocator->ctx, arena.base) >= arenaSize);

    // alloc
    u64 *tiny = mtb_arena_bump(&arena, u64, 1);
    assert(tiny != nil);
    assert(*tiny == 0);       // is zeroed out
    *tiny = U64_MAX;          // is writable
    assert(*tiny == U64_MAX); // is readable

    // page alloc
    u8 *page = mtb_arena_bump(&arena, u8, pageSize, .align = pageSize);
    assert(page != 0);
    assert((u64)page % pageSize == 0); // aligned to page boundary
    for (u32 i = 0; i < pageSize; i++) assert(page[i] == 0);      // is zeroed out
    for (u32 i = 0; i < pageSize; i++) page[i] = U8_MAX;          // is writable
    for (u32 i = 0; i < pageSize; i++) assert(page[i] == U8_MAX); // is readable

    // save/restore
    MtbArenaSavePoint sp = mtb_arena_save(&arena);
    assert(mtb_arena_bump(&arena, u8, 8) != nil);
    assert(arena.offset != sp.offset);
    mtb_arena_restore(&arena, &sp);
    assert(arena.offset == sp.offset);

    // clear
    assert(arena.offset != 0);
    mtb_arena_clear(&arena);
    assert(arena.offset == 0);

    // deinit
    assert(mtb_arena_bump(&arena, u32, 1) != nil);
    assert(arena.base != nil);
    assert(arena.offset != 0);
    assert(arena.size != 0);
    assert(arena.allocator != nil);
    mtb_arena_deinit(&arena);
    assert(arena.base == nil);
    assert(arena.offset == 0);
    assert(arena.size == 0);
    assert(arena.allocator == nil);
}

func void
_test_mtb_def_virt_allocator(void)
{
    _test_mtb_allocator(&MTB_ARENA_DEF_VIRT_ALLOCATOR);
}

func void
_test_mtb_def_allocator(void)
{
    _test_mtb_allocator(&MTB_ARENA_DEF_ALLOCATOR);
}

func void
_test_mtb_arena(void)
{
    _test_mtb_def_virt_allocator();
    _test_mtb_def_allocator();
}

#endif // MTB_ARENA_TESTS
