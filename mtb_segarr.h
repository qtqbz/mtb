#ifndef MTB_MTB_SEGARR_H
#define MTB_MTB_SEGARR_H

#include "mtb_macro.h"
#include "mtb_type.h"
#include "mtb_arena.h"

#define MTB_SEGARR_SKIPPED_SEGMENTS 6
#define MTB_SEGARR_MAX_SEGMENTS 42 // 48 - 6


typedef struct mtb_segarr MtbSegArr;
struct mtb_segarr
{
    MtbArena *arena;
    u8 *segments[MTB_SEGARR_MAX_SEGMENTS];
    u64 count;
    u64 itemSize;
};


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
