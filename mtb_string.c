#include <string.h>
#include <stdio.h>

#include "mtb_string.h"


public MtbStr
mtb_str_from_cstr(char *cstr)
{
    return mtb_str((u8 *)cstr, strlen(cstr));
}

public char *
mtb_str_to_cstr(MtbArena *arena, MtbStr str)
{
    char *cstr = mtb_arena_bump(arena, char, str.length + 1);
    memcpy(cstr, str.chars, str.length);
    return cstr;
}

public MtbStr
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

public MtbStr
mtb_str_sprintf(MtbArena *arena, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    MtbStr result = mtb_str_vsprintf(arena, fmt, args);

    va_end(args);
    return result;
}

public i32
_mtb_str_cmp(MtbStr a, MtbStr b, bool insensitive)
{
    // TODO intrinsify
    u64 minLength = mtb_min_u64(a.length, b.length);
    if (insensitive) {
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

public MtbStr
mtb_str_dup(MtbArena *arena, MtbStr str)
{
    if (str.length == 0) {
        return mtb_str_empty();
    }
    u8 *bytes = mtb_arena_bump(arena, u8, str.length);
    memcpy(bytes, str.bytes, str.length);
    return mtb_str(bytes, str.length);
}

public MtbStr
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

public MtbStr
mtb_str_substr(MtbStr str, u64 start, u64 end)
{
    mtb_assert_always(start < end && end <= str.length);
    return mtb_str(str.bytes + start, end - start);
}

public void
mtb_str_builder_init(MtbArena *arena, MtbStrBuilder *sb)
{
    sb->arena = arena;
    mtb_list_init(&sb->list);
}

public void
mtb_str_builder_append(MtbStrBuilder *sb, MtbStr *str)
{
    MtbStrBuilder *node = mtb_arena_bump(sb->arena, MtbStrBuilder, 1);
    node->str = str;
    mtb_list_add_last(&sb->list, &node->list);
}

public void
mtb_str_builder_appendf(MtbStrBuilder *sb, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    MtbStr *str = mtb_arena_bump(sb->arena, MtbStr, 1);
    *str = mtb_str_vsprintf(sb->arena, fmt, args);
    va_end(args);
    mtb_str_builder_append(sb, str);
}

public void
mtb_str_builder_prepend(MtbStrBuilder *sb, MtbStr *str)
{
    MtbStrBuilder *node = mtb_arena_bump(sb->arena, MtbStrBuilder, 1);
    node->str = str;
    mtb_list_add_first(&sb->list, &node->list);
}

public void
mtb_str_builder_prependf(MtbStrBuilder *sb, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    MtbStr *str = mtb_arena_bump(sb->arena, MtbStr, 1);
    *str = mtb_str_vsprintf(sb->arena, fmt, args);
    va_end(args);
    mtb_str_builder_prepend(sb, str);
}

public MtbStr
mtb_str_join(MtbStrBuilder *sb, MtbStr delim)
{
    u64 length = 0;
    mtb_list_foreach(&sb->list, l) {
        MtbStrBuilder *node = mtb_containerof(l, MtbStrBuilder, list);
        length = node->str->length + length + delim.length;
    }
    if (length == 0) {
        return mtb_str_empty();
    }
    length -= delim.length;

    u8 *bytes = mtb_arena_bump(sb->arena, u8, length);
    u64 bytesOffset = 0;
    mtb_list_foreach(&sb->list, l) {
        MtbStrBuilder *node = mtb_containerof(l, MtbStrBuilder, list);
        memcpy(bytes + bytesOffset, node->str->bytes, node->str->length);
        bytesOffset += node->str->length;
        if (bytesOffset < length) {
            memcpy(bytes + bytesOffset, delim.bytes, delim.length);
            bytesOffset += delim.length;
        }
    }
    return mtb_str(bytes, length);
}


#ifdef MTB_STRING_TESTS

#include <assert.h>


intern void
test_mtb_str_cstr(MtbArena arena)
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

intern void
test_mtb_str_sprintf(MtbArena arena)
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

intern void
test_mtb_str_cmp(void)
{
    assert(mtb_str_is_equal(mtb_str_empty(), mtb_str_empty()));
    assert(mtb_str_cmp(mtb_str_lit("abcd"), mtb_str_empty()) > 0);
    assert(mtb_str_cmp(mtb_str_empty(), mtb_str_lit("abcd")) < 0);
    assert(mtb_str_is_equal(mtb_str_lit("abcd"), mtb_str_lit("abcd")));
    assert(mtb_str_cmp(mtb_str_lit("abcd"), mtb_str_lit("abcD")) > 0);
    assert(mtb_str_cmp(mtb_str_lit("abcD"), mtb_str_lit("abcd")) < 0);
    assert(mtb_str_cmp(mtb_str_lit("abcde"), mtb_str_lit("abcd")) > 0);
    assert(mtb_str_cmp(mtb_str_lit("abcd"), mtb_str_lit("abcde")) < 0);

    assert(mtb_str_is_equal_i(mtb_str_empty(), mtb_str_empty()));
    assert(mtb_str_cmp_i(mtb_str_lit("AbCd"), mtb_str_empty()) > 0);
    assert(mtb_str_cmp_i(mtb_str_empty(), mtb_str_lit("aBcD")) < 0);
    assert(mtb_str_is_equal_i(mtb_str_lit("AbCd"), mtb_str_lit("aBcD")));
    assert(mtb_str_cmp_i(mtb_str_lit("AbCe"), mtb_str_lit("aBcD")) > 0);
    assert(mtb_str_cmp_i(mtb_str_lit("AbCd"), mtb_str_lit("aBcE")) < 0);
    assert(mtb_str_cmp_i(mtb_str_lit("AbCdE"), mtb_str_lit("aBcD")) > 0);
    assert(mtb_str_cmp_i(mtb_str_lit("AbCd"), mtb_str_lit("aBcDe")) < 0);
}

intern void
test_mtb_str_dup(MtbArena arena)
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

intern void
test_mtb_str_cat(MtbArena arena)
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

intern void
test_mtb_str_substr(void)
{
    MtbStr s = mtb_str_lit("Hello, World!");
    assert(mtb_str_is_equal(mtb_str_substr(s, 0, s.length), mtb_str_lit("Hello, World!")));
    assert(mtb_str_is_equal(mtb_str_substr(s, 0, 5), mtb_str_lit("Hello")));
    assert(mtb_str_is_equal(mtb_str_substr(s, 7, s.length), mtb_str_lit("World!")));
}

intern void
test_mtb_str_builder(MtbArena arena)
{
    MtbStrBuilder sb = {0};
    mtb_str_builder_init(&arena, &sb);

    char *e1 = "";
    MtbStr a1 = mtb_str_join_char(&sb, ' ');
    assert(a1.length == strlen(e1));
    assert(strncmp(a1.chars, e1, a1.length) == 0);
    mtb_str_builder_clear(&sb);

    char *e2 = "HI";
    mtb_str_builder_prepend(&sb, &mtb_str_lit("HI"));
    MtbStr a2 = mtb_str_join_char(&sb, ' ');
    assert(a2.length == strlen(e2));
    assert(strncmp(a2.chars, e2, a2.length) == 0);
    mtb_str_builder_clear(&sb);

    char *e3 = "HELLO MY DEAR WORLD !";
    mtb_str_builder_prepend(&sb, &mtb_str_lit("DEAR"));
    mtb_str_builder_prepend(&sb, &mtb_str_lit("MY"));
    mtb_str_builder_prepend(&sb, &mtb_str_lit("HELLO"));
    mtb_str_builder_append(&sb, &mtb_str_lit("WORLD"));
    mtb_str_builder_append(&sb, &mtb_str_lit("!"));
    MtbStr a3 = mtb_str_join_char(&sb, ' ');
    assert(a3.length == strlen(e3));
    assert(strncmp(a3.chars, e3, a3.length) == 0);
    mtb_str_builder_clear(&sb);

    char *e4 = "HELLO!";
    mtb_str_builder_append(&sb, &mtb_str_lit("!"));
    mtb_str_builder_prepend(&sb, &mtb_str_lit("HELLO"));
    MtbStr a4 = mtb_str_join(&sb, mtb_str_empty());
    assert(a4.length == strlen(e4));
    assert(strncmp(a4.chars, e4, a4.length) == 0);
    mtb_str_builder_clear(&sb);

    char *e5 = "100! 0.12345?";
    mtb_str_builder_appendf(&sb, "%.5f?", 0.123451f);
    mtb_str_builder_prependf(&sb, "%d!", 100);
    MtbStr a5 = mtb_str_join_char(&sb, ' ');
    assert(a5.length == strlen(e5));
    assert(strncmp(a5.chars, e5, a5.length) == 0);
    mtb_str_builder_clear(&sb);

    assert(mtb_str_builder_is_empty(&sb));
}

intern void
test_mtb_string(void)
{
    MtbArena arena = {0};
    mtb_arena_init(&arena, kb(1), &MTB_ARENA_DEF_ALLOCATOR);

    test_mtb_str_cstr(arena);
    test_mtb_str_sprintf(arena);
    test_mtb_str_cmp();
    test_mtb_str_dup(arena);
    test_mtb_str_cat(arena);
    test_mtb_str_substr();
    test_mtb_str_builder(arena);

    mtb_arena_deinit(&arena);
}

#endif // MTB_STRING_TESTS