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

#define _mtb_dynarr_foreach(a, i, _a, _e) \
    __auto_type _a = (a); \
    for (void *_e = _a->items + _a->length * _a->itemSize, *i = _a->items; i < _e; i += _a->itemSize)
#define mtb_dynarr_foreach(a, i) _mtb_dynarr_foreach(a, i, mtb_id(_a), mtb_id(_e))

public void mtb_dynarr_init(MtbArena *arena, MtbDynArr *array, u64 itemSize);
public void mtb_dynarr_grow(MtbDynArr *array, u64 capacity);
public bool mtb_dynarr_is_empty(MtbDynArr *array);
public void mtb_dynarr_clear(MtbDynArr *array);

public void *mtb_dynarr_insert_n(MtbDynArr *array, u64 from, u64 n);
public void *mtb_dynarr_insert(MtbDynArr *array, u64 at);
public void *mtb_dynarr_remove_n(MtbDynArr *array, u64 from, u64 n);
public void *mtb_dynarr_remove(MtbDynArr *array, u64 at);
public void *mtb_dynarr_get(MtbDynArr *array, u64 at);

public void *mtb_dynarr_copy_n(MtbDynArr *array, u64 from, void *src, u64 n);


/* Stack API */

public void *mtb_dynarr_push(MtbDynArr *array);
public void *mtb_dynarr_pop(MtbDynArr *array);
public void *mtb_dynarr_top(MtbDynArr *array);


/* Queue API */

public void *mtb_dynarr_enq(MtbDynArr *array);
public void *mtb_dynarr_deq(MtbDynArr *array);
public void *mtb_dynarr_front(MtbDynArr *array);


#endif //MTB_MTB_DYNARR_H
