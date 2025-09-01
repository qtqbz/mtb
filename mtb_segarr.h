#ifndef MTB_MTB_SEGARR_H
#define MTB_MTB_SEGARR_H

#include "mtb_macro.h"
#include "mtb_type.h"
#include "mtb_arena.h"


#ifndef MTB_SEGARR_SKIP_SEGMENTS
#define MTB_SEGARR_SKIP_SEGMENTS 6
#endif
#ifndef MTB_SEGARR_MAX_SEGMENTS
#define MTB_SEGARR_MAX_SEGMENTS 24
#endif


// SEG |   LENGTH    |    START    |      END
//-----+-------------+-------------+--------------
//   0 |          64 |           0 |            63
//   1 |         128 |          64 |           191
//   2 |         256 |         192 |           447
//   3 |         512 |         448 |           959
//   4 |       1'024 |         960 |         1'983
//   5 |       2'048 |       1'984 |         4'031
//   6 |       4'096 |       4'032 |         8'127
//   7 |       8'192 |       8'128 |        16'319
//   8 |      16'384 |      16'320 |        32'703
//   9 |      32'768 |      32'704 |        65'471
//  10 |      65'536 |      65'472 |       131'007
//  11 |     131'072 |     131'008 |       262'079
//  12 |     262'144 |     262'080 |       524'223
//  13 |     524'288 |     524'224 |     1'048'511
//  14 |   1'048'576 |   1'048'512 |     2'097'087
//  15 |   2'097'152 |   2'097'088 |     4'194'239
//  16 |   4'194'304 |   4'194'240 |     8'388'543
//  17 |   8'388'608 |   8'388'544 |    16'777'151
//  18 |  16'777'216 |  16'777'152 |    33'554'367
//  19 |  33'554'432 |  33'554'368 |    67'108'799
//  20 |  67'108'864 |  67'108'800 |   134'217'663
//  21 | 134'217'728 | 134'217'664 |   268'435'391
//  22 | 268'435'456 | 268'435'392 |   536'870'847
//  23 | 536'870'912 | 536'870'848 | 1'073'741'759
typedef struct mtb_segarr MtbSegArr;
struct mtb_segarr
{
    MtbArena *arena;
    u8 *segments[MTB_SEGARR_MAX_SEGMENTS];
    u64 count;
    u64 itemSize;
};


/* Segment Array API */

#define _mtb_segarr_segment(i) \
    (u64_lit(63) - __builtin_clzll((i >> MTB_SEGARR_SKIP_SEGMENTS) + u64_lit(1)))

#define _mtb_segarr_segment_length(s) \
    ((u64_lit(1) << MTB_SEGARR_SKIP_SEGMENTS) << (s))

#define _mtb_segarr_segment_start(s) \
    (_mtb_segarr_segment_length(s) - (u64_lit(1) << MTB_SEGARR_SKIP_SEGMENTS))

#define _mtb_segarr_item(s, i) \
    (i - _mtb_segarr_segment_start(s))


public void mtb_segarr_init(MtbArena *arena, MtbSegArr *array, u64 itemSize);
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

#endif //MTB_MTB_SEGARR_H
