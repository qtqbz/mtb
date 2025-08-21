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

#define mtb_str_varg(s) (s).length, (s).chars

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
#define mtb_str_is_equal(a, b, ...) (mtb_str_cmp(a, b, __VA_ARGS__) == 0)

public u64 mtb_str_find_opt(MtbStr str, MtbStr pattern, MtbStrCmpOptions opt);
#define mtb_str_find(s, p, ...) mtb_str_find_opt(s, p, (MtbStrCmpOptions){ __VA_ARGS__ })
#define mtb_str_contains(s, p, ...) ((bool)(mtb_str_find(s, p, __VA_ARGS__) != MTB_STR_NOT_FOUND))

public bool mtb_str_has_prefix_opt(MtbStr str, MtbStr pfx, MtbStrCmpOptions opt);
#define mtb_str_has_prefix(s, p, ...) mtb_str_has_prefix_opt(s, p, (MtbStrCmpOptions){ __VA_ARGS__ })

public bool mtb_str_has_suffix_opt(MtbStr str, MtbStr sfx, MtbStrCmpOptions opt);
#define mtb_str_has_suffix(s, x, ...) mtb_str_has_suffix_opt(s, x, (MtbStrCmpOptions){ __VA_ARGS__ })

public MtbStr mtb_str_trim(MtbStr str);
public MtbStr mtb_str_to_lower(MtbArena *arena, MtbStr str);
public MtbStr mtb_str_to_upper(MtbArena *arena, MtbStr str);
public MtbStr mtb_str_dup(MtbArena *arena, MtbStr str);
public MtbStr mtb_str_cat(MtbArena *arena, MtbStr a, MtbStr b);
public MtbStr mtb_str_substr(MtbStr str, u64 start, u64 end);
#define mtb_str_is_empty(s) ((bool)((s).length == 0))


/* String Builder */

typedef struct mtb_str_builder MtbStrBuilder;
struct mtb_str_builder
{
    MtbListNode list;
    union {
        MtbArena *arena;
        MtbStr *str;
    };
};

public void mtb_str_builder_init(MtbArena *arena, MtbStrBuilder *sb);
#define mtb_str_builder_clear(b) mtb_list_reset(&(b)->list)
#define mtb_str_builder_is_empty(b) mtb_list_is_empty(&(b)->list)

public void mtb_str_builder_append(MtbStrBuilder *sb, MtbStr *str);
public void mtb_str_builder_appendf(MtbStrBuilder *sb, char *fmt, ...);
public void mtb_str_builder_prepend(MtbStrBuilder *sb, MtbStr *str);
public void mtb_str_builder_prependf(MtbStrBuilder *sb, char *fmt, ...);
public MtbStr mtb_str_join(MtbStrBuilder *sb, MtbStr delim);
#define mtb_str_join_char(sb, c) mtb_str_join(sb, mtb_str_char(c))

#endif // MTB_STRING_H
