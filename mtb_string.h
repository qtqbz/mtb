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
#define mtb_str_find(s, p, ...) mtb_str_find_opt(s, p, (MtbStrFindOptions){ __VA_ARGS__ })
#define mtb_str_find_lit(s, p, ...) mtb_str_find(s, mtb_str_lit(p), __VA_ARGS__)
#define mtb_str_find_char(s, c, ...) mtb_str_find(s, mtb_str_char(c), __VA_ARGS__)
#define mtb_str_find_cstr(s, p, ...) mtb_str_find(s, mtb_str_from_cstr(p), __VA_ARGS__)
#define mtb_str_contains(s, p, ...) ((bool)(mtb_str_find(s, p, __VA_ARGS__) != MTB_STR_NOT_FOUND))
#define mtb_str_contains_lit(s, p, ...) mtb_str_contains(s, mtb_str_lit(p), __VA_ARGS__)
#define mtb_str_contains_char(s, c, ...) mtb_str_contains(s, mtb_str_char(c), __VA_ARGS__)
#define mtb_str_contains_cstr(s, p, ...) mtb_str_contains(s, mtb_str_from_cstr(p), __VA_ARGS__)

public bool mtb_str_has_prefix_opt(MtbStr str, MtbStr pfx, MtbStrFindOptions opt);
#define mtb_str_has_prefix(s, p, ...) mtb_str_has_prefix_opt(s, p, (MtbStrFindOptions){ __VA_ARGS__ })
#define mtb_str_has_prefix_lit(s, p, ...) mtb_str_has_prefix(s, mtb_str_lit(p), __VA_ARGS__)
#define mtb_str_has_prefix_cstr(s, p, ...) mtb_str_has_prefix(s, mtb_str_from_cstr(p), __VA_ARGS__)

public bool mtb_str_has_suffix_opt(MtbStr str, MtbStr sfx, MtbStrFindOptions opt);
#define mtb_str_has_suffix(s, x, ...) mtb_str_has_suffix_opt(s, x, (MtbStrFindOptions){ __VA_ARGS__ })
#define mtb_str_has_suffix_lit(s, x, ...) mtb_str_has_suffix(s, mtb_str_lit(x), __VA_ARGS__)
#define mtb_str_has_suffix_cstr(s, x, ...) mtb_str_has_suffix(s, mtb_str_from_cstr(x), __VA_ARGS__)

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
    MtbListNode node;
    union {
        MtbArena *arena;
        MtbStr *str;
    };
};

public void mtb_str_list_init(MtbArena *arena, MtbStrList *list);
public void mtb_str_list_clear(MtbStrList *list);
public bool mtb_str_list_is_empty(MtbStrList *list);

public MtbStrList *mtb_str_list_append(MtbStrList *list, MtbStr *str);
#define mtb_str_list_append_lit(l, s) mtb_str_list_append(l, &mtb_str_lit(s))
public MtbStrList *mtb_str_list_appendf(MtbStrList *list, char *fmt, ...);

public MtbStrList *mtb_str_list_prepend(MtbStrList *list, MtbStr *str);
#define mtb_str_list_prepend_lit(l, s) mtb_str_list_prepend(l, &mtb_str_lit(s))
public MtbStrList *mtb_str_list_prependf(MtbStrList *list, char *fmt, ...);

public MtbStr mtb_str_join_opt(MtbStrList *list, MtbStr delim, MtbStrJoinOptions opt);
#define mtb_str_join(s, d, ...) mtb_str_join_opt(s, d, (MtbStrJoinOptions){ __VA_ARGS__ })
#define mtb_str_join_lit(s, d, ...) mtb_str_join(s, mtb_str_lit(d), __VA_ARGS__)
#define mtb_str_join_char(s, d, ...) mtb_str_join(s, mtb_str_char(d), __VA_ARGS__)

public void mtb_str_split_opt(MtbStrList *list, MtbStr str, MtbStr delim, MtbStrSplitOptions opt);
#define mtb_str_split(l, s, d, ...) mtb_str_split_opt(l, s, d, (MtbStrSplitOptions){ __VA_ARGS__ })
#define mtb_str_split_lit(l, s, d, ...) mtb_str_split(l, s, mtb_str_lit(d), __VA_ARGS__)
#define mtb_str_split_char(l, s, d, ...) mtb_str_split(l, s, mtb_str_char(d), __VA_ARGS__)

#endif // MTB_STRING_H
