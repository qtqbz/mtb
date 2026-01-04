#ifndef MTB_STRING_H
#define MTB_STRING_H

#include <stdarg.h>


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

func MtbStr mtb_str_from_cstr(char *cstr);
func char *mtb_str_to_cstr(MtbArena *arena, MtbStr str);

func MtbStr mtb_str_vsprintf(MtbArena *arena, char *fmt, va_list args);
func MtbStr mtb_str_sprintf(MtbArena *arena, char *fmt, ...);

func i32 mtb_str_cmp_opt(MtbStr a, MtbStr b, MtbStrCmpOptions opt);
#define mtb_str_cmp(a, b, ...) mtb_str_cmp_opt(a, b, (MtbStrCmpOptions){ __VA_ARGS__ })
#define mtb_str_cmp_lit(a, b, ...) mtb_str_cmp(a, mtb_str_lit(b), __VA_ARGS__)
#define mtb_str_cmp_cstr(a, b, ...) mtb_str_cmp(a, mtb_str_from_cstr(b), __VA_ARGS__)
#define mtb_str_is_equal(a, b, ...) (mtb_str_cmp(a, b, __VA_ARGS__) == 0)
#define mtb_str_is_equal_lit(a, b, ...) mtb_str_is_equal(a, mtb_str_lit(b), __VA_ARGS__)
#define mtb_str_is_equal_cstr(a, b, ...) mtb_str_is_equal(a, mtb_str_from_cstr(b), __VA_ARGS__)

func u64 mtb_str_find_opt(MtbStr str, MtbStr pattern, MtbStrFindOptions opt);
#define mtb_str_find(str, pattern, ...) mtb_str_find_opt(str, pattern, (MtbStrFindOptions){ __VA_ARGS__ })
#define mtb_str_find_lit(str, pattern, ...) mtb_str_find(str, mtb_str_lit(pattern), __VA_ARGS__)
#define mtb_str_find_char(str, pattern, ...) mtb_str_find(str, mtb_str_char(pattern), __VA_ARGS__)
#define mtb_str_find_cstr(str, pattern, ...) mtb_str_find(str, mtb_str_from_cstr(pattern), __VA_ARGS__)
#define mtb_str_contains(str, pattern, ...) ((bool)(mtb_str_find(str, pattern, __VA_ARGS__) != MTB_STR_NOT_FOUND))
#define mtb_str_contains_lit(str, pattern, ...) mtb_str_contains(str, mtb_str_lit(pattern), __VA_ARGS__)
#define mtb_str_contains_char(str, pattern, ...) mtb_str_contains(str, mtb_str_char(pattern), __VA_ARGS__)
#define mtb_str_contains_cstr(str, pattern, ...) mtb_str_contains(str, mtb_str_from_cstr(pattern), __VA_ARGS__)

func bool mtb_str_has_prefix_opt(MtbStr str, MtbStr pfx, MtbStrFindOptions opt);
#define mtb_str_has_prefix(str, pfx, ...) mtb_str_has_prefix_opt(str, pfx, (MtbStrFindOptions){ __VA_ARGS__ })
#define mtb_str_has_prefix_lit(str, pfx, ...) mtb_str_has_prefix(str, mtb_str_lit(pfx), __VA_ARGS__)
#define mtb_str_has_prefix_cstr(str, pfx, ...) mtb_str_has_prefix(str, mtb_str_from_cstr(pfx), __VA_ARGS__)

func bool mtb_str_has_suffix_opt(MtbStr str, MtbStr sfx, MtbStrFindOptions opt);
#define mtb_str_has_suffix(str, sfx, ...) mtb_str_has_suffix_opt(str, sfx, (MtbStrFindOptions){ __VA_ARGS__ })
#define mtb_str_has_suffix_lit(str, sfx, ...) mtb_str_has_suffix(str, mtb_str_lit(sfx), __VA_ARGS__)
#define mtb_str_has_suffix_cstr(str, sfx, ...) mtb_str_has_suffix(str, mtb_str_from_cstr(sfx), __VA_ARGS__)

func MtbStr mtb_str_trim(MtbStr str);
func MtbStr mtb_str_trim_beg(MtbStr str);
func MtbStr mtb_str_trim_end(MtbStr str);
func MtbStr mtb_str_skip(MtbStr str, u64 length);
func MtbStr mtb_str_chop(MtbStr str, u64 length);
func MtbStr mtb_str_to_lower(MtbArena *arena, MtbStr str);
func MtbStr mtb_str_to_upper(MtbArena *arena, MtbStr str);
func MtbStr mtb_str_dup(MtbArena *arena, MtbStr str);
func MtbStr mtb_str_cat(MtbArena *arena, MtbStr a, MtbStr b);
func MtbStr mtb_str_substr(MtbStr str, u64 beg, u64 end);
func MtbStr mtb_str_prefix(MtbStr str, u64 length);
func MtbStr mtb_str_suffix(MtbStr str, u64 length);
func bool mtb_str_is_empty(MtbStr str);


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

func void mtb_str_list_init(MtbArena *arena, MtbStrList *list);
func void mtb_str_list_clear(MtbStrList *list);
func bool mtb_str_list_is_empty(MtbStrList *list);

func MtbStrList *mtb_str_list_append(MtbStrList *list, MtbStr *str);
#define mtb_str_list_append_lit(list, str) mtb_str_list_append(list, &mtb_str_lit(str))
func MtbStrList *mtb_str_list_appendf(MtbStrList *list, char *fmt, ...);

func MtbStrList *mtb_str_list_prepend(MtbStrList *list, MtbStr *str);
#define mtb_str_list_prepend_lit(list, str) mtb_str_list_prepend(list, &mtb_str_lit(str))
func MtbStrList *mtb_str_list_prependf(MtbStrList *list, char *fmt, ...);

func MtbStr mtb_str_join_opt(MtbStrList *list, MtbStr delim, MtbStrJoinOptions opt);
#define mtb_str_join(list, delim, ...) mtb_str_join_opt(list, delim, (MtbStrJoinOptions){ __VA_ARGS__ })
#define mtb_str_join_lit(list, delim, ...) mtb_str_join(list, mtb_str_lit(delim), __VA_ARGS__)
#define mtb_str_join_char(list, delim, ...) mtb_str_join(list, mtb_str_char(delim), __VA_ARGS__)

func void mtb_str_split_opt(MtbStrList *list, MtbStr str, MtbStr delim, MtbStrSplitOptions opt);
#define mtb_str_split(list, str, delim, ...) mtb_str_split_opt(list, str, delim, (MtbStrSplitOptions){ __VA_ARGS__ })
#define mtb_str_split_lit(list, str, delim, ...) mtb_str_split(list, str, mtb_str_lit(delim), __VA_ARGS__)
#define mtb_str_split_char(list, str, delim, ...) mtb_str_split(list, str, mtb_str_char(delim), __VA_ARGS__)


/* Hash map utils */

func u64
mtb_str_key_hash(void *key)
{
    MtbStr *s = (MtbStr *)key;
    u64 hash = 0;
    for (u64 i = 0; i < s->length; i++) {
        hash = 17000069 * hash + s->chars[i];
    }
    return hash;
}

func bool
mtb_str_key_equals(void *key1, void *key2)
{
    MtbStr *s1 = (MtbStr *)key1;
    MtbStr *s2 = (MtbStr *)key2;
    return mtb_str_is_equal(*s1, *s2);
}

#endif // MTB_STRING_H


#ifdef MTB_STRING_IMPLEMENTATION

#include <string.h>
#include <stdio.h>


func MtbStr
mtb_str_from_cstr(char *cstr)
{
    return mtb_str((u8 *)cstr, strlen(cstr));
}

func char *
mtb_str_to_cstr(MtbArena *arena, MtbStr str)
{
    char *cstr = mtb_arena_bump(arena, char, str.length + 1);
    memcpy(cstr, str.chars, str.length);
    return cstr;
}

func MtbStr
mtb_str_vsprintf(MtbArena *arena, char *fmt, va_list args)
{
    va_list argsTmp;
    va_copy(argsTmp, args);
    u64 length = vsnprintf(nil, 0, fmt, argsTmp);
    va_end(argsTmp);

    if (length == 0) {
        return mtb_str_empty();
    }

    char *chars = mtb_arena_bump(arena, char, length);
    vsprintf(chars, fmt, args);

    return mtb_str((u8 *)chars, length);
}

func MtbStr
mtb_str_sprintf(MtbArena *arena, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    MtbStr result = mtb_str_vsprintf(arena, fmt, args);

    va_end(args);
    return result;
}

func i32
mtb_str_cmp_opt(MtbStr a, MtbStr b, MtbStrCmpOptions opt)
{
    // TODO intrinsify
    u64 minLength = mtb_min_u64(a.length, b.length);
    if (opt.ignoreCase) {
        for (u64 i = 0; i < minLength; i++) {
            char ca = mtb_char_to_lower(a.chars[i]);
            char cb = mtb_char_to_lower(b.chars[i]);
            if (ca != cb) {
                return ca > cb ? 1 : -1;
            }
        }
    }
    else {
        i32 result = strncmp(a.chars, b.chars, minLength);
        if (result != 0) {
            return result;
        }
    }
    return a.length == b.length ? 0 : a.length > b.length ? 1 : -1;
}

func u64
mtb_str_find_opt(MtbStr str, MtbStr pattern, MtbStrFindOptions opt)
{
    if (str.length >= pattern.length) {
        u64 i, iEnd = str.length - pattern.length + 1;
        u64 j, jEnd = pattern.length;
        if (opt.ignoreCase) {
            for (i = 0; i < iEnd; i++) {
                for (j = 0; j < jEnd; j++) {
                    if (mtb_char_to_lower(str.chars[i + j]) != mtb_char_to_lower(pattern.chars[j])) {
                        break;
                    }
                }
                if (j == jEnd) {
                    return i;
                }
            }
        }
        else {
            for (i = 0; i < iEnd; i++) {
                for (j = 0; j < jEnd; j++) {
                    if (str.chars[i + j] != pattern.chars[j]) {
                        break;
                    }
                }
                if (j == jEnd) {
                    return i;
                }
            }
        }
    }
    return MTB_STR_NOT_FOUND;
}

func bool
mtb_str_has_prefix_opt(MtbStr str, MtbStr pfx, MtbStrFindOptions opt)
{
    if (str.length < pfx.length) {
        return false;
    }
    MtbStr substr = mtb_str_substr(str, 0, pfx.length);
    return mtb_str_is_equal(pfx, substr, .ignoreCase = opt.ignoreCase);
}

func bool
mtb_str_has_suffix_opt(MtbStr str, MtbStr sfx, MtbStrFindOptions opt)
{
    if (str.length < sfx.length) {
        return false;
    }
    MtbStr substr = mtb_str_substr(str, str.length - sfx.length, str.length);
    return mtb_str_is_equal(sfx, substr, .ignoreCase = opt.ignoreCase);
}

func MtbStr
mtb_str_trim(MtbStr str)
{
    u64 beg = 0;
    while (beg < str.length && mtb_char_is_space(str.chars[beg])) {
        beg++;
    }
    u64 end = str.length;
    while (beg < end && mtb_char_is_space(str.chars[end - 1])) {
        end--;
    }
    str.chars += beg;
    str.length = end - beg;
    return str;
}

func MtbStr
mtb_str_trim_beg(MtbStr str)
{
    u64 beg = 0;
    while (beg < str.length && mtb_char_is_space(str.chars[beg])) {
        beg++;
    }
    str.chars += beg;
    str.length -= beg;
    return str;
}

func MtbStr
mtb_str_trim_end(MtbStr str)
{
    u64 end = str.length;
    while (end > 0 && mtb_char_is_space(str.chars[end - 1])) {
        end--;
    }
    str.length = end;
    return str;
}

func MtbStr
mtb_str_skip(MtbStr str, u64 length)
{
    mtb_assert_always(str.length >= length);
    str.chars += length;
    str.length -= length;
    return str;
}

func MtbStr
mtb_str_chop(MtbStr str, u64 length)
{
    mtb_assert_always(str.length >= length);
    str.length -= length;
    return str;
}

func MtbStr
mtb_str_to_lower(MtbArena *arena, MtbStr str)
{
    if (mtb_str_is_empty(str)) {
        return str;
    }
    char *chars = mtb_arena_bump(arena, char, str.length);
    for (u64 i = 0; i < str.length; i++) {
        chars[i] = mtb_char_to_lower(str.chars[i]);
    }
    str.chars = chars;
    return str;
}

func MtbStr
mtb_str_to_upper(MtbArena *arena, MtbStr str)
{
    if (mtb_str_is_empty(str)) {
        return str;
    }
    char *chars = mtb_arena_bump(arena, char, str.length);
    for (u64 i = 0; i < str.length; i++) {
        chars[i] = mtb_char_to_upper(str.chars[i]);
    }
    str.chars = chars;
    return str;
}

func MtbStr
mtb_str_dup(MtbArena *arena, MtbStr str)
{
    if (mtb_str_is_empty(str)) {
        return str;
    }
    u8 *bytes = mtb_arena_bump(arena, u8, str.length);
    memcpy(bytes, str.bytes, str.length);
    str.bytes = bytes;
    return str;
}

func MtbStr
mtb_str_cat(MtbArena *arena, MtbStr a, MtbStr b)
{
    u64 length = mtb_add_u64(a.length, b.length);
    if (length == 0) {
        return mtb_str_empty();
    }
    u8 *bytes = mtb_arena_bump(arena, u8, length);
    memcpy(bytes, a.bytes, a.length);
    memcpy(bytes + a.length, b.bytes, b.length);
    return mtb_str(bytes, length);
}

func MtbStr
mtb_str_substr(MtbStr str, u64 beg, u64 end)
{
    mtb_assert_always(beg <= end && end <= str.length);
    str.chars += beg;
    str.length = end - beg;
    return str;
}

func MtbStr
mtb_str_prefix(MtbStr str, u64 length)
{
    mtb_assert_always(str.length >= length);
    str.length = length;
    return str;
}

func MtbStr mtb_str_suffix(MtbStr str, u64 length)
{
    mtb_assert_always(str.length >= length);
    str.chars += str.length - length;
    str.length = length;
    return str;
}

func bool
mtb_str_is_empty(MtbStr str)
{
    return str.length == 0;
}

func void
mtb_str_list_init(MtbArena *arena, MtbStrList *list)
{
    list->arena = arena;
    mtb_list_init(&list->node);
}

func void
mtb_str_list_clear(MtbStrList *list)
{
    mtb_list_reset(&list->node);
}

func bool
mtb_str_list_is_empty(MtbStrList *list)
{
    return mtb_list_is_empty(&list->node);
}

func MtbStrList *
mtb_str_list_append(MtbStrList *list, MtbStr *str)
{
    MtbStrList *node = mtb_arena_bump(list->arena, MtbStrList, 1);
    node->str = str;
    mtb_list_add_last(&list->node, &node->node);
    return node;
}

func MtbStrList *
mtb_str_list_appendf(MtbStrList *list, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    MtbStr *str = mtb_arena_bump(list->arena, MtbStr, 1);
    *str = mtb_str_vsprintf(list->arena, fmt, args);
    va_end(args);
    return mtb_str_list_append(list, str);
}

func MtbStrList *
mtb_str_list_prepend(MtbStrList *list, MtbStr *str)
{
    MtbStrList *node = mtb_arena_bump(list->arena, MtbStrList, 1);
    node->str = str;
    mtb_list_add_first(&list->node, &node->node);
    return node;
}

func MtbStrList *
mtb_str_list_prependf(MtbStrList *list, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    MtbStr *str = mtb_arena_bump(list->arena, MtbStr, 1);
    *str = mtb_str_vsprintf(list->arena, fmt, args);
    va_end(args);
    return mtb_str_list_prepend(list, str);
}

func MtbStr
mtb_str_join_opt(MtbStrList *list, MtbStr delim, MtbStrJoinOptions opt)
{
    u64 length = 0;
    mtb_list_foreach(&list->node, n) {
        MtbStrList *node = mtb_containerof(n, MtbStrList, node);
        MtbStr *str = node->str;
        if (!mtb_str_is_empty(*str) || !opt.skipEmpty) {
            length += str->length + delim.length;
        }
    }
    if (length == 0) {
        return mtb_str_empty();
    }
    length -= delim.length;

    u8 *bytes = mtb_arena_bump(list->arena, u8, length);
    u64 bytesOffset = 0;
    mtb_list_foreach(&list->node, n) {
        MtbStrList *node = mtb_containerof(n, MtbStrList, node);
        MtbStr *str = node->str;
        if (!mtb_str_is_empty(*str) || !opt.skipEmpty) {
            memcpy(bytes + bytesOffset, str->bytes, str->length);
            bytesOffset += str->length;
            if (bytesOffset < length) {
                memcpy(bytes + bytesOffset, delim.bytes, delim.length);
                bytesOffset += delim.length;
            }
        }
    }
    return mtb_str(bytes, length);
}

func void
mtb_str_split_opt(MtbStrList *list, MtbStr str, MtbStr delim, MtbStrSplitOptions opt)
{
    MtbArena *arena = list->arena;

    if (mtb_str_is_empty(delim)) {
        MtbStr *tok = mtb_arena_bump(arena, MtbStr, 1);
        *tok = mtb_str_prefix(str, str.length);
        mtb_str_list_append(list, tok);
    }
    else {
        while (!mtb_str_is_empty(str)) {
            u64 i = mtb_str_find(str, delim);
            MtbStr tok = mtb_str_prefix(str, mtb_min_u64(i, str.length));
            if (!mtb_str_is_empty(tok) || !opt.skipEmpty) {
                MtbStrList *node = mtb_str_list_append(list, mtb_arena_bump(arena, MtbStr, 1));
                *node->str = tok;

                str.length -= tok.length;
                str.bytes += tok.length;
            }
            if (i != MTB_STR_NOT_FOUND) {
                str.length -= delim.length;
                str.bytes += delim.length;
            }
        }
    }
}

#endif // MTB_STRING_IMPLEMENTATION


#ifdef MTB_STRING_TESTS

#include <assert.h>


func void
_test_mtb_str_cstr(MtbArena arena)
{
    char *c1 = "My C String!";
    MtbStr s1 = mtb_str_from_cstr(c1);
    assert(strlen(c1) == s1.length);
    assert(strncmp(c1, s1.chars, s1.length) == 0);

    MtbStr s2 = mtb_str_lit("My non-C string!");
    char *c2 = mtb_str_to_cstr(&arena, s2);
    assert(strlen(c1) == s1.length);
    assert(strncmp(c2, s2.chars, s2.length) == 0);
}

func void
_test_mtb_str_sprintf(MtbArena arena)
{
    MtbStr e1 = mtb_str_empty();
    MtbStr a1 = mtb_str_sprintf(&arena, "");
    assert(e1.length == a1.length);
    assert(mtb_str_is_equal(e1, a1));

    MtbStr e2 = mtb_str_from_cstr("123\n0.4567\nAbCdEfGh");
    MtbStr a2 = mtb_str_sprintf(&arena, "%d\n%.4f\n%s", 123, 0.4567, "AbCdEfGh");
    assert(e2.length == a2.length);
    assert(mtb_str_is_equal(e2, a2));
}

func void
_test_mtb_str_cmp(void)
{
    assert(mtb_str_is_equal(mtb_str_empty(), mtb_str_empty()));
    assert(mtb_str_cmp(mtb_str_lit("abcd"), mtb_str_empty()) > 0);
    assert(mtb_str_cmp(mtb_str_empty(), mtb_str_lit("abcd")) < 0);
    assert(mtb_str_is_equal(mtb_str_lit("abcd"), mtb_str_lit("abcd")));
    assert(mtb_str_cmp(mtb_str_lit("abcd"), mtb_str_lit("abcD")) > 0);
    assert(mtb_str_cmp(mtb_str_lit("abcD"), mtb_str_lit("abcd")) < 0);
    assert(mtb_str_cmp(mtb_str_lit("abcde"), mtb_str_lit("abcd")) > 0);
    assert(mtb_str_cmp(mtb_str_lit("abcd"), mtb_str_lit("abcde")) < 0);

    assert(mtb_str_is_equal(mtb_str_empty(), mtb_str_empty(), .ignoreCase = true));
    assert(mtb_str_cmp(mtb_str_lit("AbCd"), mtb_str_empty(), .ignoreCase = true) > 0);
    assert(mtb_str_cmp(mtb_str_empty(), mtb_str_lit("aBcD"), .ignoreCase = true) < 0);
    assert(mtb_str_is_equal(mtb_str_lit("AbCd"), mtb_str_lit("aBcD"), .ignoreCase = true));
    assert(mtb_str_cmp(mtb_str_lit("AbCe"), mtb_str_lit("aBcD"), .ignoreCase = true) > 0);
    assert(mtb_str_cmp(mtb_str_lit("AbCd"), mtb_str_lit("aBcE"), .ignoreCase = true) < 0);
    assert(mtb_str_cmp(mtb_str_lit("AbCdE"), mtb_str_lit("aBcD"), .ignoreCase = true) > 0);
    assert(mtb_str_cmp(mtb_str_lit("AbCd"), mtb_str_lit("aBcDe"), .ignoreCase = true) < 0);
}

func void
_test_mtb_str_find(void)
{
    assert(mtb_str_find(mtb_str_lit("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), mtb_str_empty()) == 0);
    assert(mtb_str_find(mtb_str_empty(), mtb_str_lit("ABCDEFGHIJKLMNOPQRSTUVWXYZ")) == MTB_STR_NOT_FOUND);
    assert(mtb_str_find(mtb_str_lit("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), mtb_str_lit("ABCDEFGHIJKLMNOPQRSTUVWXYZ")) == 0);
    assert(mtb_str_find(mtb_str_lit("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), mtb_str_lit("A")) == 0);
    assert(mtb_str_find(mtb_str_lit("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), mtb_str_lit("MNOPQRS")) == 12);
    assert(mtb_str_find(mtb_str_lit("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), mtb_str_lit("Z")) == 25);
    assert(mtb_str_find(mtb_str_lit("ABCDEFGHIJKLMNOPQRSTUVWXYZ"), mtb_str_lit("z")) == MTB_STR_NOT_FOUND);
    assert(mtb_str_contains(mtb_str_lit("It's a Me, Mario!"), mtb_str_lit("Mario")));
    assert(!mtb_str_contains(mtb_str_lit("It's a Me, Mario!"), mtb_str_lit("Wario")));

    assert(mtb_str_find(mtb_str_lit("AbCdEfGhIjKlMnOpQrStUvWxYz"), mtb_str_empty(), .ignoreCase = true) == 0);
    assert(mtb_str_find(mtb_str_empty(), mtb_str_lit("AbCdEfGhIjKlMnOpQrStUvWxYz"), .ignoreCase = true) == MTB_STR_NOT_FOUND);
    assert(mtb_str_find(mtb_str_lit("AbCdEfGhIjKlMnOpQrStUvWxYz"), mtb_str_lit("aBcDeFgHiJkLmNoPqRsTuVwXyZ"), .ignoreCase = true) == 0);
    assert(mtb_str_find(mtb_str_lit("AbCdEfGhIjKlMnOpQrStUvWxYz"), mtb_str_lit("a"), .ignoreCase = true) == 0);
    assert(mtb_str_find(mtb_str_lit("AbCdEfGhIjKlMnOpQrStUvWxYz"), mtb_str_lit("mNoPqRs"), .ignoreCase = true) == 12);
    assert(mtb_str_find(mtb_str_lit("AbCdEfGhIjKlMnOpQrStUvWxYz"), mtb_str_lit("Z"), .ignoreCase = true) == 25);
    assert(mtb_str_find(mtb_str_lit("AbCdEfGhIjKlMnOpQrStUvWxYz"), mtb_str_lit("1"), .ignoreCase = true) == MTB_STR_NOT_FOUND);
    assert(mtb_str_contains(mtb_str_lit("It's a Me, Mario!"), mtb_str_lit("mario"), .ignoreCase = true));
    assert(!mtb_str_contains(mtb_str_lit("It's a Me, Mario!"), mtb_str_lit("wario"), .ignoreCase = true));
}

func void
_test_mtb_str_has_prefix(void)
{
    assert(mtb_str_has_prefix(mtb_str_lit("Hello, World!"), mtb_str_empty()));
    assert(!mtb_str_has_prefix(mtb_str_empty(), mtb_str_lit("Hello, World!")));
    assert(mtb_str_has_prefix(mtb_str_lit("Hello, World!"), mtb_str_lit("Hello")));
    assert(!mtb_str_has_prefix(mtb_str_lit("Hello, World!"), mtb_str_lit("World!")));
    assert(mtb_str_has_prefix(mtb_str_lit("Hello, World!"), mtb_str_lit("Hello, World!")));

    assert(mtb_str_has_prefix(mtb_str_lit("Hello, World!"), mtb_str_empty(), .ignoreCase = true));
    assert(!mtb_str_has_prefix(mtb_str_empty(), mtb_str_lit("Hello, World!"), .ignoreCase = true));
    assert(mtb_str_has_prefix(mtb_str_lit("Hello, World!"), mtb_str_lit("hello"), .ignoreCase = true));
    assert(!mtb_str_has_prefix(mtb_str_lit("Hello, World!"), mtb_str_lit("world!"), .ignoreCase = true));
    assert(mtb_str_has_prefix(mtb_str_lit("Hello, World!"), mtb_str_lit("hello, world!"), .ignoreCase = true));
}

func void
_test_mtb_str_has_suffix(void)
{
    assert(mtb_str_has_suffix(mtb_str_lit("Hello, World!"), mtb_str_empty()));
    assert(!mtb_str_has_suffix(mtb_str_empty(), mtb_str_lit("Hello, World!")));
    assert(mtb_str_has_suffix(mtb_str_lit("Hello, World!"), mtb_str_lit("World!")));
    assert(!mtb_str_has_suffix(mtb_str_lit("Hello, World!"), mtb_str_lit("Hello")));
    assert(mtb_str_has_suffix(mtb_str_lit("Hello, World!"), mtb_str_lit("Hello, World!")));

    assert(mtb_str_has_suffix(mtb_str_lit("Hello, World!"), mtb_str_empty(), .ignoreCase = true));
    assert(!mtb_str_has_suffix(mtb_str_empty(), mtb_str_lit("Hello, World!"), .ignoreCase = true));
    assert(mtb_str_has_suffix(mtb_str_lit("Hello, World!"), mtb_str_lit("world!"), .ignoreCase = true));
    assert(!mtb_str_has_suffix(mtb_str_lit("Hello, World!"), mtb_str_lit("hello"), .ignoreCase = true));
    assert(mtb_str_has_suffix(mtb_str_lit("Hello, World!"), mtb_str_lit("hello, world!"), .ignoreCase = true));
}

func void
_test_mtb_str_trim(void)
{
    assert(mtb_str_is_equal(mtb_str_trim(mtb_str_empty()), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_trim(mtb_str_lit(" \t\r\n\v\f")), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_trim(mtb_str_lit("Hello! \t\r\n\v\f")), mtb_str_lit("Hello!")));
    assert(mtb_str_is_equal(mtb_str_trim(mtb_str_lit(" \t\r\n\v\fHello!")), mtb_str_lit("Hello!")));
    assert(mtb_str_is_equal(mtb_str_trim(mtb_str_lit(" \t\r\n\v\fHello! \t\r\n\v\f")), mtb_str_lit("Hello!")));
}

func void
_test_mtb_str_trim_beg(void)
{
    assert(mtb_str_is_equal(mtb_str_trim_beg(mtb_str_empty()), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_trim_beg(mtb_str_lit(" \t\r\n\v\f")), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_trim_beg(mtb_str_lit("Hello! \t\r\n\v\f")), mtb_str_lit("Hello! \t\r\n\v\f")));
    assert(mtb_str_is_equal(mtb_str_trim_beg(mtb_str_lit(" \t\r\n\v\fHello!")), mtb_str_lit("Hello!")));
    assert(mtb_str_is_equal(mtb_str_trim_beg(mtb_str_lit(" \t\r\n\v\fHello! \t\r\n\v\f")), mtb_str_lit("Hello! \t\r\n\v\f")));
}

func void
_test_mtb_str_trim_end(void)
{
    assert(mtb_str_is_equal(mtb_str_trim_end(mtb_str_empty()), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_trim_end(mtb_str_lit(" \t\r\n\v\f")), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_trim_end(mtb_str_lit("Hello! \t\r\n\v\f")), mtb_str_lit("Hello!")));
    assert(mtb_str_is_equal(mtb_str_trim_end(mtb_str_lit(" \t\r\n\v\fHello!")), mtb_str_lit(" \t\r\n\v\fHello!")));
    assert(mtb_str_is_equal(mtb_str_trim_end(mtb_str_lit(" \t\r\n\v\fHello! \t\r\n\v\f")), mtb_str_lit(" \t\r\n\v\fHello!")));
}

func void
_test_mtb_str_skip()
{
    assert(mtb_str_is_equal(mtb_str_skip(mtb_str_empty(), 0), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_skip(mtb_str_lit("Hello, World!"), 0), mtb_str_lit("Hello, World!")));
    assert(mtb_str_is_equal(mtb_str_skip(mtb_str_lit("Hello, World!"), 7), mtb_str_lit("World!")));
    assert(mtb_str_is_equal(mtb_str_skip(mtb_str_lit("Hello, World!"), 13), mtb_str_empty()));
}

func void
_test_mtb_str_chop()
{
    assert(mtb_str_is_equal(mtb_str_chop(mtb_str_empty(), 0), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_chop(mtb_str_lit("Hello, World!"), 0), mtb_str_lit("Hello, World!")));
    assert(mtb_str_is_equal(mtb_str_chop(mtb_str_lit("Hello, World!"), 8), mtb_str_lit("Hello")));
    assert(mtb_str_is_equal(mtb_str_chop(mtb_str_lit("Hello, World!"), 13), mtb_str_empty()));
}

func void
_test_mtb_str_to_lower(MtbArena arena)
{
    assert(mtb_str_is_equal(mtb_str_to_lower(&arena, mtb_str_empty()), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_to_lower(&arena, mtb_str_lit("Hello, World!")), mtb_str_lit("hello, world!")));
}

func void
_test_mtb_str_to_upper(MtbArena arena)
{
    assert(mtb_str_is_equal(mtb_str_to_upper(&arena, mtb_str_empty()), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_to_upper(&arena, mtb_str_lit("Hello, World!")), mtb_str_lit("HELLO, WORLD!")));
}

func void
_test_mtb_str_dup(MtbArena arena)
{
    MtbStr o1 = mtb_str_empty();
    MtbStr a1 = mtb_str_dup(&arena, o1);
    assert(mtb_str_is_empty(a1));
    assert(mtb_str_is_equal(o1, a1));

    MtbStr o2 = mtb_str_lit("Duplicate Me!");
    MtbStr a2 = mtb_str_dup(&arena, o2);
    assert(o2.bytes != a2.bytes);
    assert(mtb_str_is_equal(o2, a2));
}

func void
_test_mtb_str_cat(MtbArena arena)
{
    MtbStr e1 = mtb_str_empty();
    MtbStr a1 = mtb_str_cat(&arena, mtb_str_empty(), mtb_str_empty());
    assert(mtb_str_is_equal(e1, a1));

    MtbStr e2 = mtb_str_lit("abcd");
    MtbStr a2 = mtb_str_cat(&arena, mtb_str_lit("abcd"), mtb_str_empty());
    assert(mtb_str_is_equal(e2, a2));

    MtbStr e3 = mtb_str_lit("abcd");
    MtbStr a3 = mtb_str_cat(&arena, mtb_str_empty(), mtb_str_lit("abcd"));
    assert(mtb_str_is_equal(e3, a3));

    MtbStr e4 = mtb_str_lit("abcd1234");
    MtbStr a4 = mtb_str_cat(&arena, mtb_str_lit("abcd"), mtb_str_lit("1234"));
    assert(mtb_str_is_equal(e4, a4));
}

func void
_test_mtb_str_substr(void)
{
    MtbStr s = mtb_str_lit("Hello, World!");
    assert(mtb_str_is_equal(mtb_str_substr(s, 0, s.length), mtb_str_lit("Hello, World!")));
    assert(mtb_str_is_equal(mtb_str_substr(s, 0, 5), mtb_str_lit("Hello")));
    assert(mtb_str_is_equal(mtb_str_substr(s, 7, s.length), mtb_str_lit("World!")));
    assert(mtb_str_is_equal(mtb_str_substr(s, 0, 0), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_substr(s, s.length, s.length), mtb_str_empty()));
}

func void
_test_mtb_str_prefix()
{
    assert(mtb_str_is_equal(mtb_str_prefix(mtb_str_empty(), 0), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_prefix(mtb_str_lit("Hello, World!"), 0), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_prefix(mtb_str_lit("Hello, World!"), 5), mtb_str_lit("Hello")));
    assert(mtb_str_is_equal(mtb_str_prefix(mtb_str_lit("Hello, World!"), 13), mtb_str_lit("Hello, World!")));
}

func void
_test_mtb_str_suffix()
{
    assert(mtb_str_is_equal(mtb_str_suffix(mtb_str_empty(), 0), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_suffix(mtb_str_lit("Hello, World!"), 0), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_suffix(mtb_str_lit("Hello, World!"), 6), mtb_str_lit("World!")));
    assert(mtb_str_is_equal(mtb_str_suffix(mtb_str_lit("Hello, World!"), 13), mtb_str_lit("Hello, World!")));
}

func void
_test_mtb_str_join(MtbArena arena)
{
    MtbStrList list = {0};
    mtb_str_list_init(&arena, &list);

    char *e1 = "";
    MtbStr a1 = mtb_str_join_char(&list, ' ');
    assert(a1.length == strlen(e1));
    assert(strncmp(a1.chars, e1, a1.length) == 0);
    mtb_str_list_clear(&list);

    char *e2 = "HI";
    mtb_str_list_prepend_lit(&list, "HI");
    MtbStr a2 = mtb_str_join_char(&list, ' ');
    assert(a2.length == strlen(e2));
    assert(strncmp(a2.chars, e2, a2.length) == 0);
    mtb_str_list_clear(&list);

    char *e3 = "HELLO MY DEAR WORLD !";
    mtb_str_list_prepend_lit(&list, "DEAR");
    mtb_str_list_prepend_lit(&list, "MY");
    mtb_str_list_prepend_lit(&list, "HELLO");
    mtb_str_list_append_lit(&list, "WORLD");
    mtb_str_list_append_lit(&list, "!");
    MtbStr a3 = mtb_str_join_char(&list, ' ');
    assert(a3.length == strlen(e3));
    assert(strncmp(a3.chars, e3, a3.length) == 0);
    mtb_str_list_clear(&list);

    char *e4 = "HELLO!";
    mtb_str_list_append_lit(&list, "!");
    mtb_str_list_prepend_lit(&list, "HELLO");
    MtbStr a4 = mtb_str_join_lit(&list, "");
    assert(a4.length == strlen(e4));
    assert(strncmp(a4.chars, e4, a4.length) == 0);
    mtb_str_list_clear(&list);

    char *e5 = "100! 0.12345?";
    mtb_str_list_appendf(&list, "%.5f?", 0.123451f);
    mtb_str_list_prependf(&list, "%d!", 100);
    MtbStr a5 = mtb_str_join_char(&list, ' ');
    assert(a5.length == strlen(e5));
    assert(strncmp(a5.chars, e5, a5.length) == 0);
    mtb_str_list_clear(&list);

    char *e6 = ",,";
    mtb_str_list_append_lit(&list, "");
    mtb_str_list_append_lit(&list, "");
    mtb_str_list_append_lit(&list, "");
    MtbStr a6 = mtb_str_join_char(&list, ',');
    assert(a6.length == strlen(e6));
    assert(strncmp(a6.chars, e6, a6.length) == 0);
    mtb_str_list_clear(&list);

    char *e7 = "";
    mtb_str_list_append_lit(&list, "");
    mtb_str_list_append_lit(&list, "");
    mtb_str_list_append_lit(&list, "");
    MtbStr a7 = mtb_str_join_char(&list, ',', .skipEmpty = true);
    assert(a7.length == strlen(e7));
    assert(strncmp(a7.chars, e7, a7.length) == 0);
    mtb_str_list_clear(&list);

    assert(mtb_str_list_is_empty(&list));
}

func void
_test_mtb_str_split(MtbArena arena)
{
    MtbStrList list = {0};
    mtb_str_list_init(&arena, &list);

    MtbStr s1 = mtb_str_empty();
    mtb_str_split_char(&list, s1, ',');
    assert(mtb_str_list_is_empty(&list));
    mtb_str_list_clear(&list);

    MtbStr s2 = mtb_str_lit("A,B,C,D,E");
    MtbStr e2[] = { mtb_str_lit("A"), mtb_str_lit("B"), mtb_str_lit("C"), mtb_str_lit("D"), mtb_str_lit("E") };
    mtb_str_split_char(&list, s2, ',');
    i32 i2 = 0;
    mtb_list_foreach(&list.node, n) {
        MtbStrList *node = mtb_containerof(n, MtbStrList, node);
        assert(mtb_str_is_equal(*node->str, e2[i2++]));
    }
    mtb_str_list_clear(&list);

    MtbStr s3 = mtb_str_lit("A,B,C,D,E");
    MtbStr e3[] = { mtb_str_lit("A,B,C,D,E") };
    mtb_str_split_lit(&list, s3, " ");
    i32 i3 = 0;
    mtb_list_foreach(&list.node, n) {
        MtbStrList *node = mtb_containerof(n, MtbStrList, node);
        assert(mtb_str_is_equal(*node->str, e3[i3++]));
    }
    mtb_str_list_clear(&list);

    MtbStr s4 = mtb_str_lit("A,B,C,D,E");
    MtbStr e4[] = { mtb_str_lit("A,B,C,D,E") };
    mtb_str_split(&list, s4, mtb_str_empty());
    i32 i4 = 0;
    mtb_list_foreach(&list.node, n) {
        MtbStrList *node = mtb_containerof(n, MtbStrList, node);
        assert(mtb_str_is_equal(*node->str, e4[i4++]));
    }
    mtb_str_list_clear(&list);

    MtbStr s5 = mtb_str_lit(",,");
    MtbStr e5[] = { mtb_str_empty(), mtb_str_empty(), mtb_str_empty() };
    mtb_str_split_char(&list, s5, ',');
    i32 i5 = 0;
    mtb_list_foreach(&list.node, n) {
        MtbStrList *node = mtb_containerof(n, MtbStrList, node);
        assert(mtb_str_is_equal(*node->str, e5[i5++]));
    }
    mtb_str_list_clear(&list);

    MtbStr s6 = mtb_str_lit(",,");
    mtb_str_split_char(&list, s6, ',', .skipEmpty = true);
    assert(mtb_str_list_is_empty(&list));
    mtb_str_list_clear(&list);

    assert(mtb_str_list_is_empty(&list));
}

func void
_test_mtb_string(void)
{
    MtbArena arena = {0};
    mtb_arena_init(&arena, kb(1), &MTB_ARENA_DEF_ALLOCATOR);

    _test_mtb_str_cstr(arena);
    _test_mtb_str_sprintf(arena);
    _test_mtb_str_cmp();
    _test_mtb_str_find();
    _test_mtb_str_has_prefix();
    _test_mtb_str_has_suffix();
    _test_mtb_str_trim();
    _test_mtb_str_skip();
    _test_mtb_str_chop();
    _test_mtb_str_to_lower(arena);
    _test_mtb_str_to_upper(arena);
    _test_mtb_str_dup(arena);
    _test_mtb_str_cat(arena);
    _test_mtb_str_substr();
    _test_mtb_str_prefix();
    _test_mtb_str_suffix();
    _test_mtb_str_join(arena);
    _test_mtb_str_split(arena);

    mtb_arena_deinit(&arena);
}

#endif // MTB_STRING_TESTS
