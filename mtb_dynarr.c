#include <string.h>

#include "mtb_dynarr.h"


public void
mtb_dynarr_init(MtbArena *arena, MtbDynArr *array, u64 itemSize)
{
    mtb_assert_always(itemSize > 0);

    array->arena = arena;
    array->items = nil;
    array->itemSize = itemSize;
    array->length = 0;
    array->capacity = 0;
}

public void
mtb_dynarr_grow(MtbDynArr *array, u64 capacity)
{
    mtb_assert_always(capacity > array->capacity);

    u8 *oldItems = array->items;
    array->items = mtb_arena_bump(array->arena, u8, capacity);
    if (!mtb_dynarr_is_empty(array)) {
        memcpy(array->items, oldItems, array->length * array->itemSize);
    }
    array->capacity = capacity;
}

public void *
mtb_dynarr_insert_n(MtbDynArr *array, u64 from, u64 n)
{
    mtb_assert_always(from <= array->length);
    mtb_assert_always(n > 0);

    u64 minCapacity = mtb_mul_u64(mtb_add_u64(array->length, n), array->itemSize);
    if (array->capacity < minCapacity) {
        u64 capacity = array->capacity == 0 ? mtb_mul_u64(MTB_DYNARR_INIT_COUNT, array->itemSize)
                                            : mtb_mul_u64(array->capacity, MTB_DYNARR_GROWTH_FACTOR);
        while (capacity < minCapacity) {
            capacity = mtb_mul_u64(capacity, MTB_DYNARR_GROWTH_FACTOR);
        }
        mtb_dynarr_grow(array, capacity);
    }

    u8 *beg = array->items + from * array->itemSize;
    if (from < array->length) {
        memmove(beg + n * array->itemSize, beg, (array->length - from) * array->itemSize);
    }
    array->length += n;

    return beg;
}

public void *
mtb_dynarr_remove_n(MtbDynArr *array, u64 from, u64 n)
{
    mtb_assert_always(from < array->length);
    mtb_assert_always(n > 0);

    u64 to = mtb_add_u64(from, n);
    mtb_assert_always(to <= array->length);

    u8 *beg = array->items + from * array->itemSize;
    if (to < array->length) {
        memmove(beg, array->items + to * array->itemSize, (array->length - to) * array->itemSize);
    }
    array->length -= n;

    return beg;
}

public void *
mtb_dynarr_get(MtbDynArr *array, u64 at)
{
    mtb_assert_always(at < array->length);
    return array->items + at * array->itemSize;
}


#ifdef MTB_DYNARR_TESTS

#include <assert.h>


intern void
test_mtb_dynarr_insert(MtbArena arena)
{
    MtbDynArr array = {0};
    mtb_dynarr_init(&arena, &array, sizeof(u64));
    assert(mtb_dynarr_is_empty(&array));
    assert(array.itemSize == sizeof(u64));

    u64 init[] = { 3 };
    u64 initCount = mtb_countof(init);
    void *initBeg = mtb_dynarr_insert_n(&array, 0, initCount);
    memcpy(initBeg, init, initCount * sizeof(*init));
    assert(!mtb_dynarr_is_empty(&array));
    assert(memcmp(mtb_dynarr_get(&array, 0), (u64[]){ 3 }, array.length * array.itemSize) == 0);

    u64 prefix[] = { 0, 1, 2 };
    u64 prefixCount = mtb_countof(prefix);
    void *prefixBeg = mtb_dynarr_insert_n(&array, 0, prefixCount);
    memcpy(prefixBeg, prefix, prefixCount * sizeof(*prefix));
    assert(memcmp(mtb_dynarr_get(&array, 0), (u64[]){ 0, 1, 2, 3 }, array.length * array.itemSize) == 0);

    u64 suffix[] = { 7, 8, 9 };
    u64 suffixCount = mtb_countof(suffix);
    void *suffixBeg = mtb_dynarr_insert_n(&array, array.length, suffixCount);
    memcpy(suffixBeg, suffix, suffixCount * sizeof(*suffix));
    assert(memcmp(mtb_dynarr_get(&array, 0), (u64[]){ 0, 1, 2, 3, 7, 8, 9 }, array.length * array.itemSize) == 0);

    u64 middle[] = { 4, 5, 6 };
    u64 middleCount = mtb_countof(middle);
    void *middleBeg = mtb_dynarr_insert_n(&array, 4, middleCount);
    memcpy(middleBeg, middle, middleCount * sizeof(*middle));
    assert(memcmp(mtb_dynarr_get(&array, 0), (u64[]){ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, array.length * array.itemSize) == 0);

    u64 i = 0;
    mtb_dynarr_foreach(&array, item) {
        *(u64 *)item = i++;
    }
    assert(i == array.length);

    mtb_dynarr_clear(&array);
    assert(mtb_dynarr_is_empty(&array));
}

intern void
test_mtb_dynarr_remove(MtbArena arena)
{
    MtbDynArr array = {0};
    mtb_dynarr_init(&arena, &array, sizeof(u64));
    assert(mtb_dynarr_is_empty(&array));
    assert(array.itemSize == sizeof(u64));

    u64 init[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    u64 initCount = mtb_countof(init);
    void *initBeg = mtb_dynarr_insert_n(&array, 0, initCount);
    memcpy(initBeg, init, initCount * sizeof(*init));
    assert(!mtb_dynarr_is_empty(&array));
    assert(memcmp(mtb_dynarr_get(&array, 0), (u64[]){ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, array.length * array.itemSize) == 0);

    u64 prefix[] = { 0, 1, 2 };
    u64 prefixCount = mtb_countof(prefix);
    mtb_dynarr_remove_n(&array, 0, prefixCount);
    assert(memcmp(mtb_dynarr_get(&array, 0), (u64[]){ 3, 4, 5, 6, 7, 8, 9 }, array.length * array.itemSize) == 0);

    u64 suffix[] = { 7, 8, 9 };
    u64 suffixCount = mtb_countof(suffix);
    mtb_dynarr_remove_n(&array, 4, suffixCount);
    assert(memcmp(mtb_dynarr_get(&array, 0), (u64[]){ 3, 4, 5, 6 }, array.length * array.itemSize) == 0);

    u64 middle[] = { 4, 5, 6 };
    u64 middleCount = mtb_countof(middle);
    mtb_dynarr_remove_n(&array, 1, middleCount);
    assert(memcmp(mtb_dynarr_get(&array, 0), (u64[]){ 3 }, array.length * array.itemSize) == 0);

    mtb_dynarr_clear(&array);
    assert(mtb_dynarr_is_empty(&array));
}

intern void
test_mtb_dynarr_grow(MtbArena arena)
{
    MtbDynArr array = {0};
    mtb_dynarr_init(&arena, &array, sizeof(u16));
    assert(mtb_dynarr_is_empty(&array));
    assert(array.capacity == 0);
    assert(array.itemSize == sizeof(u16));

    u64 count = 10;
    u64 initCapacity = count * array.itemSize;
    mtb_dynarr_grow(&array, initCapacity);
    assert(mtb_dynarr_is_empty(&array));
    assert(array.capacity == initCapacity);
    assert(array.length == 0);

    mtb_dynarr_insert_n(&array, 0, count);
    assert(array.capacity == initCapacity);
    assert(array.length == count);

    mtb_dynarr_insert(&array, array.length);
    assert(array.capacity == initCapacity * MTB_DYNARR_GROWTH_FACTOR);
    assert(array.length == count + 1);

    mtb_dynarr_clear(&array);
    assert(array.capacity == initCapacity * MTB_DYNARR_GROWTH_FACTOR);
    assert(mtb_dynarr_is_empty(&array));
}

intern void
test_mtb_dynarr_stack(MtbArena arena)
{
    MtbDynArr stack = {0};
    mtb_dynarr_init(&arena, &stack, sizeof(u32));
    assert(mtb_dynarr_is_empty(&stack));
    assert(stack.itemSize == sizeof(u32));

    *(u32 *)mtb_dynarr_push(&stack) = 1;
    *(u32 *)mtb_dynarr_push(&stack) = 2;
    *(u32 *)mtb_dynarr_push(&stack) = 3;

    assert(*(u32 *)mtb_dynarr_top(&stack) == 3);
    assert(*(u32 *)mtb_dynarr_pop(&stack) == 3);
    assert(*(u32 *)mtb_dynarr_top(&stack) == 2);
    assert(*(u32 *)mtb_dynarr_pop(&stack) == 2);
    assert(*(u32 *)mtb_dynarr_top(&stack) == 1);
    assert(*(u32 *)mtb_dynarr_pop(&stack) == 1);

    assert(mtb_dynarr_is_empty(&stack));
}

intern void
test_mtb_dynarr(void)
{
    MtbArena arena = {0};
    mtb_arena_init(&arena, 100, &MTB_ARENA_DEF_ALLOCATOR);

    test_mtb_dynarr_insert(arena);
    test_mtb_dynarr_remove(arena);
    test_mtb_dynarr_grow(arena);
    test_mtb_dynarr_stack(arena);

    mtb_arena_deinit(&arena);
}

#endif // MTB_DYNARR_TESTS