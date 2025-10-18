#ifndef MTB_HMAP_H
#define MTB_HMAP_H

#include "mtb_macro.h"
#include "mtb_type.h"
#include "mtb_arena.h"


#ifndef MTB_HMAP_MIN_CAPACITY
#define MTB_HMAP_MIN_CAPACITY 16
#endif

#define mtb_hmap_entry(hmap, index) ((hmap)->entries + (index) * (hmap)->entrySize)
#define mtb_hmap_entry_header(hmap, entry) ((MtbHmapHeader *)(entry))
#define mtb_hmap_entry_key(hmap, entry) ((entry) + (hmap)->headerSize)
#define mtb_hmap_entry_value(hmap, entry) (mtb_hmap_entry_key(hmap, entry) + (hmap)->keySize)


typedef u8 MtbHmapEntryStatus;
enum
{
    MTB_HMAP_ENTRY_FREE = 0,
    MTB_HMAP_ENTRY_OCCUPIED = 1,
    MTB_HMAP_ENTRY_REMOVED = 2,
};

typedef struct mtb_hmap_header MtbHmapHeader;
struct mtb_hmap_header
{
    MtbHmapEntryStatus status;
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

    u64 (*key_hash)(void *k);
    bool (*key_equals)(void *k1, void *k2);
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
                              u64 (*key_hash)(void *k),
                              bool (*key_equals)(void *k1, void *k2),
                              MtbHmapInitOptions opt);
#define mtb_hmap_init(hmap, arena, keyType, valueType, key_hash, key_equals, ...) \
    mtb_hmap_init_opt(hmap, \
                      arena, \
                      sizeof(keyType), \
                      sizeof(valueType), \
                      key_hash, \
                      key_equals, \
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


/* Iterator API */

typedef struct mtb_hmap_iter MtbHmapIter;
struct mtb_hmap_iter
{
    MtbHmap *hmap;
    u8 *prev;
    u8 *next;
};


public void mtb_hmap_iter_init(MtbHmapIter *it, MtbHmap *hmap);
public void mtb_hmap_iter_reset(MtbHmapIter *it);
public bool mtb_hmap_iter_has_next(MtbHmapIter *it);
public void *mtb_hmap_iter_next(MtbHmapIter *it);
public void *mtb_hmap_iter_next_key(MtbHmapIter *it);
public void *mtb_hmap_iter_next_value(MtbHmapIter *it);
public void *mtb_hmap_iter_remove(MtbHmapIter *it);

#endif //MTB_HMAP_H
