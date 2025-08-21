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

public u64
mtb_str_find_opt(MtbStr str, MtbStr pattern, MtbStrCmpOptions opt)
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

public bool
mtb_str_has_prefix_opt(MtbStr str, MtbStr pfx, MtbStrCmpOptions opt)
{
    if (str.length < pfx.length) {
        return false;
    }
    MtbStr substr = mtb_str_substr(str, 0, pfx.length);
    return mtb_str_is_equal(pfx, substr, .ignoreCase = opt.ignoreCase);
}

public bool
mtb_str_has_suffix_opt(MtbStr str, MtbStr sfx, MtbStrCmpOptions opt)
{
    if (str.length < sfx.length) {
        return false;
    }
    MtbStr substr = mtb_str_substr(str, str.length - sfx.length, str.length);
    return mtb_str_is_equal(sfx, substr, .ignoreCase = opt.ignoreCase);
}

public MtbStr
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
    return mtb_str_substr(str, beg, end);
}

public MtbStr
mtb_str_to_lower(MtbArena *arena, MtbStr str)
{
    if (mtb_str_is_empty(str)) {
        return mtb_str_empty();
    }
    char *chars = mtb_arena_bump(arena, char, str.length);
    for (u64 i = 0; i < str.length; i++) {
        chars[i] = mtb_char_to_lower(str.chars[i]);
    }
    return mtb_str((u8 *)chars, str.length);
}

public MtbStr
mtb_str_to_upper(MtbArena *arena, MtbStr str)
{
    if (mtb_str_is_empty(str)) {
        return mtb_str_empty();
    }
    char *chars = mtb_arena_bump(arena, char, str.length);
    for (u64 i = 0; i < str.length; i++) {
        chars[i] = mtb_char_to_upper(str.chars[i]);
    }
    return mtb_str((u8 *)chars, str.length);
}

public MtbStr
mtb_str_dup(MtbArena *arena, MtbStr str)
{
    if (mtb_str_is_empty(str)) {
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
    mtb_assert_always(start <= end && end <= str.length);
    return start == end ? mtb_str_empty() : mtb_str(str.bytes + start, end - start);
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

    assert(mtb_str_is_equal(mtb_str_empty(), mtb_str_empty(), .ignoreCase = true));
    assert(mtb_str_cmp(mtb_str_lit("AbCd"), mtb_str_empty(), .ignoreCase = true) > 0);
    assert(mtb_str_cmp(mtb_str_empty(), mtb_str_lit("aBcD"), .ignoreCase = true) < 0);
    assert(mtb_str_is_equal(mtb_str_lit("AbCd"), mtb_str_lit("aBcD"), .ignoreCase = true));
    assert(mtb_str_cmp(mtb_str_lit("AbCe"), mtb_str_lit("aBcD"), .ignoreCase = true) > 0);
    assert(mtb_str_cmp(mtb_str_lit("AbCd"), mtb_str_lit("aBcE"), .ignoreCase = true) < 0);
    assert(mtb_str_cmp(mtb_str_lit("AbCdE"), mtb_str_lit("aBcD"), .ignoreCase = true) > 0);
    assert(mtb_str_cmp(mtb_str_lit("AbCd"), mtb_str_lit("aBcDe"), .ignoreCase = true) < 0);
}

intern void
test_mtb_str_find(void)
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

intern void
test_mtb_str_has_prefix(void)
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

intern void
test_mtb_str_has_suffix(void)
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

intern void
test_mtb_str_trim(void)
{
    assert(mtb_str_is_equal(mtb_str_trim(mtb_str_empty()), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_trim(mtb_str_lit(" \t\r\n\v\f")), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_trim(mtb_str_lit("Hello! \t\r\n\v\f")), mtb_str_lit("Hello!")));
    assert(mtb_str_is_equal(mtb_str_trim(mtb_str_lit(" \t\r\n\v\fHello!")), mtb_str_lit("Hello!")));
    assert(mtb_str_is_equal(mtb_str_trim(mtb_str_lit(" \t\r\n\v\fHello! \t\r\n\v\f")), mtb_str_lit("Hello!")));
}

intern void
test_mtb_str_to_lower(MtbArena arena)
{
    assert(mtb_str_is_equal(mtb_str_to_lower(&arena, mtb_str_empty()), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_to_lower(&arena, mtb_str_lit("Hello, World!")), mtb_str_lit("hello, world!")));
}

intern void
test_mtb_str_to_upper(MtbArena arena)
{
    assert(mtb_str_is_equal(mtb_str_to_upper(&arena, mtb_str_empty()), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_to_upper(&arena, mtb_str_lit("Hello, World!")), mtb_str_lit("HELLO, WORLD!")));
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
    assert(mtb_str_is_equal(mtb_str_substr(s, 0, 0), mtb_str_empty()));
    assert(mtb_str_is_equal(mtb_str_substr(s, s.length, s.length), mtb_str_empty()));
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
    test_mtb_str_find();
    test_mtb_str_has_prefix();
    test_mtb_str_has_suffix();
    test_mtb_str_trim();
    test_mtb_str_to_lower(arena);
    test_mtb_str_to_upper(arena);
    test_mtb_str_dup(arena);
    test_mtb_str_cat(arena);
    test_mtb_str_substr();
    test_mtb_str_builder(arena);

    mtb_arena_deinit(&arena);
}

#endif // MTB_STRING_TESTS