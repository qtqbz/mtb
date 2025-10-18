#ifndef MTB_MTB_SEGARR_H
#define MTB_MTB_SEGARR_H

#ifndef MTB_SEGARR_SKIP_SEGMENTS
#define MTB_SEGARR_SKIP_SEGMENTS 4
#endif
#ifndef MTB_SEGARR_MAX_SEGMENTS
#define MTB_SEGARR_MAX_SEGMENTS (48 - MTB_SEGARR_SKIP_SEGMENTS)
#endif


// SEG |       LENGTH        |        START        |        END
//-----+---------------------+---------------------+--------------------
//   0 |                  16 |                   0 |                  15
//   1 |                  32 |                  16 |                  47
//   2 |                  64 |                  48 |                 111
//   3 |                 128 |                 112 |                 239
//   4 |                 256 |                 240 |                 495
//   5 |                 512 |                 496 |               1'007
//   6 |               1'024 |               1'008 |               2'031
//   7 |               2'048 |               2'032 |               4'079
//   8 |               4'096 |               4'080 |               8'175
//   9 |               8'192 |               8'176 |              16'367
//  10 |              16'384 |              16'368 |              32'751
//  11 |              32'768 |              32'752 |              65'519
//  12 |              65'536 |              65'520 |             131'055
//  13 |             131'072 |             131'056 |             262'127
//  14 |             262'144 |             262'128 |             524'271
//  15 |             524'288 |             524'272 |           1'048'559
//  16 |           1'048'576 |           1'048'560 |           2'097'135
//  17 |           2'097'152 |           2'097'136 |           4'194'287
//  18 |           4'194'304 |           4'194'288 |           8'388'591
//  19 |           8'388'608 |           8'388'592 |          16'777'199
//  20 |          16'777'216 |          16'777'200 |          33'554'415
//  21 |          33'554'432 |          33'554'416 |          67'108'847
//  22 |          67'108'864 |          67'108'848 |         134'217'711
//  23 |         134'217'728 |         134'217'712 |         268'435'439
//  24 |         268'435'456 |         268'435'440 |         536'870'895
//  25 |         536'870'912 |         536'870'896 |       1'073'741'807
//  26 |       1'073'741'824 |       1'073'741'808 |       2'147'483'631
//  27 |       2'147'483'648 |       2'147'483'632 |       4'294'967'279
//  28 |       4'294'967'296 |       4'294'967'280 |       8'589'934'575
//  29 |       8'589'934'592 |       8'589'934'576 |      17'179'869'167
//  30 |      17'179'869'184 |      17'179'869'168 |      34'359'738'351
//  31 |      34'359'738'368 |      34'359'738'352 |      68'719'476'719
//  32 |      68'719'476'736 |      68'719'476'720 |     137'438'953'455
//  33 |     137'438'953'472 |     137'438'953'456 |     274'877'906'927
//  34 |     274'877'906'944 |     274'877'906'928 |     549'755'813'871
//  35 |     549'755'813'888 |     549'755'813'872 |   1'099'511'627'759
//  36 |   1'099'511'627'776 |   1'099'511'627'760 |   2'199'023'255'535
//  37 |   2'199'023'255'552 |   2'199'023'255'536 |   4'398'046'511'087
//  38 |   4'398'046'511'104 |   4'398'046'511'088 |   8'796'093'022'191
//  39 |   8'796'093'022'208 |   8'796'093'022'192 |  17'592'186'044'399
//  40 |  17'592'186'044'416 |  17'592'186'044'400 |  35'184'372'088'815
//  41 |  35'184'372'088'832 |  35'184'372'088'816 |  70'368'744'177'647
//  42 |  70'368'744'177'664 |  70'368'744'177'648 | 140'737'488'355'311
//  43 | 140'737'488'355'328 | 140'737'488'355'312 | 281'474'976'710'639
typedef struct mtb_segarr MtbSegArr;
struct mtb_segarr
{
    MtbArena *arena;
    u8 *segments[MTB_SEGARR_MAX_SEGMENTS];
    u64 count;
    u64 itemSize;
};


/* Segment Array API */

public void mtb_segarr_init(MtbSegArr *array, MtbArena *arena, u64 itemSize);
public void mtb_segarr_clear(MtbSegArr *array);
public bool mtb_segarr_is_empty(MtbSegArr *array);

public void *mtb_segarr_add_last(MtbSegArr *array);
public void *mtb_segarr_remove_last(MtbSegArr *array);
public void *mtb_segarr_get(MtbSegArr *array, u64 index);

public void mtb_segarr_add_last_n(MtbSegArr *array, void *src, u64 n);


/* Stack API */

public void *mtb_segarr_push(MtbSegArr *array);
public void *mtb_segarr_pop(MtbSegArr *array);
public void *mtb_segarr_top(MtbSegArr *array);


/* Iterator API */

typedef struct mtb_segarr_iter MtbSegArrIter;
struct mtb_segarr_iter
{
    MtbSegArr *array;
    u64 index;
};


public void mtb_segarr_iter_init(MtbSegArrIter *it, MtbSegArr *array);
public void mtb_segarr_iter_reset(MtbSegArrIter *it);
public bool mtb_segarr_iter_has_next(MtbSegArrIter *it);
public void *mtb_segarr_iter_next(MtbSegArrIter *it);

#endif //MTB_MTB_SEGARR_H


#ifdef MTB_SEGARR_IMPLEMENTATION

#define _mtb_segarr_segment(i) (u64_lit(63) - mtb_leading_zeros_count((i >> MTB_SEGARR_SKIP_SEGMENTS) + u64_lit(1)))
#define _mtb_segarr_segment_length(s) ((u64_lit(1) << MTB_SEGARR_SKIP_SEGMENTS) << (s))
#define _mtb_segarr_segment_start(s) (_mtb_segarr_segment_length(s) - (u64_lit(1) << MTB_SEGARR_SKIP_SEGMENTS))
#define _mtb_segarr_item(s, i) (i - _mtb_segarr_segment_start(s))


public void
mtb_segarr_init(MtbSegArr *array, MtbArena *arena, u64 itemSize)
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

public void
mtb_segarr_iter_init(MtbSegArrIter *it, MtbSegArr *array)
{
    it->array = array;
    mtb_segarr_iter_reset(it);
}

public void
mtb_segarr_iter_reset(MtbSegArrIter *it)
{
    it->index = 0;
}

public bool
mtb_segarr_iter_has_next(MtbSegArrIter *it)
{
    return it->index < it->array->count;
}

public void *
mtb_segarr_iter_next(MtbSegArrIter *it)
{
    return mtb_segarr_get(it->array, it->index++);
}

#endif // MTB_SEGARR_IMPLEMENTATION


#ifdef MTB_SEGARR_TESTS

#include <assert.h>


intern void
test_mtb_segarr_add_last(MtbArena arena)
{
    MtbSegArr array = {0};
    mtb_segarr_init(&array, &arena, sizeof(u64));

    u64 n = 100000;
    for (u64 i = 0; i < n; i++) {
        *(u64 *)mtb_segarr_add_last(&array) = i;
    }
    for (u64 i = 0; i < n; i++) {
        assert(*(u64 *)mtb_segarr_get(&array, i) == i);
    }
    mtb_segarr_clear(&array);
    assert(mtb_segarr_is_empty(&array));
}

intern void
test_mtb_segarr_remove_last(MtbArena arena)
{
    MtbSegArr array = {0};
    mtb_segarr_init(&array, &arena, sizeof(u64));

    u64 n = 100000;
    for (u64 i = 0; i < n; i++) {
        *(u64 *)mtb_segarr_add_last(&array) = i;
    }
    for (u64 i = n; i > 0 ; i--) {
        assert(*(u64 *)mtb_segarr_remove_last(&array) == (i - 1));
    }
    assert(mtb_segarr_is_empty(&array));
}

intern void
test_mtb_segarr_add_last_n(MtbArena arena)
{
    MtbSegArr array = {0};
    mtb_segarr_init(&array, &arena, sizeof(u64));

    u64 n = 100000;
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
    mtb_segarr_init(&array, &arena, sizeof(u64));

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
test_mtb_segarr_iter(MtbArena arena)
{
    MtbSegArr array = {0};
    mtb_segarr_init(&array, &arena, sizeof(u64));

    MtbSegArrIter it = {0};
    mtb_segarr_iter_init(&it, &array);

    assert(!mtb_segarr_iter_has_next(&it));

    u64 n = 100000;
    for (u64 i = 0; n > i; i++) {
        *(u64 *)mtb_segarr_push(&array) = i;
    }

    for (int i = 0; i < 5; i++) {
        mtb_segarr_iter_reset(&it);

        u64 k = 0;
        while (mtb_segarr_iter_has_next(&it)) {
            assert(*(u64 *)mtb_segarr_iter_next(&it) == k++);
        }
    }

    mtb_segarr_clear(&array);
    assert(mtb_segarr_is_empty(&array));
}

intern void
test_mtb_segarr(void)
{
    MtbArena arena = {0};
    mtb_arena_init(&arena, mb(10), &MTB_ARENA_DEF_ALLOCATOR);

    test_mtb_segarr_add_last(arena);
    test_mtb_segarr_remove_last(arena);
    test_mtb_segarr_add_last_n(arena);
    test_mtb_segarr_stack(arena);
    test_mtb_segarr_iter(arena);

    mtb_arena_deinit(&arena);
}

#endif // MTB_SEGARR_TESTS
