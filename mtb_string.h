#ifndef MTB_STRING_H
#define MTB_STRING_H

#include <stdarg.h>

#include "mtb_macro.h"
#include "mtb_type.h"
#include "mtb_list.h"
#include "mtb_arena.h"


/* String */

typedef struct mtb_str MtbStr;
struct mtb_str
{
    union {
        u8 *bytes;
        char *chars; // not null-terminated!
    };
    u64 length; // in bytes
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

public i32 _mtb_str_cmp(MtbStr a, MtbStr b, bool insensitive);
#define mtb_str_cmp(a, b) _mtb_str_cmp(a, b, false)
#define mtb_str_cmp_i(a, b) _mtb_str_cmp(a, b, true)
#define mtb_str_is_equal(a, b) (mtb_str_cmp(a, b) == 0)
#define mtb_str_is_equal_i(a, b) (mtb_str_cmp_i(a, b) == 0)

#define MTB_STR_NOT_FOUND U64_MAX
public u64 _mtb_str_find(MtbStr a, MtbStr b, bool insensitive);
#define mtb_str_find(a, b) _mtb_str_find(a, b, false)
#define mtb_str_find_i(a, b) _mtb_str_find(a, b, true)
#define mtb_str_contains(a, b) ((bool)(mtb_str_find(a, b, false) != MTB_STR_NOT_FOUND))
#define mtb_str_contains_i(a, b) ((bool)(mtb_str_find(a, b, true) != MTB_STR_NOT_FOUND))

public MtbStr mtb_str_trim_start(MtbStr s, char *chars);
#define mtb_str_trim_start_space(s) mtb_str_trim_start(s, " \t\r\n\v\f")
public MtbStr mtb_str_trim_end(MtbStr s, char *chars);
#define mtb_str_trim_end_space(s) mtb_str_trim_end(s, " \t\r\n\v\f")
public MtbStr mtb_str_trim(MtbStr s, char *chars);
#define mtb_str_trim_space(s) mtb_str_trim(s, " \t\r\n\v\f")

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
