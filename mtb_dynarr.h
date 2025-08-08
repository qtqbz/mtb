#ifndef MTB_MTB_DYNARR_H
#define MTB_MTB_DYNARR_H

#include "mtb_macro.h"
#include "mtb_type.h"
#include "mtb_arena.h"


#ifndef MTB_DYNARR_INIT_COUNT
#define MTB_DYNARR_INIT_COUNT 12
#endif
#ifndef MTB_DYNARR_GROWTH_FACTOR
#define MTB_DYNARR_GROWTH_FACTOR 2
#endif

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
    void *_e = _a->items + _a->length * _a->itemSize; \
    for (void *i = _a->items; i < _e; i += _a->itemSize)
#define mtb_dynarr_foreach(a, i) _mtb_dynarr_foreach(a, i, mtb_id(_a), mtb_id(_e))

public void mtb_dynarr_init(MtbArena *arena, MtbDynArr *array, u64 itemSize);
public void mtb_dynarr_grow(MtbDynArr *array, u64 capacity);
#define mtb_dynarr_is_empty(a) ((bool)((a)->length == 0))
#define mtb_dynarr_clear(a) (a)->length = 0

public void *mtb_dynarr_insert_n(MtbDynArr *array, u64 from, u64 n);
#define mtb_dynarr_insert(a, i) mtb_dynarr_insert_n(a, i, 1)
public void *mtb_dynarr_remove_n(MtbDynArr *array, u64 from, u64 n);
#define mtb_dynarr_remove(a, i) mtb_dynarr_remove_n(a, i, 1)
public void *mtb_dynarr_get(MtbDynArr *array, u64 at);


/* Stack API */

#define _mtb_dynarr_push(s, _s) ({ \
    __auto_type _s = (s); \
    mtb_dynarr_insert(_s, (_s)->length); \
})
#define mtb_dynarr_push(s) _mtb_dynarr_push(s, mtb_id(_s))
#define _mtb_dynarr_pop(s, _s) ({ \
    __auto_type _s = (s); \
    mtb_dynarr_remove(_s, (_s)->length - 1); \
})
#define mtb_dynarr_pop(s) _mtb_dynarr_pop(s, mtb_id(_s))
#define _mtb_dynarr_top(s, _s) ({ \
    __auto_type _s = (s); \
    mtb_dynarr_get(_s, (_s)->length - 1); \
})
#define mtb_dynarr_top(s) _mtb_dynarr_top(s, mtb_id(_s))


#endif //MTB_MTB_DYNARR_H
