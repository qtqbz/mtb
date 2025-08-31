#include "mtb_segarr.h"


#define _mtb_segarr_segment(i) \
    (u64_lit(63) - __builtin_clzll((i >> MTB_SEGARR_SKIPPED_SEGMENTS) + u64_lit(1)))

#define _mtb_segarr_segment_length(s) \
    ((u64_lit(1) << MTB_SEGARR_SKIPPED_SEGMENTS) << (s))

#define _mtb_segarr_segment_start(s) \
    (_mtb_segarr_segment_length(s) - (u64_lit(1) << MTB_SEGARR_SKIPPED_SEGMENTS))

#define _mtb_segarr_item(s, i) \
    (i - _mtb_segarr_segment_start(s))


public void
mtb_segarr_init(MtbArena *arena, MtbSegArr *array, u64 itemSize)
{
    array->arena = arena;
    array->itemSize = itemSize;
    array->count = 0;
}

public void
mtb_segarr_clear(MtbSegArr *array)
{
    array->count = 0;
}

public bool
mtb_segarr_is_empty(MtbSegArr *array)
{
    return array->count == 0;
}

public void *
mtb_segarr_add_last(MtbSegArr *array)
{
    u64 index = array->count++;
    u64 segment = _mtb_segarr_segment(index);
    u64 item = _mtb_segarr_item(segment, index);
    if (array->segments[segment] == nil) {
        u64 length = _mtb_segarr_segment_length(segment);
        u64 size = mtb_mul_u64(length, array->itemSize);
        array->segments[segment] = mtb_arena_bump(array->arena, u8, size);
    }
    return array->segments[segment] + item * array->itemSize;
}

public void *
mtb_segarr_remove_last(MtbSegArr *array)
{
    mtb_assert_always(array->count > 0);
    u64 index = --array->count;
    u64 segment = _mtb_segarr_segment(index);
    u64 item = _mtb_segarr_item(segment, index);
    return array->segments[segment] + item * array->itemSize;
}

public void *
mtb_segarr_get(MtbSegArr *array, u64 index)
{
    mtb_assert_always(index < array->count);
    u64 segment = _mtb_segarr_segment(index);
    u64 item = _mtb_segarr_item(segment, index);
    return array->segments[segment] + item * array->itemSize;
}

public void
mtb_segarr_add_last_n(MtbSegArr *array, void *src, u64 n)
{
    for (u64 i = 0; i < n; i++) {
        memmove(mtb_segarr_add_last(array), src + i * array->itemSize, array->itemSize);
    }
}

public void *
mtb_segarr_push(MtbSegArr *array)
{
    return mtb_segarr_add_last(array);
}

public void *
mtb_segarr_pop(MtbSegArr *array)
{
    return mtb_segarr_remove_last(array);
}

public void *
mtb_segarr_top(MtbSegArr *array)
{
    return mtb_segarr_get(array, array->count - 1);
}


#ifdef MTB_SEGARR_TESTS

#include <assert.h>


intern void
test_mtb_segarr_array(MtbArena arena)
{
    MtbSegArr array = {0};
    mtb_segarr_init(&arena, &array, sizeof(u64));

    u64 n = 100000;
    for (u64 i = 0; i < n; i++) {
        *(u64 *)mtb_segarr_add_last(&array) = i;
    }
    for (u64 i = 0; i < n; i++) {
        assert(*(u64 *)mtb_segarr_get(&array, i) == i);
    }
    for (u64 i = n; i > 0 ; i--) {
        assert(*(u64 *)mtb_segarr_remove_last(&array) == (i - 1));
    }
    assert(mtb_segarr_is_empty(&array));

    u64 *arr = mtb_arena_bump(&arena, u64, n);
    for (u64 i = 0; i < n; i++) {
        arr[i] = n - i - 1;
    }
    mtb_segarr_add_last_n(&array, arr, n);
    for (u64 i = 0; i < n; i++) {
        assert(*(u64 *)mtb_segarr_get(&array, i) == arr[i]);
    }
    mtb_segarr_clear(&array);
    assert(mtb_segarr_is_empty(&array));
}

intern void
test_mtb_segarr_stack(MtbArena arena)
{
    MtbSegArr array = {0};
    mtb_segarr_init(&arena, &array, sizeof(u64));

    u64 n = 100000;
    for (u64 i = 0; n > i; i++) {
        *(u64 *)mtb_segarr_push(&array) = i;
    }
    for (u64 i = n; i > 0 ; i--) {
        assert(*(u64 *)mtb_segarr_top(&array) == (i - 1));
        assert(*(u64 *)mtb_segarr_pop(&array) == (i - 1));
    }
    assert(mtb_segarr_is_empty(&array));
}

intern void
test_mtb_segarr(void)
{
    MtbArena arena = {0};
    mtb_arena_init(&arena, mb(10), &MTB_ARENA_DEF_ALLOCATOR);

    test_mtb_segarr_array(arena);
    test_mtb_segarr_stack(arena);

    mtb_arena_deinit(&arena);
}

#endif // MTB_SEGARR_TESTS
