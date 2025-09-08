#ifndef MTB_STRING_H
#define MTB_STRING_H

#include <stdarg.h>

#include "mtb_macro.h"
#include "mtb_type.h"
#include "mtb_list.h"
#include "mtb_arena.h"


/* String */

#define MTB_STR_NOT_FOUND U64_MAX

typedef struct mtb_str MtbStr;
struct mtb_str
{
    union {
        u8 *bytes;
        char *chars; // not null-terminated!
    };
    u64 length; // in bytes
};

typedef struct mtb_str_cmp_options MtbStrCmpOptions;
struct mtb_str_cmp_options
{
    bool ignoreCase;
};

typedef struct mtb_str_find_options MtbStrFindOptions;
struct mtb_str_find_options
{
    bool ignoreCase;
};

typedef struct mtb_str_split_options MtbStrSplitOptions;
struct mtb_str_split_options
{
    bool skipEmpty;
};

typedef struct mtb_str_join_options MtbStrJoinOptions;
struct mtb_str_join_options
{
    bool skipEmpty;
};

#define mtb_str_varg(s) (i32)((s).length), ((s).chars)

#define mtb_str(b, l) ((MtbStr){ .bytes = (b), .length = (l) })
#define mtb_str_char(c) mtb_str((u8[]){ (c) }, 1)
#define mtb_str_lit(c) mtb_str((u8 *)(c), sizeof(c) - 1)
#define mtb_str_empty() mtb_str_lit("")

public MtbStr mtb_str_from_cstr(char *cstr);
public char *mtb_str_to_cstr(MtbArena *arena, MtbStr str);

public MtbStr mtb_str_vsprintf(MtbArena *arena, char *fmt, va_list args);
public MtbStr mtb_str_sprintf(MtbArena *arena, char *fmt, ...);

public i32 mtb_str_cmp_opt(MtbStr a, MtbStr b, MtbStrCmpOptions opt);
#define mtb_str_cmp(a, b, ...) mtb_str_cmp_opt(a, b, (MtbStrCmpOptions){ __VA_ARGS__ })
#define mtb_str_cmp_lit(a, b, ...) mtb_str_cmp(a, mtb_str_lit(b), __VA_ARGS__)
#define mtb_str_cmp_cstr(a, b, ...) mtb_str_cmp(a, mtb_str_from_cstr(b), __VA_ARGS__)
#define mtb_str_is_equal(a, b, ...) (mtb_str_cmp(a, b, __VA_ARGS__) == 0)
#define mtb_str_is_equal_lit(a, b, ...) mtb_str_is_equal(a, mtb_str_lit(b), __VA_ARGS__)
#define mtb_str_is_equal_cstr(a, b, ...) mtb_str_is_equal(a, mtb_str_from_cstr(b), __VA_ARGS__)

public u64 mtb_str_find_opt(MtbStr str, MtbStr pattern, MtbStrFindOptions opt);
#define mtb_str_find(str, pattern, ...) mtb_str_find_opt(str, pattern, (MtbStrFindOptions){ __VA_ARGS__ })
#define mtb_str_find_lit(str, pattern, ...) mtb_str_find(str, mtb_str_lit(pattern), __VA_ARGS__)
#define mtb_str_find_char(str, pattern, ...) mtb_str_find(str, mtb_str_char(pattern), __VA_ARGS__)
#define mtb_str_find_cstr(str, pattern, ...) mtb_str_find(str, mtb_str_from_cstr(pattern), __VA_ARGS__)
#define mtb_str_contains(str, pattern, ...) ((bool)(mtb_str_find(str, pattern, __VA_ARGS__) != MTB_STR_NOT_FOUND))
#define mtb_str_contains_lit(str, pattern, ...) mtb_str_contains(str, mtb_str_lit(pattern), __VA_ARGS__)
#define mtb_str_contains_char(str, pattern, ...) mtb_str_contains(str, mtb_str_char(pattern), __VA_ARGS__)
#define mtb_str_contains_cstr(str, pattern, ...) mtb_str_contains(str, mtb_str_from_cstr(pattern), __VA_ARGS__)

public bool mtb_str_has_prefix_opt(MtbStr str, MtbStr pfx, MtbStrFindOptions opt);
#define mtb_str_has_prefix(str, pfx, ...) mtb_str_has_prefix_opt(str, pfx, (MtbStrFindOptions){ __VA_ARGS__ })
#define mtb_str_has_prefix_lit(str, pfx, ...) mtb_str_has_prefix(str, mtb_str_lit(pfx), __VA_ARGS__)
#define mtb_str_has_prefix_cstr(str, pfx, ...) mtb_str_has_prefix(str, mtb_str_from_cstr(pfx), __VA_ARGS__)

public bool mtb_str_has_suffix_opt(MtbStr str, MtbStr sfx, MtbStrFindOptions opt);
#define mtb_str_has_suffix(str, sfx, ...) mtb_str_has_suffix_opt(str, sfx, (MtbStrFindOptions){ __VA_ARGS__ })
#define mtb_str_has_suffix_lit(str, sfx, ...) mtb_str_has_suffix(str, mtb_str_lit(sfx), __VA_ARGS__)
#define mtb_str_has_suffix_cstr(str, sfx, ...) mtb_str_has_suffix(str, mtb_str_from_cstr(sfx), __VA_ARGS__)

public MtbStr mtb_str_trim(MtbStr str);
public MtbStr mtb_str_trim_beg(MtbStr str);
public MtbStr mtb_str_trim_end(MtbStr str);
public MtbStr mtb_str_skip(MtbStr str, u64 length);
public MtbStr mtb_str_chop(MtbStr str, u64 length);
public MtbStr mtb_str_to_lower(MtbArena *arena, MtbStr str);
public MtbStr mtb_str_to_upper(MtbArena *arena, MtbStr str);
public MtbStr mtb_str_dup(MtbArena *arena, MtbStr str);
public MtbStr mtb_str_cat(MtbArena *arena, MtbStr a, MtbStr b);
public MtbStr mtb_str_substr(MtbStr str, u64 beg, u64 end);
public MtbStr mtb_str_prefix(MtbStr str, u64 length);
public MtbStr mtb_str_suffix(MtbStr str, u64 length);
public bool mtb_str_is_empty(MtbStr str);


/* String List */

typedef struct mtb_str_list MtbStrList;
struct mtb_str_list
{
    MtbList node;
    union {
        MtbArena *arena;
        MtbStr *str;
    };
};

public void mtb_str_list_init(MtbArena *arena, MtbStrList *list);
public void mtb_str_list_clear(MtbStrList *list);
public bool mtb_str_list_is_empty(MtbStrList *list);

public MtbStrList *mtb_str_list_append(MtbStrList *list, MtbStr *str);
#define mtb_str_list_append_lit(list, str) mtb_str_list_append(list, &mtb_str_lit(str))
public MtbStrList *mtb_str_list_appendf(MtbStrList *list, char *fmt, ...);

public MtbStrList *mtb_str_list_prepend(MtbStrList *list, MtbStr *str);
#define mtb_str_list_prepend_lit(list, str) mtb_str_list_prepend(list, &mtb_str_lit(str))
public MtbStrList *mtb_str_list_prependf(MtbStrList *list, char *fmt, ...);

public MtbStr mtb_str_join_opt(MtbStrList *list, MtbStr delim, MtbStrJoinOptions opt);
#define mtb_str_join(list, delim, ...) mtb_str_join_opt(list, delim, (MtbStrJoinOptions){ __VA_ARGS__ })
#define mtb_str_join_lit(list, delim, ...) mtb_str_join(list, mtb_str_lit(delim), __VA_ARGS__)
#define mtb_str_join_char(list, delim, ...) mtb_str_join(list, mtb_str_char(delim), __VA_ARGS__)

public void mtb_str_split_opt(MtbStrList *list, MtbStr str, MtbStr delim, MtbStrSplitOptions opt);
#define mtb_str_split(list, str, delim, ...) mtb_str_split_opt(list, str, delim, (MtbStrSplitOptions){ __VA_ARGS__ })
#define mtb_str_split_lit(list, str, delim, ...) mtb_str_split(list, str, mtb_str_lit(delim), __VA_ARGS__)
#define mtb_str_split_char(list, str, delim, ...) mtb_str_split(list, str, mtb_str_char(delim), __VA_ARGS__)


/* Hash map utils */

public u64
mtb_str_key_hash(void *key)
{
    MtbStr *s = (MtbStr *)key;
    u64 hash = 0;
    for (u64 i = 0; i < s->length; i++) {
        hash = 17000069 * hash + s->chars[i];
    }
    return hash;
}

public bool
mtb_str_key_equals(void *key1, void *key2)
{
    MtbStr *s1 = (MtbStr *)key1;
    MtbStr *s2 = (MtbStr *)key2;
    return mtb_str_is_equal(*s1, *s2);
}

#endif // MTB_STRING_H
