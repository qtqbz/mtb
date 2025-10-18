#ifndef MTB_MTB_DYNARR_H
#define MTB_MTB_DYNARR_H

typedef struct mtb_dynarr MtbDynArr;
struct mtb_dynarr
{
    MtbArena *arena;
    u8 *items;
    u64 itemSize;
    u64 length;
    u64 capacity;
};


/* Dynamic Array API */

#define _mtb_dynarr_foreach(array, var, _array, _end) \
    __auto_type _array = (array); \
    for (void *_end = _array->items + _array->length * _array->itemSize, *var = _array->items; var < _end; var += _array->itemSize)
#define mtb_dynarr_foreach(array, var) _mtb_dynarr_foreach(array, var, mtb_id(_array), mtb_id(_end))

public void mtb_dynarr_init(MtbDynArr *array, MtbArena *arena, u64 itemSize);
public void mtb_dynarr_grow(MtbDynArr *array, u64 capacity);
public bool mtb_dynarr_is_empty(MtbDynArr *array);
public void mtb_dynarr_clear(MtbDynArr *array);

public void *mtb_dynarr_insert_n(MtbDynArr *array, u64 begIndex, u64 n);
public void *mtb_dynarr_insert(MtbDynArr *array, u64 index);
public void *mtb_dynarr_remove_n(MtbDynArr *array, u64 begIndex, u64 n);
public void *mtb_dynarr_remove(MtbDynArr *array, u64 index);
public void *mtb_dynarr_get(MtbDynArr *array, u64 index);

public void *mtb_dynarr_copy_n(MtbDynArr *array, u64 begIndex, void *src, u64 n);


/* Stack API */

public void *mtb_dynarr_push(MtbDynArr *array);
public void *mtb_dynarr_pop(MtbDynArr *array);
public void *mtb_dynarr_top(MtbDynArr *array);


/* Queue API */

public void *mtb_dynarr_enq(MtbDynArr *array);
public void *mtb_dynarr_deq(MtbDynArr *array);
public void *mtb_dynarr_front(MtbDynArr *array);


/* Iterator API */

typedef struct mtb_dynarr_iter MtbDynArrIter;
struct mtb_dynarr_iter
{
    MtbDynArr *array;
    u64 index;
};


public void mtb_dynarr_iter_init(MtbDynArrIter *it, MtbDynArr *array);
public void mtb_dynarr_iter_reset(MtbDynArrIter *it);
public bool mtb_dynarr_iter_has_next(MtbDynArrIter *it);
public void *mtb_dynarr_iter_next(MtbDynArrIter *it);


#endif //MTB_MTB_DYNARR_H


#ifdef MTB_DYNARR_IMPLEMENTATION

#include <string.h>


public void
mtb_dynarr_init(MtbDynArr *array, MtbArena *arena, u64 itemSize)
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
mtb_dynarr_insert_n(MtbDynArr *array, u64 begIndex, u64 n)
{
    mtb_assert_always(begIndex <= array->length);
    mtb_assert_always(n > 0);

    u64 capacity = array->capacity;
    u64 minCapacity = mtb_mul_u64(mtb_add_u64(array->length, n), array->itemSize);
    if (capacity < minCapacity) {
        capacity += capacity >> 1;
        mtb_dynarr_grow(array, mtb_max_u64(capacity, minCapacity));
    }

    u8 *beg = array->items + begIndex * array->itemSize;
    if (begIndex < array->length) {
        memmove(beg + n * array->itemSize, beg, (array->length - begIndex) * array->itemSize);
    }
    array->length += n;

    return beg;
}

public void *
mtb_dynarr_insert(MtbDynArr *array, u64 index)
{
    return mtb_dynarr_insert_n(array, index, 1);
}

public void *
mtb_dynarr_remove_n(MtbDynArr *array, u64 begIndex, u64 n)
{
    mtb_assert_always(begIndex < array->length);
    mtb_assert_always(n > 0);

    u64 endIndex = mtb_add_u64(begIndex, n);
    mtb_assert_always(endIndex <= array->length);

    u8 *beg = array->items + begIndex * array->itemSize;
    if (endIndex < array->length) {
        memmove(beg, array->items + endIndex * array->itemSize, (array->length - endIndex) * array->itemSize);
    }
    array->length -= n;

    return beg;
}

public void *
mtb_dynarr_remove(MtbDynArr *array, u64 index)
{
    return mtb_dynarr_remove_n(array, index, 1);
}

public void *
mtb_dynarr_get(MtbDynArr *array, u64 index)
{
    mtb_assert_always(index < array->length);
    return array->items + index * array->itemSize;
}

public void *
mtb_dynarr_copy_n(MtbDynArr *array, u64 begIndex, void *src, u64 n)
{
    return memmove(mtb_dynarr_insert_n(array, begIndex, n), src, n * array->itemSize);
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

public void
mtb_dynarr_iter_init(MtbDynArrIter *it, MtbDynArr *array)
{
    it->array = array;
    mtb_dynarr_iter_reset(it);
}

public void
mtb_dynarr_iter_reset(MtbDynArrIter *it)
{
    it->index = 0;
}

public bool
mtb_dynarr_iter_has_next(MtbDynArrIter *it)
{
    return it->index < it->array->length;
}

public void *
mtb_dynarr_iter_next(MtbDynArrIter *it)
{
    return mtb_dynarr_get(it->array, it->index++);
}

#endif //MTB_DYNARR_IMPLEMENTATION


#ifdef MTB_DYNARR_TESTS

#include <assert.h>


intern void
test_mtb_dynarr_insert(MtbArena arena)
{
    MtbDynArr array = {0};
    mtb_dynarr_init(&array, &arena, sizeof(u64));
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
    mtb_dynarr_init(&array, &arena, sizeof(u64));
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
    mtb_dynarr_init(&array, &arena, sizeof(u16));
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
    mtb_dynarr_init(&stack, &arena, sizeof(u32));
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
    mtb_dynarr_init(&queue, &arena, sizeof(u32));
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
test_mtb_dynarr_iter(MtbArena arena)
{
    MtbDynArr array = {0};
    mtb_dynarr_init(&array, &arena, sizeof(u32));
    assert(mtb_dynarr_is_empty(&array));

    MtbDynArrIter it = {0};
    mtb_dynarr_iter_init(&it, &array);
    assert(!mtb_dynarr_iter_has_next(&it));

    u32 n = 10;
    for (u32 i = 0; i < n; i++) {
        *(u32 *)mtb_dynarr_push(&array) = i;
    }

    mtb_dynarr_iter_reset(&it);
    assert(mtb_dynarr_iter_has_next(&it));

    u32 i = 0;
    while (mtb_dynarr_iter_has_next(&it)) {
        assert(*(u32 *)mtb_dynarr_iter_next(&it) == i++);
    }
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
    test_mtb_dynarr_iter(arena);

    mtb_arena_deinit(&arena);
}

#endif // MTB_DYNARR_TESTS
