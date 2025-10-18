#include "mtb_hmap.h"


#define _mtb_hmap_threshold(capacity) ((capacity) - ((capacity) >> 2)) // 0.75 * capacity
#define _mtb_hmap_modulo_capacity(hmap, n) ((n) & ((hmap)->capacity - 1))


public void
mtb_hmap_init_opt(MtbHmap *hmap,
                  MtbArena *arena,
                  u64 keySize,
                  u64 valueSize,
                  u64 (*key_hash)(void *k),
                  bool (*key_equals)(void *k1, void *k2),
                  MtbHmapInitOptions opt)
{
    mtb_assert_always(mtb_is_pow2_or_zero(opt.capacity));
    mtb_assert_always(mtb_is_pow2_or_zero(keySize));
    mtb_assert_always(mtb_is_pow2_or_zero(opt.keyAlign));
    mtb_assert_always(mtb_is_pow2_or_zero(valueSize));
    mtb_assert_always(mtb_is_pow2_or_zero(opt.valueAlign));

    hmap->arena = arena;

    hmap->capacity = opt.capacity < MTB_HMAP_MIN_CAPACITY ? MTB_HMAP_MIN_CAPACITY : opt.capacity;
    hmap->count = 0;

    u64 align = mtb_max_u64(mtb_alignof(MtbHmapHeader), mtb_max_u64(opt.keyAlign, opt.valueAlign));
    hmap->headerSize = mtb_align_pow2(sizeof(MtbHmapHeader), align);
    hmap->keySize = mtb_align_pow2(keySize, align);
    hmap->valueSize = mtb_align_pow2(valueSize, align);
    hmap->entrySize = hmap->headerSize + hmap->keySize + hmap->valueSize;

    hmap->entries = mtb_arena_bump(hmap->arena, u8, hmap->capacity * hmap->entrySize);

    hmap->key_hash = key_hash;
    hmap->key_equals = key_equals;
}

public void
mtb_hmap_clear(MtbHmap *hmap)
{
    hmap->count = 0;
    memset(hmap->entries, 0, hmap->capacity * hmap->entrySize);
}

public bool
mtb_hmap_is_empty(MtbHmap *hmap)
{
    return hmap->count == 0;
}

public u64
mtb_hmap_calc_capacity(u64 n)
{
    u64 capacity = n + (n / 3);
    if (capacity < MTB_HMAP_MIN_CAPACITY) {
        return MTB_HMAP_MIN_CAPACITY;
    }
    return mtb_roundup_pow2(capacity);
}

public void
mtb_hmap_grow(MtbHmap *hmap, u64 capacity)
{
    mtb_assert_always(mtb_is_pow2(capacity));
    mtb_assert_always(capacity > hmap->capacity);

    MtbHmap oldHmap = *hmap;

    hmap->count = 0;
    hmap->capacity = capacity;
    hmap->entries = mtb_arena_bump(hmap->arena, u8, hmap->capacity * hmap->entrySize);

    for (u64 oldIndex = 0; oldIndex < oldHmap.capacity; oldIndex++) {
        u8 *oldEntry = mtb_hmap_entry(&oldHmap, oldIndex);
        if (mtb_hmap_entry_header(&oldHmap, oldEntry)->status != MTB_HMAP_ENTRY_OCCUPIED) {
            continue;
        }
        u64 hash = hmap->key_hash(mtb_hmap_entry_key(&oldHmap, oldEntry));
        u64 index = _mtb_hmap_modulo_capacity(hmap, hash);
        u8 *entry = mtb_hmap_entry(hmap, index);
        while (mtb_hmap_entry_header(hmap, entry)->status == MTB_HMAP_ENTRY_OCCUPIED) {
            index = _mtb_hmap_modulo_capacity(hmap, index + 1);
            entry = mtb_hmap_entry(hmap, index);
        }
        memcpy(entry, oldEntry, hmap->entrySize);
        hmap->count++;
    }
}

public void *
mtb_hmap_put(MtbHmap *hmap, void *key)
{
    if (hmap->count >= _mtb_hmap_threshold(hmap->capacity)) {
        mtb_hmap_grow(hmap, hmap->capacity << 1);
    }
    u64 hash = hmap->key_hash(key);
    u64 index = _mtb_hmap_modulo_capacity(hmap, hash);
    u8 *entry = mtb_hmap_entry(hmap, index);
    while (mtb_hmap_entry_header(hmap, entry)->status == MTB_HMAP_ENTRY_OCCUPIED) {
        if (hmap->key_equals(mtb_hmap_entry_key(hmap, entry), key)) {
            return mtb_hmap_entry_value(hmap, entry);
        }
        index = _mtb_hmap_modulo_capacity(hmap, index + 1);
        entry = mtb_hmap_entry(hmap, index);
    }
    mtb_hmap_entry_header(hmap, entry)->status = MTB_HMAP_ENTRY_OCCUPIED;
    memcpy(mtb_hmap_entry_key(hmap, entry), key, hmap->keySize);
    hmap->count++;
    return mtb_hmap_entry_value(hmap, entry);
}

public void *
mtb_hmap_remove(MtbHmap *hmap, void *key)
{
    u64 hash = hmap->key_hash(key);
    u64 index = _mtb_hmap_modulo_capacity(hmap, hash);
    u8 *entry = mtb_hmap_entry(hmap, index);
    while (mtb_hmap_entry_header(hmap, entry)->status != MTB_HMAP_ENTRY_FREE) {
        if (mtb_hmap_entry_header(hmap, entry)->status != MTB_HMAP_ENTRY_REMOVED) {
            if (hmap->key_equals(mtb_hmap_entry_key(hmap, entry), key)) {
                mtb_hmap_entry_header(hmap, entry)->status = MTB_HMAP_ENTRY_REMOVED;
                hmap->count--;
                return mtb_hmap_entry_value(hmap, entry);
            }
        }
        index = _mtb_hmap_modulo_capacity(hmap, index + 1);
        entry = mtb_hmap_entry(hmap, index);
    }
    return nil;
}

public void *
mtb_hmap_get(MtbHmap *hmap, void *key)
{
    u64 hash = hmap->key_hash(key);
    u64 index = _mtb_hmap_modulo_capacity(hmap, hash);
    u8 *entry = mtb_hmap_entry(hmap, index);
    while (mtb_hmap_entry_header(hmap, entry)->status != MTB_HMAP_ENTRY_FREE) {
        if (mtb_hmap_entry_header(hmap, entry)->status != MTB_HMAP_ENTRY_REMOVED) {
            if (hmap->key_equals(mtb_hmap_entry_key(hmap, entry), key)) {
                return mtb_hmap_entry_value(hmap, entry);
            }
        }
        index = _mtb_hmap_modulo_capacity(hmap, index + 1);
        entry = mtb_hmap_entry(hmap, index);
    }
    return nil;
}

public void
mtb_hmap_iter_init(MtbHmapIter *it, MtbHmap *hmap)
{
    it->hmap = hmap;
    mtb_hmap_iter_reset(it);
}

public void
mtb_hmap_iter_reset(MtbHmapIter *it)
{
    it->prev = nil;
    it->next = nil;
}

public bool
mtb_hmap_iter_has_next(MtbHmapIter *it)
{
    u8 *next = it->prev == nil ? it->hmap->entries : it->prev + it->hmap->entrySize;
    u8 *end = it->hmap->entries + it->hmap->capacity * it->hmap->entrySize;
    while (next < end) {
        if (mtb_hmap_entry_header(it->hmap, next)->status == MTB_HMAP_ENTRY_OCCUPIED) {
            it->next = next;
            return true;
        }
        next += it->hmap->entrySize;
    }
    return false;
}

public void *
mtb_hmap_iter_next(MtbHmapIter *it)
{
    mtb_assert_always(it->next != nil);
    it->prev = it->next;
    it->next = nil;
    return it->prev;
}

public void *
mtb_hmap_iter_next_key(MtbHmapIter *it)
{
    return mtb_hmap_entry_key(it->hmap, mtb_hmap_iter_next(it));
}

public void *
mtb_hmap_iter_next_value(MtbHmapIter *it)
{
    return mtb_hmap_entry_value(it->hmap, mtb_hmap_iter_next(it));
}

public void *
mtb_hmap_iter_remove(MtbHmapIter *it)
{
    mtb_assert_always(it->prev != nil);
    mtb_hmap_entry_header(it->hmap, it->prev)->status = MTB_HMAP_ENTRY_REMOVED;
    it->hmap->count--;
    return it->prev;
}


#ifdef MTB_HMAP_TESTS

#include <assert.h>
#include <string.h>


intern u64
calc_hash_str(void *key)
{
    char *s = *(char **)key;
    u64 hash = 0;
    while (*s != '\0') hash = 17000069 * hash + *s++;
    return hash;
}

intern bool
is_equal_str(void *key1, void *key2)
{
    char *s1 = *(char **)key1;
    char *s2 = *(char **)key2;
    return strcmp(s1, s2) == 0;
}

intern void
test_mtb_hmap_calc_capacity(void)
{
    for (u64 i = 0; i <= _mtb_hmap_threshold(MTB_HMAP_MIN_CAPACITY); i++) {
        assert(mtb_hmap_calc_capacity(i) == MTB_HMAP_MIN_CAPACITY);
    }
    assert(mtb_hmap_calc_capacity(_mtb_hmap_threshold(MTB_HMAP_MIN_CAPACITY) + 1) == 32);
    assert(mtb_hmap_calc_capacity(MTB_HMAP_MIN_CAPACITY) == 32);
    assert(mtb_hmap_calc_capacity(45) == 64);
    assert(mtb_hmap_calc_capacity(100) == 256);
    assert(mtb_hmap_calc_capacity(200) == 512);
    assert(mtb_hmap_calc_capacity(400) == 1024);
}

intern void
test_mtb_hmap_put(MtbArena arena)
{
    char *text = "Lorem ipsum dolor sit amet consectetuer adipiscing elit Pellentesque ipsum Fusce"
                 " dui leo imperdiet in aliquam sit amet feugiat eu orci Etiam neque Fusce consect"
                 "etuer risus a nunc Cum sociis natoque penatibus et magnis dis parturient montes "
                 "nascetur ridiculus mus Donec quis nibh at felis congue commodo Nullam sit amet m"
                 "agna in magna gravida vehicula Curabitur vitae diam non enim vestibulum interdum"
                 " Donec iaculis gravida nulla Etiam sapien elit consequat eget tristique non vene"
                 "natis quis ante Fusce tellus Morbi scelerisque luctus velit Maecenas ipsum velit"
                 " consectetuer eu lobortis ut dictum at dui Fusce tellus Proin in tellus sit amet"
                 " nibh dignissim sagittis Et harum quidem rerum facilis est et expedita distincti"
                 "o Nullam at arcu a est sollicitudin euismod Vivamus luctus egestas leo Suspendis"
                 "se sagittis ultrices augueFusce consectetuer risus a nunc Proin mattis lacinia j"
                 "usto Mauris metus Inlaoreet magna id viverra tincidunt sem odio bibendum justo v"
                 "el imperdiet sapien wisi sed libero Mauris dolor felis sagittis at luctus sed al"
                 "iquam non tellus Nam libero tempore cum soluta nobis est eligendi optio cumque n"
                 "ihil impedit quo minus id quod maxime placeat facere possimus omnis voluptas ass"
                 "umenda est omnis dolor repellendus Cras elementum Nullam dapibus fermentum ipsum"
                 " Aliquam erat volutpat Curabitur sagittis hendrerit ante Nunc dapibus tortor vel"
                 " mi dapibus sollicitudin Nunc auctor Neque porro quisquam est quidolorem ipsum q"
                 "uia dolor sit amet consectetur adipisci velit sed quia non numquam eius modi tem"
                 "pora incidunt ut labore et dolore magnam aliquam quaerat voluptatem Maecenas fer"
                 "mentum sem in pharetra pellentesque velit turpis volutpat ante in pharetra metus"
                 " odio a lectus Nam sed tellus id magna elementum tincidunt Fusce consectetuer ri"
                 "sus a nunc Nulla turpis magna cursus sit amet suscipit a interdum id felis Excep"
                 "teur sint occaecat cupidatat non proident sunt in culpa qui officia deserunt mol"
                 "lit anim id est laborum Mauris elementum mauris vitae tortor Aliquam erat volutp"
                 "at In rutrum Temporibus autemquibusdam et aut officiis debitis aut rerum necessi"
                 "tatibus saepe eveniet ut et voluptates repudiandae sint et molestiae non recusan"
                 "dae Vivamus ac leo pretium faucibus Class aptent taciti sociosqu ad litora torqu"
                 "ent per conubia nostra per inceptos hymenaeos Etiam commodo dui eget wisi Etiam "
                 "dictum tincidunt diam Nullam eget nisl Itaque earum rerum hic tenetur a sapiente"
                 " delectus ut aut reiciendis voluptatibus maiores alias consequatur aut perferend"
                 "is doloribus asperiores repellat Phasellus rhoncus Duis viverra diam non justo S"
                 "ed convallis magna eu sem Proin pede metus vulputate nec fermentum fringilla veh"
                 "icula vitae justo Maecenas ipsum velit consectetuer eu lobortis ut dictum at dui"
                 " Nullam lectus justo vulputate eget mollis sed tempor sed magna Nullam feugiat t"
                 "urpis at pulvinar vulputate erat libero tristique tellus necbibendum odio risus "
                 "sit amet ante Etiam sapien elit consequat eget tristique non venenatis quis ante"
                 " Nulla est Nullam faucibus mi quis velit Duis condimentum augue id magna semper "
                 "rutrum Aliquam erat volutpat Mauris metus Phasellus enim erat vestibulum vel ali"
                 "quam a posuere eu velit Donec ipsummassa ullamcorper in auctor et scelerisque se"
                 "d est Lorem ipsum dolor sitamet consectetuer adipiscing elit Curabitur bibendum "
                 "justo non orci Sed elit dui pellentesque a faucibus vel interdum nec diam Fusce "
                 "consectetuer risus a nunc Cras pede libero dapibus nec pretium sit amet tempor q"
                 "uis Maecenas lorem Morbi imperdiet mauris ac auctor dictum nisl ligula egestas n"
                 "ulla et sollicitudin sem purus in lacus Curabitur sagittis hendrerit ante Nullam"
                 " faucibus mi quis velit Nullam justo enim consectetuer nec ullamcorper ac vestib"
                 "ulum in elit Suspendisse sagittis ultrices augue Etiam dictum tincidunt diam Pro"
                 "in mattis lacinia justo Quisque porta Maecenas ipsum velit consectetuer eu lobor"
                 "tis ut dictum at dui";
    // echo text | tr ' ' '\n' | sort | uniq -c | sort -nrk1
    struct histogram {
        char *word;
        u64 count;
    } hist[] = {
        { .word = "consectetuer", .count =10 },
        { .word = "a", .count =10 },
        { .word = "velit", .count = 9 },
        { .word = "Nullam", .count = 9 },
        { .word = "non", .count = 9 },
        { .word = "in", .count = 9 },
        { .word = "sit", .count = 8 },
        { .word = "magna", .count = 8 },
        { .word = "justo", .count = 8 },
        { .word = "ipsum", .count = 8 },
        { .word = "et", .count = 8 },
        { .word = "est", .count = 8 },
        { .word = "amet", .count = 8 },
        { .word = "sed", .count = 7 },
        { .word = "at", .count = 7 },
        { .word = "ut", .count = 6 },
        { .word = "tellus", .count = 6 },
        { .word = "sagittis", .count = 6 },
        { .word = "quis", .count = 6 },
        { .word = "id", .count = 6 },
        { .word = "Fusce", .count = 6 },
        { .word = "eu", .count = 6 },
        { .word = "Etiam", .count = 6 },
        { .word = "elit", .count = 6 },
        { .word = "dui", .count = 6 },
        { .word = "dictum", .count = 6 },
        { .word = "ante", .count = 6 },
        { .word = "risus", .count = 5 },
        { .word = "Maecenas", .count = 5 },
        { .word = "erat", .count = 5 },
        { .word = "eget", .count = 5 },
        { .word = "dolor", .count = 5 },
        { .word = "diam", .count = 5 },
        { .word = "volutpat", .count = 4 },
        { .word = "vel", .count = 4 },
        { .word = "tincidunt", .count = 4 },
        { .word = "sem", .count = 4 },
        { .word = "Proin", .count = 4 },
        { .word = "nunc", .count = 4 },
        { .word = "nec", .count = 4 },
        { .word = "metus", .count = 4 },
        { .word = "Mauris", .count = 4 },
        { .word = "libero", .count = 4 },
        { .word = "faucibus", .count = 4 },
        { .word = "dapibus", .count = 4 },
        { .word = "Curabitur", .count = 4 },
        { .word = "aut", .count = 4 },
        { .word = "aliquam", .count = 4 },
        { .word = "vulputate", .count = 3 },
        { .word = "vitae", .count = 3 },
        { .word = "vestibulum", .count = 3 },
        { .word = "turpis", .count = 3 },
        { .word = "tristique", .count = 3 },
        { .word = "sollicitudin", .count = 3 },
        { .word = "sapien", .count = 3 },
        { .word = "rerum", .count = 3 },
        { .word = "odio", .count = 3 },
        { .word = "mi", .count = 3 },
        { .word = "luctus", .count = 3 },
        { .word = "lobortis", .count = 3 },
        { .word = "leo", .count = 3 },
        { .word = "interdum", .count = 3 },
        { .word = "imperdiet", .count = 3 },
        { .word = "fermentum", .count = 3 },
        { .word = "felis", .count = 3 },
        { .word = "enim", .count = 3 },
        { .word = "elementum", .count = 3 },
        { .word = "Donec", .count = 3 },
        { .word = "auctor", .count = 3 },
        { .word = "Aliquam", .count = 3 },
        { .word = "ac", .count = 3 },
        { .word = "wisi", .count = 2 },
        { .word = "viverra", .count = 2 },
        { .word = "Vivamus", .count = 2 },
        { .word = "venenatis", .count = 2 },
        { .word = "vehicula", .count = 2 },
        { .word = "ultrices", .count = 2 },
        { .word = "ullamcorper", .count = 2 },
        { .word = "tortor", .count = 2 },
        { .word = "tempor", .count = 2 },
        { .word = "Suspendisse", .count = 2 },
        { .word = "sint", .count = 2 },
        { .word = "Sed", .count = 2 },
        { .word = "scelerisque", .count = 2 },
        { .word = "rutrum", .count = 2 },
        { .word = "quia", .count = 2 },
        { .word = "pretium", .count = 2 },
        { .word = "Phasellus", .count = 2 },
        { .word = "pharetra", .count = 2 },
        { .word = "per", .count = 2 },
        { .word = "pellentesque", .count = 2 },
        { .word = "pede", .count = 2 },
        { .word = "orci", .count = 2 },
        { .word = "omnis", .count = 2 },
        { .word = "Nunc", .count = 2 },
        { .word = "Nulla", .count = 2 },
        { .word = "nulla", .count = 2 },
        { .word = "nisl", .count = 2 },
        { .word = "nibh", .count = 2 },
        { .word = "Nam", .count = 2 },
        { .word = "Morbi", .count = 2 },
        { .word = "mauris", .count = 2 },
        { .word = "mattis", .count = 2 },
        { .word = "Lorem", .count = 2 },
        { .word = "lectus", .count = 2 },
        { .word = "lacinia", .count = 2 },
        { .word = "hendrerit", .count = 2 },
        { .word = "gravida", .count = 2 },
        { .word = "feugiat", .count = 2 },
        { .word = "egestas", .count = 2 },
        { .word = "Duis", .count = 2 },
        { .word = "Cras", .count = 2 },
        { .word = "consequat", .count = 2 },
        { .word = "commodo", .count = 2 },
        { .word = "bibendum", .count = 2 },
        { .word = "augue", .count = 2 },
        { .word = "adipiscing", .count = 2 },
        { .word = "voluptatibus", .count = 1 },
        { .word = "voluptates", .count = 1 },
        { .word = "voluptatem", .count = 1 },
        { .word = "voluptas", .count = 1 },
        { .word = "torquent", .count = 1 },
        { .word = "tenetur", .count = 1 },
        { .word = "Temporibus", .count = 1 },
        { .word = "tempore", .count = 1 },
        { .word = "tempora", .count = 1 },
        { .word = "taciti", .count = 1 },
        { .word = "suscipit", .count = 1 },
        { .word = "sunt", .count = 1 },
        { .word = "soluta", .count = 1 },
        { .word = "sociosqu", .count = 1 },
        { .word = "sociis", .count = 1 },
        { .word = "sitamet", .count = 1 },
        { .word = "semper", .count = 1 },
        { .word = "sapiente", .count = 1 },
        { .word = "saepe", .count = 1 },
        { .word = "ridiculus", .count = 1 },
        { .word = "rhoncus", .count = 1 },
        { .word = "repudiandae", .count = 1 },
        { .word = "repellendus", .count = 1 },
        { .word = "repellat", .count = 1 },
        { .word = "reiciendis", .count = 1 },
        { .word = "recusandae", .count = 1 },
        { .word = "quod", .count = 1 },
        { .word = "quo", .count = 1 },
        { .word = "Quisque", .count = 1 },
        { .word = "quisquam", .count = 1 },
        { .word = "quidolorem", .count = 1 },
        { .word = "quidem", .count = 1 },
        { .word = "qui", .count = 1 },
        { .word = "quaerat", .count = 1 },
        { .word = "purus", .count = 1 },
        { .word = "pulvinar", .count = 1 },
        { .word = "proident", .count = 1 },
        { .word = "posuere", .count = 1 },
        { .word = "possimus", .count = 1 },
        { .word = "porta", .count = 1 },
        { .word = "porro", .count = 1 },
        { .word = "placeat", .count = 1 },
        { .word = "perferendis", .count = 1 },
        { .word = "penatibus", .count = 1 },
        { .word = "Pellentesque", .count = 1 },
        { .word = "parturient", .count = 1 },
        { .word = "optio", .count = 1 },
        { .word = "officiis", .count = 1 },
        { .word = "officia", .count = 1 },
        { .word = "occaecat", .count = 1 },
        { .word = "numquam", .count = 1 },
        { .word = "nostra", .count = 1 },
        { .word = "nobis", .count = 1 },
        { .word = "nihil", .count = 1 },
        { .word = "Neque", .count = 1 },
        { .word = "neque", .count = 1 },
        { .word = "necessitatibus", .count = 1 },
        { .word = "necbibendum", .count = 1 },
        { .word = "natoque", .count = 1 },
        { .word = "nascetur", .count = 1 },
        { .word = "mus", .count = 1 },
        { .word = "montes", .count = 1 },
        { .word = "mollit", .count = 1 },
        { .word = "mollis", .count = 1 },
        { .word = "molestiae", .count = 1 },
        { .word = "modi", .count = 1 },
        { .word = "minus", .count = 1 },
        { .word = "maxime", .count = 1 },
        { .word = "maiores", .count = 1 },
        { .word = "magnis", .count = 1 },
        { .word = "magnam", .count = 1 },
        { .word = "lorem", .count = 1 },
        { .word = "litora", .count = 1 },
        { .word = "ligula", .count = 1 },
        { .word = "lacus", .count = 1 },
        { .word = "laborum", .count = 1 },
        { .word = "labore", .count = 1 },
        { .word = "Itaque", .count = 1 },
        { .word = "ipsummassa", .count = 1 },
        { .word = "Inlaoreet", .count = 1 },
        { .word = "incidunt", .count = 1 },
        { .word = "inceptos", .count = 1 },
        { .word = "In", .count = 1 },
        { .word = "impedit", .count = 1 },
        { .word = "iaculis", .count = 1 },
        { .word = "hymenaeos", .count = 1 },
        { .word = "hic", .count = 1 },
        { .word = "harum", .count = 1 },
        { .word = "fringilla", .count = 1 },
        { .word = "facilis", .count = 1 },
        { .word = "facere", .count = 1 },
        { .word = "expedita", .count = 1 },
        { .word = "Excepteur", .count = 1 },
        { .word = "eveniet", .count = 1 },
        { .word = "euismod", .count = 1 },
        { .word = "Et", .count = 1 },
        { .word = "eligendi", .count = 1 },
        { .word = "eius", .count = 1 },
        { .word = "earum", .count = 1 },
        { .word = "doloribus", .count = 1 },
        { .word = "dolore", .count = 1 },
        { .word = "distinctio", .count = 1 },
        { .word = "dis", .count = 1 },
        { .word = "dignissim", .count = 1 },
        { .word = "deserunt", .count = 1 },
        { .word = "delectus", .count = 1 },
        { .word = "debitis", .count = 1 },
        { .word = "cursus", .count = 1 },
        { .word = "cupidatat", .count = 1 },
        { .word = "cumque", .count = 1 },
        { .word = "Cum", .count = 1 },
        { .word = "cum", .count = 1 },
        { .word = "culpa", .count = 1 },
        { .word = "convallis", .count = 1 },
        { .word = "conubia", .count = 1 },
        { .word = "consequatur", .count = 1 },
        { .word = "consectetur", .count = 1 },
        { .word = "congue", .count = 1 },
        { .word = "condimentum", .count = 1 },
        { .word = "Class", .count = 1 },
        { .word = "autemquibusdam", .count = 1 },
        { .word = "augueFusce", .count = 1 },
        { .word = "assumenda", .count = 1 },
        { .word = "asperiores", .count = 1 },
        { .word = "arcu", .count = 1 },
        { .word = "aptent", .count = 1 },
        { .word = "anim", .count = 1 },
        { .word = "alias", .count = 1 },
        { .word = "adipisci", .count = 1 },
        { .word = "ad", .count = 1 },
    };

    MtbHmap hmap = {0};
    mtb_hmap_init(&hmap, &arena, char *, u64, calc_hash_str, is_equal_str);
    assert(mtb_hmap_is_empty(&hmap));

    char *textCopy = strcpy(mtb_arena_bump(&arena, char, strlen(text) + 1), text);
    char *token = strtok(textCopy, " ");
    while (token != nil) {
        u64 *count = mtb_hmap_get(&hmap, &token);
        if (count == nil) {
            *(u64 *)mtb_hmap_put(&hmap, &token) = 1;
        }
        else {
            *count += 1;
        }
        token = strtok(nil, " ");
    }
    assert(!mtb_hmap_is_empty(&hmap));
    for (u64 i = 0; i < mtb_countof(hist); i++) {
        struct histogram *wc = hist + i;
        assert(*(u64 *)mtb_hmap_get(&hmap, &wc->word) == wc->count);
    }

    mtb_hmap_clear(&hmap);
    assert(mtb_hmap_is_empty(&hmap));
}

intern void
test_mtb_hmap_remove(MtbArena arena)
{
    MtbHmap hmap = {0};
    mtb_hmap_init(&hmap, &arena, sizeof(char *), sizeof(u64), calc_hash_str, is_equal_str);

    char *k1 = "Pizza";
    u64 v1 = 11;

    assert(mtb_hmap_remove(&hmap, &k1) == nil);

    for (int i = 0; i < 5; i++) {
        *(u64 *)mtb_hmap_put(&hmap, &k1) = v1;
        assert(*(u64 *)mtb_hmap_remove(&hmap, &k1) == v1);
        assert(mtb_hmap_remove(&hmap, &k1) == nil);
    }

    assert(mtb_hmap_is_empty(&hmap));
}

intern void
test_mtb_hmap_iter(MtbArena arena)
{
    MtbHmap hmap = {0};
    mtb_hmap_init(&hmap, &arena, sizeof(char *), sizeof(u64), calc_hash_str, is_equal_str);

    MtbHmapIter it = {0};
    mtb_hmap_iter_init(&it, &hmap);

    assert(!mtb_hmap_iter_has_next(&it));

    struct {
        char *letter;
        u64 order;
    } alphabet[] = {
        { .letter = "a", .order = 0 },
        { .letter = "b", .order = 1 },
        { .letter = "c", .order = 2 },
        { .letter = "d", .order = 3 },
        { .letter = "e", .order = 4 },
        { .letter = "f", .order = 5 },
        { .letter = "g", .order = 6 },
        { .letter = "h", .order = 7 },
        { .letter = "i", .order = 8 },
        { .letter = "j", .order = 9 },
        { .letter = "k", .order = 10 },
        { .letter = "l", .order = 11 },
        { .letter = "m", .order = 12 },
        { .letter = "n", .order = 13 },
        { .letter = "o", .order = 14 },
        { .letter = "p", .order = 15 },
        { .letter = "q", .order = 16 },
        { .letter = "r", .order = 17 },
        { .letter = "s", .order = 18 },
        { .letter = "t", .order = 19 },
        { .letter = "u", .order = 20 },
        { .letter = "v", .order = 21 },
        { .letter = "w", .order = 22 },
        { .letter = "x", .order = 23 },
        { .letter = "y", .order = 24 },
        { .letter = "z", .order = 25 },
    };

    char *removed[] = { "k", "o", "t", "e", "n" };

    for (u64 i = 0; i < mtb_countof(alphabet); i++) {
        *(u64 *)mtb_hmap_put(&hmap, &alphabet[i].letter) = alphabet[i].order;
    }

    mtb_hmap_iter_reset(&it);
    while (mtb_hmap_iter_has_next(&it)) {
        char *key = *(char **)mtb_hmap_iter_next_key(&it);
        for (u64 i = 0; i < mtb_countof(removed); i++) {
            if (strcmp(key, removed[i]) == 0) {
                mtb_hmap_iter_remove(&it);
            }
        }
    }
    for (u64 i = 0; i < mtb_countof(removed); i++) {
        assert(mtb_hmap_get(&hmap, &removed[i]) == nil);
    }

    mtb_hmap_iter_reset(&it);
    while (mtb_hmap_iter_has_next(&it)) {
        mtb_hmap_iter_next(&it);
        mtb_hmap_iter_remove(&it);
    }
    assert(mtb_hmap_is_empty(&hmap));

    mtb_hmap_iter_reset(&it);
    assert(!mtb_hmap_iter_has_next(&it));
}

intern void
test_mtb_hmap(void)
{
    MtbArena arena = {0};
    mtb_arena_init(&arena, mb(1), &MTB_ARENA_DEF_ALLOCATOR);

    test_mtb_hmap_put(arena);
    test_mtb_hmap_remove(arena);
    test_mtb_hmap_calc_capacity();
    test_mtb_hmap_iter(arena);

    mtb_arena_deinit(&arena);
}

#endif // MTB_HMAP_TESTS
