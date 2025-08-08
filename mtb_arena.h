#ifndef MTB_ARENA_H
#define MTB_ARENA_H

#include "mtb_macro.h"
#include "mtb_type.h"


/* Arena Allocator */

typedef struct mtb_arena_allocator MtbArenaAllocator;
struct mtb_arena_allocator
{
    void *ctx;                                      /* custom context */
    void *(*alloc)(void *ctx, void *ptr, u64 size); /* allocate if size > 0, free otherwise */
    u64 (*size)(void *ctx, void *ptr);              /* return allocation size */
};

public u64 mtb_arena_def_size(void *ctx, void *ptr);
public void *mtb_arena_def_alloc(void *ctx, void *ptr, u64 size);

public u64 mtb_arena_def_virt_size(void *ctx, void *ptr);
public void *mtb_arena_def_virt_alloc(void *ctx, void *ptr, u64 size);

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
#define MTB_ARENA_DEF_ALIGN 8
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

public void mtb_arena_init(MtbArena *arena, u64 size, MtbArenaAllocator *allocator);
public void mtb_arena_deinit(MtbArena *arena);

public void *mtb_arena_bump_raw(MtbArena *arena, u64 size, u64 align, bool zero);
#define mtb_arena_bump(a, t, c) \
    (t *)mtb_arena_bump_raw(a, mtb_mul_u64(sizeof(t), (c)), mtb_max_u64(mtb_alignof(t), MTB_ARENA_DEF_ALIGN), true)
#define mtb_arena_bump_nozero(a, t, c) \
    (t *)mtb_arena_bump_raw(a, mtb_mul_u64(sizeof(t), (c)), mtb_max_u64(mtb_alignof(t), MTB_ARENA_DEF_ALIGN), false)
#define mtb_arena_bump_aligned(a, t, c, l) \
    (t *)mtb_arena_bump_raw(a, mtb_mul_u64(sizeof(t), (c)), l, true)
#define mtb_arena_bump_aligned_nozero(a, t, c, l) \
    (t *)mtb_arena_bump_raw(a, mtb_mul_u64(sizeof(t), (c)), l, false)

public MtbArenaSavePoint mtb_arena_save(MtbArena *arena);
public void mtb_arena_restore(MtbArena *arena, MtbArenaSavePoint *sp);

#define mtb_arena_clear(a) (a)->offset = 0

#endif //MTB_ARENA_H
