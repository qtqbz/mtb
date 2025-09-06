#ifndef MTB_HMAP_H
#define MTB_HMAP_H

#include "mtb_macro.h"
#include "mtb_type.h"
#include "mtb_arena.h"


#define MTB_HMAP_MIN_CAPACITY 16

#define mtb_hmap_threshold(capacity) ((capacity) - ((capacity) >> 2)) // 0.75 * capacity
#define mtb_hmap_modulo_capacity(hmap, n) ((n) & (hmap->capacity - 1))
#define mtb_hmap_entry(hmap, i) ((hmap)->entries + (i) * (hmap)->entrySize)
#define mtb_hmap_entry_header(hmap, i) ((MtbHmapEntryHeader *)mtb_hmap_entry(hmap, i))
#define mtb_hmap_entry_key(hmap, i) (mtb_hmap_entry(hmap, i) + (hmap)->headerSize)
#define mtb_hmap_entry_value(hmap, i) (mtb_hmap_entry_key(hmap, i) + (hmap)->keySize)


typedef struct mtb_hmap_entry_header MtbHmapEntryHeader;
struct mtb_hmap_entry_header
{
    bool occupied : 1;
    bool removed : 1;
};

typedef struct mtb_hmap MtbHmap;
struct mtb_hmap
{
    MtbArena *arena;

    u64 capacity; // must be power of 2!
    u64 count;

    u64 headerSize;
    u64 keySize;
    u64 valueSize;
    u64 entrySize;
    u8 *entries;

    u64 (*calcHash)(void *k);
    bool (*isEqual)(void *k1, void *k2);
};

typedef struct mtb_hmap_init_options MtbHmapInitOptions;
struct mtb_hmap_init_options
{
    u64 capacity;
    u64 keyAlign;
    u64 valueAlign;
};


public void mtb_hmap_init_opt(MtbHmap *hmap,
                              MtbArena *arena,
                              u64 keySize,
                              u64 valueSize,
                              u64 (*calcHash)(void *k),
                              bool (*isEqual)(void *k1, void *k2),
                              MtbHmapInitOptions opt);
#define mtb_hmap_init(hmap, arena, keyType, valueType, calcHash, isEqual, ...) \
    mtb_hmap_init_opt(hmap, \
                      arena, \
                      sizeof(keyType), \
                      sizeof(valueType), \
                      calcHash, isEqual, \
                      (MtbHmapInitOptions){ \
                          .keyAlign = mtb_alignof(keyType), \
                          .valueAlign = mtb_alignof(valueType), \
                          __VA_ARGS__ \
                      })
public void mtb_hmap_clear(MtbHmap *hmap);
public bool mtb_hmap_is_empty(MtbHmap *hmap);
public void mtb_hmap_grow(MtbHmap *hmap, u64 capacity);
public u64 mtb_hmap_calc_capacity(u64 n);

public void *mtb_hmap_put(MtbHmap *hmap, void *key);
public void *mtb_hmap_remove(MtbHmap *hmap, void *key);
public void *mtb_hmap_get(MtbHmap *hmap, void *key);

#endif //MTB_HMAP_H
