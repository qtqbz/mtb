#include "mtb_type.h"


public bool
mtb_char_is_lower(char c)
{
    return 'a' <= c && c <= 'z';
}

public bool
mtb_char_is_upper(char c)
{
    return 'A' <= c && c <= 'Z';
}

public bool
mtb_char_is_alpha(char c)
{
    return mtb_char_is_lower(c) || mtb_char_is_upper(c);
}

public bool
mtb_char_is_digit(char c)
{
    return '0' <= c && c <= '9';
}

public bool
mtb_char_is_xdigit(char c)
{
    return mtb_char_is_digit(c) || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f');
}

public bool
mtb_char_is_alnum(char c)
{
    return mtb_char_is_alpha(c) || mtb_char_is_digit(c);
}

public bool
mtb_char_is_space(char c)
{
    switch(c) {
        case ' ':
        case '\t':
        case '\r':
        case '\n':
        case '\v':
        case '\f': {
            return true;
        }
        default: {
            return false;
        }
    }
}

public char
mtb_char_to_lower(char c)
{
    return mtb_char_is_upper(c) ? c + ('a' - 'A') : c;
}

public char
mtb_char_to_upper(char c)
{
    return mtb_char_is_lower(c) ? c + ('A' - 'a') : c;
}


#ifdef MTB_TYPE_TESTS

#include <assert.h>


intern void
test_mtb_min(void)
{
    assert(mtb_min_i8(I8_MIN, I8_MAX) == I8_MIN);
    assert(mtb_min_i16(I16_MIN, I16_MAX) == I16_MIN);
    assert(mtb_min_i32(I32_MIN, I32_MAX) == I32_MIN);
    assert(mtb_min_i64(I64_MIN, I64_MAX) == I64_MIN);

    assert(mtb_min_u8(0, U8_MAX) == 0);
    assert(mtb_min_u16(0, U16_MAX) == 0);
    assert(mtb_min_u32(0, U32_MAX) == 0);
    assert(mtb_min_u64(0, U64_MAX) == 0);
}

intern void
test_mtb_max(void)
{
    assert(mtb_max_i8(I8_MIN, I8_MAX) == I8_MAX);
    assert(mtb_max_i16(I16_MIN, I16_MAX) == I16_MAX);
    assert(mtb_max_i32(I32_MIN, I32_MAX) == I32_MAX);
    assert(mtb_max_i64(I64_MIN, I64_MAX) == I64_MAX);

    assert(mtb_max_u8(0, U8_MAX) == U8_MAX);
    assert(mtb_max_u16(0, U16_MAX) == U16_MAX);
    assert(mtb_max_u32(0, U32_MAX) == U32_MAX);
    assert(mtb_max_u64(0, U64_MAX) == U64_MAX);
}

intern void
test_mtb_add(void)
{
    assert(mtb_add_i8(I8_MAX - 1, 1) == I8_MAX);
    assert(mtb_add_i16(I16_MAX - 1, 1) == I16_MAX);
    assert(mtb_add_i32(I32_MAX - 1, 1) == I32_MAX);
    assert(mtb_add_i64(I64_MAX - 1, 1) == I64_MAX);

    assert(mtb_add_u8(U8_MAX - 1, 1) == U8_MAX);
    assert(mtb_add_u16(U16_MAX - 1, 1) == U16_MAX);
    assert(mtb_add_u32(U32_MAX - 1, 1) == U32_MAX);
    assert(mtb_add_u64(U64_MAX - 1, 1) == U64_MAX);
}

intern void
test_mtb_sub(void)
{
    assert(mtb_sub_i8(I8_MIN + 1, 1) == I8_MIN);
    assert(mtb_sub_i16(I16_MIN + 1, 1) == I16_MIN);
    assert(mtb_sub_i32(I32_MIN + 1, 1) == I32_MIN);
    assert(mtb_sub_i64(I64_MIN + 1, 1) == I64_MIN);

    assert(mtb_sub_u8(0 + 1, 1) == 0);
    assert(mtb_sub_u16(0 + 1, 1) == 0);
    assert(mtb_sub_u32(0 + 1, 1) == 0);
    assert(mtb_sub_u64(0 + 1, 1) == 0);
}

intern void
test_mtb_mul(void)
{
    assert(mtb_mul_i8(1, I8_MAX) == I8_MAX);
    assert(mtb_mul_i16(1, I16_MAX) == I16_MAX);
    assert(mtb_mul_i32(1, I32_MAX) == I32_MAX);
    assert(mtb_mul_i64(1, I64_MAX) == I64_MAX);

    assert(mtb_mul_u8(1, U8_MAX) == U8_MAX);
    assert(mtb_mul_u16(1, U16_MAX) == U16_MAX);
    assert(mtb_mul_u32(1, U32_MAX) == U32_MAX);
    assert(mtb_mul_u64(1, U64_MAX) == U64_MAX);
}

intern void
test_mtb_cast(void)
{
    assert(mtb_cast_i16_i8(I8_MIN) == I8_MIN);
    assert(mtb_cast_i16_i8(I8_MAX) == I8_MAX);

    assert(mtb_cast_i32_i8(I8_MIN) == I8_MIN);
    assert(mtb_cast_i32_i8(I8_MAX) == I8_MAX);

    assert(mtb_cast_i32_i16(I16_MIN) == I16_MIN);
    assert(mtb_cast_i32_i16(I16_MAX) == I16_MAX);

    assert(mtb_cast_i64_i8(I8_MIN) == I8_MIN);
    assert(mtb_cast_i64_i8(I8_MAX) == I8_MAX);

    assert(mtb_cast_i64_i16(I16_MIN) == I16_MIN);
    assert(mtb_cast_i64_i16(I16_MAX) == I16_MAX);

    assert(mtb_cast_i64_i32(I32_MIN) == I32_MIN);
    assert(mtb_cast_i64_i32(I32_MAX) == I32_MAX);

    assert(mtb_cast_i8_u8(0) == 0);
    assert(mtb_cast_i8_u8(I8_MAX) == (u8)I8_MAX);

    assert(mtb_cast_i16_u16(0) == 0);
    assert(mtb_cast_i16_u16(I16_MAX) == (u16)I16_MAX);

    assert(mtb_cast_i32_u32(0) == 0);
    assert(mtb_cast_i32_u32(I32_MAX) == (u32)I32_MAX);

    assert(mtb_cast_i64_u64(0) == 0);
    assert(mtb_cast_i64_u64(I64_MAX) == (u64)I64_MAX);

    assert(mtb_cast_u16_u8(0) == 0);
    assert(mtb_cast_u16_u8(U8_MAX) == U8_MAX);

    assert(mtb_cast_u32_u8(0) == 0);
    assert(mtb_cast_u32_u8(U8_MAX) == U8_MAX);

    assert(mtb_cast_u32_u16(0) == 0);
    assert(mtb_cast_u32_u16(U16_MAX) == U16_MAX);

    assert(mtb_cast_u64_u8(0) == 0);
    assert(mtb_cast_u64_u8(U8_MAX) == U8_MAX);

    assert(mtb_cast_u64_u16(0) == 0);
    assert(mtb_cast_u64_u16(U16_MAX) == U16_MAX);

    assert(mtb_cast_u64_u32(0) == 0);
    assert(mtb_cast_u64_u32(U32_MAX) == U32_MAX);

    assert(mtb_cast_u8_i8(0) == 0);
    assert(mtb_cast_u8_i8(I8_MAX) == I8_MAX);

    assert(mtb_cast_u16_i16(0) == 0);
    assert(mtb_cast_u16_i16(I16_MAX) == I16_MAX);

    assert(mtb_cast_u32_i32(0) == 0);
    assert(mtb_cast_u32_i32(I32_MAX) == I32_MAX);

    assert(mtb_cast_u64_i64(0) == 0);
    assert(mtb_cast_u64_i64(I64_MAX) == I64_MAX);

    assert(mtb_cast_char_u8(0) == 0);
    assert(mtb_cast_char_u8(CHAR_MAX) == CHAR_MAX);

    assert(mtb_cast_u8_char(0) == 0);
    assert(mtb_cast_u8_char(CHAR_MAX) == CHAR_MAX);
}

intern void
test_mtb_type(void)
{
    test_mtb_min();
    test_mtb_max();
    test_mtb_add();
    test_mtb_sub();
    test_mtb_mul();
    test_mtb_cast();
}

#endif // MTB_TYPE_TESTS
