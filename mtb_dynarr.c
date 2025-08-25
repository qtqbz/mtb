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
        memmove(array->items, oldItems, array->length * array->itemSize);
    }
    array->capacity = capacity;
}

public bool
mtb_dynarr_is_empty(MtbDynArr *array)
{
    return array->length == 0;
}

public void
mtb_dynarr_clear(MtbDynArr *array)
{
    array->length = 0;
}

public void *
mtb_dynarr_insert_n(MtbDynArr *array, u64 from, u64 n)
{
    mtb_assert_always(from <= array->length);
    mtb_assert_always(n > 0);

    u64 capacity = array->capacity;
    u64 minCapacity = mtb_mul_u64(mtb_add_u64(array->length, n), array->itemSize);
    if (capacity < minCapacity) {
        capacity += capacity >> 1;
        mtb_dynarr_grow(array, mtb_max_u64(capacity, minCapacity));
    }

    u8 *beg = array->items + from * array->itemSize;
    if (from < array->length) {
        memmove(beg + n * array->itemSize, beg, (array->length - from) * array->itemSize);
    }
    array->length += n;

    return beg;
}

public void *
mtb_dynarr_insert(MtbDynArr *array, u64 at)
{
    return mtb_dynarr_insert_n(array, at, 1);
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
mtb_dynarr_remove(MtbDynArr *array, u64 at)
{
    return mtb_dynarr_remove_n(array, at, 1);
}

public void *
mtb_dynarr_get(MtbDynArr *array, u64 at)
{
    mtb_assert_always(at < array->length);
    return array->items + at * array->itemSize;
}

public void *
mtb_dynarr_copy_n(MtbDynArr *array, u64 from, void *src, u64 n)
{
    return memmove(mtb_dynarr_insert_n(array, from, n), src, n * array->itemSize);
}

public void *
mtb_dynarr_push(MtbDynArr *array)
{
    return mtb_dynarr_insert(array, array->length);
}

public void *
mtb_dynarr_pop(MtbDynArr *array)
{
    return mtb_dynarr_remove(array, array->length - 1);
}

public void *
mtb_dynarr_top(MtbDynArr *array)
{
    return mtb_dynarr_get(array, array->length - 1);
}

public void *
mtb_dynarr_enq(MtbDynArr *array)
{
    return mtb_dynarr_insert(array, 0);
}

public void *
mtb_dynarr_deq(MtbDynArr *array)
{
    return mtb_dynarr_remove(array, array->length - 1);
}

public void *
mtb_dynarr_front(MtbDynArr *array)
{
    return mtb_dynarr_get(array, array->length - 1);
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
    mtb_dynarr_copy_n(&array, 0, init, mtb_countof(init));
    assert(!mtb_dynarr_is_empty(&array));

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
    mtb_dynarr_copy_n(&array, 0, init, mtb_countof(init));
    assert(!mtb_dynarr_is_empty(&array));

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
    mtb_dynarr_grow(&array, count * array.itemSize);
    assert(mtb_dynarr_is_empty(&array));
    assert(array.capacity == count * array.itemSize);
    assert(array.length == 0);

    mtb_dynarr_insert_n(&array, 0, count);
    assert(array.capacity == count * array.itemSize);
    assert(array.length == count);

    mtb_dynarr_insert(&array, array.length);
    assert(array.capacity >= (count + 1) * array.itemSize);
    assert(array.length == count + 1);

    mtb_dynarr_clear(&array);
    assert(array.capacity >= (count + 1) * array.itemSize);
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
test_mtb_dynarr_queue(MtbArena arena)
{
    MtbDynArr queue = {0};
    mtb_dynarr_init(&arena, &queue, sizeof(u32));
    assert(mtb_dynarr_is_empty(&queue));
    assert(queue.itemSize == sizeof(u32));

    *(u32 *)mtb_dynarr_enq(&queue) = 1;
    *(u32 *)mtb_dynarr_enq(&queue) = 2;
    *(u32 *)mtb_dynarr_enq(&queue) = 3;

    assert(*(u32 *)mtb_dynarr_front(&queue) == 1);
    assert(*(u32 *)mtb_dynarr_deq(&queue) == 1);
    assert(*(u32 *)mtb_dynarr_front(&queue) == 2);
    assert(*(u32 *)mtb_dynarr_deq(&queue) == 2);
    assert(*(u32 *)mtb_dynarr_front(&queue) == 3);
    assert(*(u32 *)mtb_dynarr_deq(&queue) == 3);

    assert(mtb_dynarr_is_empty(&queue));
}

intern void
test_mtb_dynarr(void)
{
    MtbArena arena = {0};
    mtb_arena_init(&arena, kb(1), &MTB_ARENA_DEF_ALLOCATOR);

    test_mtb_dynarr_insert(arena);
    test_mtb_dynarr_remove(arena);
    test_mtb_dynarr_grow(arena);
    test_mtb_dynarr_stack(arena);
    test_mtb_dynarr_queue(arena);

    mtb_arena_deinit(&arena);
}

#endif // MTB_DYNARR_TESTS