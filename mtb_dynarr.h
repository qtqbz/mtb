#ifndef MTB_MTB_DYNARR_H
#define MTB_MTB_DYNARR_H

#include "mtb_macro.h"
#include "mtb_type.h"
#include "mtb_arena.h"


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


#endif //MTB_MTB_DYNARR_H
