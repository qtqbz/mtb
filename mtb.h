#ifndef MTB_H
#define MTB_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef MTB_MACRO_H
#define MTB_MACRO_H

#include <stddef.h>


/* Compiler Flags */

#if defined(__clang__)
#define _MTB_COMPILER_CLANG
#elif defined(__GNUC__)
#define _MTB_COMPILER_GCC
#else
#error "Unsupported copiler"
#endif


/* Aliases */

#define func static
#define global static __attribute__((unused))

#define nil ((void *)0)


/* Macro Helpers */

#define _mtb_stringify(s) #s
#define mtb_stringify(s) _mtb_stringify(s)

#define _mtb_cat(a, b) a##b
#define mtb_cat(a, b) _mtb_cat(a, b)

#define mtb_id(a)  mtb_cat(mtb_cat(a, __LINE__), _id_)


/* Array Helpers */

#define mtb_countof(a) (sizeof(a) / sizeof(*(a)))
#define mtb_lengthof(s) (mtb_countof(s) - 1)


/* Arith Helpers */

#define mtb_is_pow2_or_zero(s) ((bool)((((s) - 1) & (s)) == 0))
#define mtb_is_pow2(s) ((bool)((s) != 0 && mtb_is_pow2_or_zero((s))))

#define mtb_leading_zeros_count(n) __builtin_clzg(n)
#define mtb_roundup_pow2(n) (u64_lit(1) << (sizeof(u64) * CHAR_BIT - mtb_leading_zeros_count(n - 1)))


/* Memory Utils */

#define kibi(n) ((u64)((n) * (1 << 10)))
#define mebi(n) ((u64)((n) * (1 << 20)))
#define gibi(n) ((u64)((n) * (1 << 30)))

#define kb(n) kibi(n)
#define mb(n) mebi(n)
#define gb(n) gibi(n)

#define mtb_align_pow2(s, a) ((u64)(((s) + (a) - 1) & (~((a) - 1))))
#define mtb_align_padding_pow2(s, a) ((u64)((-(s)) & ((a) - 1)))

#if defined(_MTB_COMPILER_GCC)
# define mtb_alignof(T) __alignof__(T)
#elif defined(_MTB_COMPILER_CLANG)
# define mtb_alignof(T) __alignof(T)
#endif


/* Reflection Helpers */

#define _mtb_containerof(p, t, m, _p) ({ \
    u8 *_p = (u8 *)(p); \
    (t *)(_p ? _p - offsetof(t, m) : nil); \
})
#define mtb_containerof(p, t, m) _mtb_containerof(p, t, m, mtb_id(_p))

#if defined(_MTB_COMPILER_GCC)
# define mtb_typeof(T) __typeof__(T)
#elif defined(_MTB_COMPILER_CLANG)
# define mtb_typeof(T) __typeof(T)
#endif


/* Unit Helpers */

#define kilo(n) ((u64)((n) * 1000))
#define mega(n) ((u64)((n) * 1000000))
#define giga(n) ((u64)((n) * 1000000000))

#define thousand(n) kilo(n)
#define million(n)  mega(n)
#define billion(n)  giga(n)


/* Debug Helpers */

#define mtb_assert_always(c) if (!(c)) __builtin_trap()
#ifdef MTB_DEBUG
#define mtb_assert(c) mtb_assert_always(c)
#else
#define mtb_assert(c) ((void)(c))
#endif

#define mtb_invalid mtb_assert_always(!"INVALID")
#define mtb_todo mtb_assert_always(!"TODO")
#define mtp_noop ((void)0)
#define mtb_unreachable() __builtin_unreachable()

#endif // MTB_MACRO_H
#ifndef MTB_TYPE_H
#define MTB_TYPE_H

#ifdef MTB_IMPLEMENTATION
#define MTB_TYPE_IMPLEMENTATION
#endif

#ifdef MTB_TESTS
#define MTB_TYPE_TESTS
#endif


#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>


/* Type Aliases */

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float    f32;
typedef double   f64;


/* Type Limits */

global i8  I8_MAX  = INT8_MAX;
global i16 I16_MAX = INT16_MAX;
global i32 I32_MAX = INT32_MAX;
global i64 I64_MAX = INT64_MAX;

global i8  I8_MIN  = INT8_MIN;
global i16 I16_MIN = INT16_MIN;
global i32 I32_MIN = INT32_MIN;
global i64 I64_MIN = INT64_MIN;

global u8  U8_MAX  = UINT8_MAX;
global u16 U16_MAX = UINT16_MAX;
global u32 U32_MAX = UINT32_MAX;
global u64 U64_MAX = UINT64_MAX;

global f32 F32_MAX = FLT_MAX;
global f64 F64_MAX = DBL_MAX;

global f32 F32_MIN = FLT_MIN;
global f64 F64_MIN = DBL_MIN;


/* Typed Constant Literal */

#define i8_lit(c)  INT8_C(c)
#define i16_lit(c) INT16_C(c)
#define i32_lit(c) INT32_C(c)
#define i64_lit(c) INT64_C(c)

#define u8_lit(c)  UINT8_C(c)
#define u16_lit(c) UINT16_C(c)
#define u32_lit(c) UINT32_C(c)
#define u64_lit(c) UINT64_C(c)


/* Safe Comparisons */

#define _mtb_min(a, b, t, _a, _b) ({ \
    t _a = (a); \
    t _b = (b); \
    _a < _b ? _a : _b; \
})
#define _mtb_max(a, b, t, _a, _b) ({ \
    t _a = (a); \
    t _b = (b); \
    _a > _b ? _a : _b; \
})

#define mtb_min_i8(a, b)  _mtb_min(a, b, i8 , mtb_id(_a), mtb_id(_b))
#define mtb_max_i8(a, b)  _mtb_max(a, b, i8 , mtb_id(_a), mtb_id(_b))

#define mtb_min_i16(a, b) _mtb_min(a, b, i16, mtb_id(_a), mtb_id(_b))
#define mtb_max_i16(a, b) _mtb_max(a, b, i16, mtb_id(_a), mtb_id(_b))

#define mtb_min_i32(a, b) _mtb_min(a, b, i32, mtb_id(_a), mtb_id(_b))
#define mtb_max_i32(a, b) _mtb_max(a, b, i32, mtb_id(_a), mtb_id(_b))

#define mtb_min_i64(a, b) _mtb_min(a, b, i64, mtb_id(_a), mtb_id(_b))
#define mtb_max_i64(a, b) _mtb_max(a, b, i64, mtb_id(_a), mtb_id(_b))

#define mtb_min_u8(a, b)  _mtb_min(a, b, u8 , mtb_id(_a), mtb_id(_b))
#define mtb_max_u8(a, b)  _mtb_max(a, b, u8 , mtb_id(_a), mtb_id(_b))

#define mtb_min_u16(a, b) _mtb_min(a, b, u16, mtb_id(_a), mtb_id(_b))
#define mtb_max_u16(a, b) _mtb_max(a, b, u16, mtb_id(_a), mtb_id(_b))

#define mtb_min_u32(a, b) _mtb_min(a, b, u32, mtb_id(_a), mtb_id(_b))
#define mtb_max_u32(a, b) _mtb_max(a, b, u32, mtb_id(_a), mtb_id(_b))

#define mtb_min_u64(a, b) _mtb_min(a, b, u64, mtb_id(_a), mtb_id(_b))
#define mtb_max_u64(a, b) _mtb_max(a, b, u64, mtb_id(_a), mtb_id(_b))


/* Safe Arithmetic */

#define _mtb_add(a, b, t, _a, _b, _r) ({ \
    t _a = (a); \
    t _b = (b); \
    t _r; \
    mtb_assert_always(!__builtin_add_overflow(_a, _b, &_r)); \
    _r; \
})
#define _mtb_sub(a, b, t, _a, _b, _r) ({ \
    t _a = (a); \
    t _b = (b); \
    t _r; \
    mtb_assert_always(!__builtin_sub_overflow(_a, _b, &_r)); \
    _r; \
})
#define _mtb_mul(a, b, t, _a, _b, _r) ({ \
    t _a = (a); \
    t _b = (b); \
    t _r; \
    mtb_assert_always(!__builtin_mul_overflow(_a, _b, &_r)); \
    _r; \
})

#define mtb_add_i8(a, b)  _mtb_add(a, b, i8 , mtb_id(_a), mtb_id(_b), mtb_id(_r))
#define mtb_sub_i8(a, b)  _mtb_sub(a, b, i8 , mtb_id(_a), mtb_id(_b), mtb_id(_r))
#define mtb_mul_i8(a, b)  _mtb_mul(a, b, i8 , mtb_id(_a), mtb_id(_b), mtb_id(_r))

#define mtb_add_i16(a, b) _mtb_add(a, b, i16, mtb_id(_a), mtb_id(_b), mtb_id(_r))
#define mtb_sub_i16(a, b) _mtb_sub(a, b, i16, mtb_id(_a), mtb_id(_b), mtb_id(_r))
#define mtb_mul_i16(a, b) _mtb_mul(a, b, i16, mtb_id(_a), mtb_id(_b), mtb_id(_r))

#define mtb_add_i32(a, b) _mtb_add(a, b, i32, mtb_id(_a), mtb_id(_b), mtb_id(_r))
#define mtb_sub_i32(a, b) _mtb_sub(a, b, i32, mtb_id(_a), mtb_id(_b), mtb_id(_r))
#define mtb_mul_i32(a, b) _mtb_mul(a, b, i32, mtb_id(_a), mtb_id(_b), mtb_id(_r))

#define mtb_add_i64(a, b) _mtb_add(a, b, i64, mtb_id(_a), mtb_id(_b), mtb_id(_r))
#define mtb_sub_i64(a, b) _mtb_sub(a, b, i64, mtb_id(_a), mtb_id(_b), mtb_id(_r))
#define mtb_mul_i64(a, b) _mtb_mul(a, b, i64, mtb_id(_a), mtb_id(_b), mtb_id(_r))

#define mtb_add_u8(a, b)  _mtb_add(a, b, u8 , mtb_id(_a), mtb_id(_b), mtb_id(_r))
#define mtb_sub_u8(a, b)  _mtb_sub(a, b, u8 , mtb_id(_a), mtb_id(_b), mtb_id(_r))
#define mtb_mul_u8(a, b)  _mtb_mul(a, b, u8 , mtb_id(_a), mtb_id(_b), mtb_id(_r))

#define mtb_add_u16(a, b) _mtb_add(a, b, u16, mtb_id(_a), mtb_id(_b), mtb_id(_r))
#define mtb_sub_u16(a, b) _mtb_sub(a, b, u16, mtb_id(_a), mtb_id(_b), mtb_id(_r))
#define mtb_mul_u16(a, b) _mtb_mul(a, b, u16, mtb_id(_a), mtb_id(_b), mtb_id(_r))

#define mtb_add_u32(a, b) _mtb_add(a, b, u32, mtb_id(_a), mtb_id(_b), mtb_id(_r))
#define mtb_sub_u32(a, b) _mtb_sub(a, b, u32, mtb_id(_a), mtb_id(_b), mtb_id(_r))
#define mtb_mul_u32(a, b) _mtb_mul(a, b, u32, mtb_id(_a), mtb_id(_b), mtb_id(_r))

#define mtb_add_u64(a, b) _mtb_add(a, b, u64, mtb_id(_a), mtb_id(_b), mtb_id(_r))
#define mtb_sub_u64(a, b) _mtb_sub(a, b, u64, mtb_id(_a), mtb_id(_b), mtb_id(_r))
#define mtb_mul_u64(a, b) _mtb_mul(a, b, u64, mtb_id(_a), mtb_id(_b), mtb_id(_r))


/* Type casts */

#define i8_cast(c)  ((i8)(c))
#define i16_cast(c) ((i16)(c))
#define i32_cast(c) ((i32)(c))
#define i64_cast(c) ((i64)(c))

#define u8_cast(c)  ((u8)(c))
#define u16_cast(c) ((u16)(c))
#define u32_cast(c) ((u32)(c))
#define u64_cast(c) ((u64)(c))

#define f32_cast(c) ((f32)(c))
#define f64_cast(c) ((f64)(c))


/* Safe Type Casts */

#define _mtb_cast_min(a, f, t, tmin, _a) ({ \
    f _a = (a); \
    mtb_assert_always((tmin) <= _a ); \
    (t)_a;\
})
#define _mtb_cast_max(a, f, t, tmax, _a) ({ \
    f _a = (a); \
    mtb_assert_always(_a <= (tmax)); \
    (t)_a;\
})
#define _mtb_cast_minmax(a, f, t, tmin, tmax, _a) ({ \
    f _a = (a); \
    mtb_assert_always((tmin) <= _a && _a <= (tmax)); \
    (t)_a;\
})

#define mtb_cast_i16_i8(a)  _mtb_cast_minmax(a, i16, i8,  I8_MIN,  I8_MAX , mtb_id(_a))
#define mtb_cast_i32_i8(a)  _mtb_cast_minmax(a, i32, i8,  I8_MIN,  I8_MAX , mtb_id(_a))
#define mtb_cast_i32_i16(a) _mtb_cast_minmax(a, i32, i16, I16_MIN, I16_MAX, mtb_id(_a))
#define mtb_cast_i64_i8(a)  _mtb_cast_minmax(a, i64, i8,  I8_MIN,  I8_MAX , mtb_id(_a))
#define mtb_cast_i64_i16(a) _mtb_cast_minmax(a, i64, i16, I16_MIN, I16_MAX, mtb_id(_a))
#define mtb_cast_i64_i32(a) _mtb_cast_minmax(a, i64, i32, I32_MIN, I32_MAX, mtb_id(_a))

#define mtb_cast_i8_u8(a)   _mtb_cast_min(a, i8,  u8,  0, mtb_id(_a))
#define mtb_cast_i16_u16(a) _mtb_cast_min(a, i16, u16, 0, mtb_id(_a))
#define mtb_cast_i32_u32(a) _mtb_cast_min(a, i32, u32, 0, mtb_id(_a))
#define mtb_cast_i64_u64(a) _mtb_cast_min(a, i64, u64, 0, mtb_id(_a))

#define mtb_cast_u16_u8(a)  _mtb_cast_max(a, u16, u8,  U8_MAX , mtb_id(_a))
#define mtb_cast_u32_u8(a)  _mtb_cast_max(a, u32, u8,  U8_MAX , mtb_id(_a))
#define mtb_cast_u32_u16(a) _mtb_cast_max(a, u32, u16, U16_MAX, mtb_id(_a))
#define mtb_cast_u64_u8(a)  _mtb_cast_max(a, u64, u8,  U8_MAX , mtb_id(_a))
#define mtb_cast_u64_u16(a) _mtb_cast_max(a, u64, u16, U16_MAX, mtb_id(_a))
#define mtb_cast_u64_u32(a) _mtb_cast_max(a, u64, u32, U32_MAX, mtb_id(_a))

#define mtb_cast_u8_i8(a)   _mtb_cast_max(a, u8,  i8,   (u8)I8_MAX , mtb_id(_a))
#define mtb_cast_u16_i16(a) _mtb_cast_max(a, u16, i16, (u16)I16_MAX, mtb_id(_a))
#define mtb_cast_u32_i32(a) _mtb_cast_max(a, u32, i32, (u32)I32_MAX, mtb_id(_a))
#define mtb_cast_u64_i64(a) _mtb_cast_max(a, u64, i64, (u64)I64_MAX, mtb_id(_a))


/* Char Operations */

#define mtb_cast_char_u8(a) _mtb_cast_min(a, char, u8,   0,             mtb_id(_a))
#define mtb_cast_u8_char(a) _mtb_cast_max(a, u8,   char, (u8)CHAR_MAX , mtb_id(_a))

func bool mtb_char_is_lower(char c);
func bool mtb_char_is_upper(char c);
func bool mtb_char_is_alpha(char c);
func bool mtb_char_is_alnum(char c);
func bool mtb_char_is_space(char c);
func bool mtb_char_is_digit(char c);
func bool mtb_char_is_xdigit(char c);

func char mtb_char_to_lower(char c);
func char mtb_char_to_upper(char c);

#endif // MTB_TYPE_H


#ifdef MTB_TYPE_IMPLEMENTATION

func bool
mtb_char_is_lower(char c)
{
    return 'a' <= c && c <= 'z';
}

func bool
mtb_char_is_upper(char c)
{
    return 'A' <= c && c <= 'Z';
}

func bool
mtb_char_is_alpha(char c)
{
    return mtb_char_is_lower(c) || mtb_char_is_upper(c);
}

func bool
mtb_char_is_digit(char c)
{
    return '0' <= c && c <= '9';
}

func bool
mtb_char_is_xdigit(char c)
{
    return mtb_char_is_digit(c) || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f');
}

func bool
mtb_char_is_alnum(char c)
{
    return mtb_char_is_alpha(c) || mtb_char_is_digit(c);
}

func bool
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

func char
mtb_char_to_lower(char c)
{
    return mtb_char_is_upper(c) ? c + ('a' - 'A') : c;
}

func char
mtb_char_to_upper(char c)
{
    return mtb_char_is_lower(c) ? c + ('A' - 'a') : c;
}

#endif // MTB_TYPE_IMPLEMENTATION


#ifdef MTB_TYPE_TESTS

#include <assert.h>


func void
_test_mtb_min(void)
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

func void
_test_mtb_max(void)
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

func void
_test_mtb_add(void)
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

func void
_test_mtb_sub(void)
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

func void
_test_mtb_mul(void)
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

func void
_test_mtb_cast(void)
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

func void
_test_mtb_type(void)
{
    _test_mtb_min();
    _test_mtb_max();
    _test_mtb_add();
    _test_mtb_sub();
    _test_mtb_mul();
    _test_mtb_cast();
}

#endif // MTB_TYPE_TESTS
#ifndef MTB_RNG_H
#define MTB_RNG_H

#ifdef MTB_IMPLEMENTATION
#define MTB_RNG_IMPLEMENTATION
#endif

#ifdef MTB_TESTS
#define MTB_RNG_TESTS
#endif


// Based on "PCG: A Family of Simple Fast Space-Efficient Statistically Good Algorithms
// for Random Number Generation", M.E. O'Neill, 2014, https://www.pcg-random.org


typedef struct mtb_rng32 MtbRng32;
struct mtb_rng32
{
    u64 state;
    u64 inc; // must be odd!
};

typedef struct mtb_rng64 MtbRng64;
struct mtb_rng64
{
    MtbRng32 gens[2];
};


func void mtb_rng32_init(MtbRng32 *rng, u64 seed);
func void mtb_rng64_init(MtbRng64 *rng, u64 seed);
func void mtb_rng32_global_init(u64 seed);
func void mtb_rng64_global_init(u64 seed);

// [0, MAX]
func u32 mtb_rng32_next(MtbRng32 *rng);
func u64 mtb_rng64_next(MtbRng64 *rng);
func u32 mtb_rng32_global_next(void);
func u64 mtb_rng64_global_next(void);

// [0, range)
func u32 mtb_rng32_next_bounded(MtbRng32 *rng, u32 range);
func u64 mtb_rng64_next_bounded(MtbRng64 *rng, u64 range);
func u32 mtb_rng32_global_next_bounded(u32 range);
func u64 mtb_rng64_global_next_bounded(u64 range);

// [0.0, 1.0]
func f32 mtb_rng32_next_unit(MtbRng32 *rng);
func f64 mtb_rng64_next_unit(MtbRng64 *rng);
func f32 mtb_rng32_global_next_unit(void);
func f64 mtb_rng64_global_next_unit(void);

#endif //MTB_RNG_H


#ifdef MTB_RNG_IMPLEMENTATION

global MtbRng32 _rng32_global = { u64_lit(0x853c49e6748fea9b), u64_lit(0xda3e39cb94b95bdb) };
global MtbRng64 _rng64_global = {{
    { u64_lit(0x979c9a98d8462005), u64_lit(0x7d3e9cb6cfe0549b) },
    { u64_lit(0x0000000000000001), u64_lit(0xda3e39cb94b95bdb) }
}};


func u64
_mtb_rng32_splitmix64(u64 *seed) {
    u64 result = (*seed += u64_lit(0x9e3779b97f4a7c15));
    result = (result ^ (result >> 30)) * u64_lit(0xbf58476d1ce4e5b9);
    result = (result ^ (result >> 27)) * u64_lit(0x94d049bb133111eb);
    return result ^ (result >> 31);
}

func void
mtb_rng32_init(MtbRng32 *rng, u64 seed)
{
    // Use splitmix64 for proper seeding
    rng->state = _mtb_rng32_splitmix64(&seed);
    rng->inc = _mtb_rng32_splitmix64(&seed) | 1; // ensure oddness
}

func void
mtb_rng64_init(MtbRng64 *rng, u64 seed)
{
    mtb_rng32_init(&rng->gens[0], seed);
    do {
        mtb_rng32_init(&rng->gens[1], _mtb_rng32_splitmix64(&seed));
    } while (rng->gens[0].inc == rng->gens[1].inc); // must be distinct!
}

func void
mtb_rng32_global_init(u64 seed)
{
    mtb_rng32_init(&_rng32_global, seed);
}

func void
mtb_rng64_global_init(u64 seed)
{
    mtb_rng64_init(&_rng64_global, seed);
}

func u32
mtb_rng32_next(MtbRng32 *rng)
{
    // PCG-XSH-RR: output = rotate32((state ^ (state >> 18)) >> 27, state >> 59)
    u64 state = rng->state;
    u32 xorShifted = u32_cast((state ^ (state >> 18)) >> 27);
    u32 rotation = u32_cast(state >> 59);
    u32 output = (xorShifted >> rotation) | (xorShifted << ((-rotation) & 31));

    rng->state = state * u64_lit(0x5851f42d4c957f2d) + rng->inc;

    return output;
}

func u64
mtb_rng64_next(MtbRng64 *rng)
{
    u32 h = mtb_rng32_next(&rng->gens[0]);
    u32 l = mtb_rng32_next(&rng->gens[1]);
    return ((u64)h << 32) | l;
}

func u32
mtb_rng32_global_next(void)
{
    return mtb_rng32_next(&_rng32_global);
}

func u64
mtb_rng64_global_next(void)
{
    return mtb_rng64_next(&_rng64_global);
}

func u32
mtb_rng32_next_bounded(MtbRng32 *rng, u32 range)
{
    mtb_assert_always(range > 0);
    u32 mask = U32_MAX >> mtb_leading_zeros_count((range - 1) | 1);
    u32 x;
    do {
        x = mtb_rng32_next(rng) & mask;
    } while (x >= range);
    return x;
}

func u64
mtb_rng64_next_bounded(MtbRng64 *rng, u64 range)
{
    mtb_assert_always(range > 0);
    u64 mask = U64_MAX >> mtb_leading_zeros_count((range - 1) | 1);
    u64 x;
    do {
        x = mtb_rng64_next(rng) & mask;
    } while (x >= range);
    return x;
}

func u32
mtb_rng32_global_next_bounded(u32 range)
{
    return mtb_rng32_next_bounded(&_rng32_global, range);
}

func u64
mtb_rng64_global_next_bounded(u64 range)
{
    return mtb_rng64_next_bounded(&_rng64_global, range);
}

func f32
mtb_rng32_next_unit(MtbRng32 *rng)
{
    return f32_cast(mtb_rng32_next(rng) >> 8) * 0x1.0p-24f;
}

func f64
mtb_rng64_next_unit(MtbRng64 *rng)
{
    return f64_cast(mtb_rng64_next(rng) >> 11) * 0x1.0p-53;
}

func f32
mtb_rng32_global_next_unit(void)
{
    return mtb_rng32_next_unit(&_rng32_global);
}

func f64
mtb_rng64_global_next_unit(void)
{
    return mtb_rng64_next_unit(&_rng64_global);
}

#endif // MTB_RNG_IMPLEMENTATION


#ifdef MTB_RNG_TESTS

#include <assert.h>
#include <time.h>


func void
_test_mtb_rng32()
{
    MtbRng32 rng = {0};
    mtb_rng32_init(&rng, time(nil));

    i32 n = 100000;

    for (i32 i = 0; i < n; i++) {
        u32 x = mtb_rng32_next_bounded(&rng, 10);
        assert(x < 10);
    }

    for (i32 i = 0; i < n; i++) {
        f32 x = mtb_rng32_next_unit(&rng);
        assert(0.0f <= x && x <= 1.0f);
    }
}

func void
_test_mtb_rng64()
{
    MtbRng64 rng = {0};
    mtb_rng64_init(&rng, time(nil));

    i32 n = 100000;

    for (i32 i = 0; i < n; i++) {
        u64 x = mtb_rng64_next_bounded(&rng, 10);
        assert(x < 10);
    }

    for (i32 i = 0; i < n; i++) {
        f64 x = mtb_rng64_next_unit(&rng);
        assert(0.0f <= x && x <= 1.0f);
    }
}

func void
_test_mtb_rng()
{
    _test_mtb_rng32();
    _test_mtb_rng64();
}

#endif // MTB_RNG_TESTS
#ifndef MTB_PERF_H
#define MTB_PERF_H

#ifdef MTB_IMPLEMENTATION
#define MTB_PERF_IMPLEMENTATION
#endif



#ifndef MTB_PERF_BLOCKS_MAX
#define MTB_PERF_BLOCKS_MAX 4096
#endif


typedef struct mtb_perf_barker_marker MtbPerfBlockMarker;
struct mtb_perf_barker_marker
{
    const char *blockName;
    u32 blockIndex;
    u32 parentBlockIndex;
    u64 cpuTimeEnter;
};

typedef struct mtb_perf_block MtbPerfBlock;
struct mtb_perf_block
{
    const char *name;
    u64 cpuTimeElapsed;
    u64 cpuTimeElapsedWithChildren;
    u64 hitCount;
};

typedef struct mtb_perf_profile MtbPerfProfile;
struct mtb_perf_profile
{
    MtbPerfBlock blocks[MTB_PERF_BLOCKS_MAX];
    u64 cpuTimeStart;
};


func void mtb_perf_start();
func void mtb_perf_print();

func u64 mtb_perf_cpu_time();
func u64 mtb_perf_cpu_freq();

func u64 mtb_perf_sys_time();
func u64 mtb_perf_sys_freq();

func MtbPerfBlockMarker mtb_perf_block_enter(const char *name, u32 index);
func void mtb_perf_block_exit(MtbPerfBlockMarker *marker);

#ifdef MTB_PERF_ENABLED
#define _mtb_perf_time_block(name, _m) \
    MtbPerfBlockMarker _m __attribute__((__cleanup__(mtb_perf_block_exit))) = mtb_perf_block_enter(name, (__COUNTER__ + 1))
#define mtb_perf_time_block(name) _mtb_perf_time_block(name, mtb_id(_m))
#define mtb_perf_time_func() mtb_perf_time_block(__FUNCTION__)
#else
#define mtb_perf_time_block(name)
#define mtb_perf_time_func()
#endif

#endif //MTB_PERF_H


#ifdef MTB_PERF_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>
#include <x86intrin.h>


#ifdef MTB_PERF_ENABLED
global MtbPerfProfile _mtb_perf_global_profile = {0};
global thread_local u32 _mtb_perf_current_block_index = 0;
#endif


func void
mtb_perf_start()
{
#ifdef MTB_PERF_ENABLED
    _mtb_perf_global_profile.cpuTimeStart = mtb_perf_cpu_time();
    memset(_mtb_perf_global_profile.blocks, 0, MTB_PERF_BLOCKS_MAX * sizeof(MtbPerfBlock));
#endif
}

func void
mtb_perf_print()
{
#ifdef MTB_PERF_ENABLED
    u64 cpuTimeEnd = mtb_perf_cpu_time();

    u64 cpuFreq = mtb_perf_cpu_freq();
    printf("cpu freq: %.2fGHz\n", (f32)cpuFreq / billion(1));

    u64 totalCpuTimeElapsed = cpuTimeEnd - _mtb_perf_global_profile.cpuTimeStart;
    printf("total cpu time: %lu\n", totalCpuTimeElapsed);

    for (u32 i = 1; i < MTB_PERF_BLOCKS_MAX; i++) {
        MtbPerfBlock *block = &_mtb_perf_global_profile.blocks[i];
        if (block->name) {
            printf("[%s]\n", block->name);
            printf("\thits: %lu\n", block->hitCount);
            printf("\tcpu time: %lu (%.2f%%)\n",
                   block->cpuTimeElapsed,
                   (f32)block->cpuTimeElapsed / (f32)totalCpuTimeElapsed * 100.0f);
            if (block->cpuTimeElapsedWithChildren != block->cpuTimeElapsed) {
                printf("\tcpu time w/ children: %lu (%.2f%%)\n",
                       block->cpuTimeElapsedWithChildren,
                       (f32)block->cpuTimeElapsedWithChildren / (f32)totalCpuTimeElapsed * 100.0f);
            }
        }
    }
#endif
}

func u64
mtb_perf_cpu_time()
{
    _mm_lfence();
    return __rdtsc();
}

func u64
mtb_perf_cpu_freq()
{
    u64 cpuTimeStart = mtb_perf_cpu_time();
    u64 sysTimeStart = mtb_perf_sys_time();
    u64 sysFreq = mtb_perf_sys_freq();

    usleep(2000); // 2ms

    u64 sysTimeEnd = mtb_perf_sys_time();
    u64 cpuTimeEnd = mtb_perf_cpu_time();
    u64 sysTimeElapsed = sysTimeEnd - sysTimeStart;
    u64 cpuTimeElapsed = cpuTimeEnd - cpuTimeStart;
    u64 cpuFreq = cpuTimeElapsed / sysTimeElapsed * sysFreq;

    return cpuFreq;
}

func u64
mtb_perf_sys_time()
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        perror("clock_gettime");
        return 0;
    }
    return billion(ts.tv_sec) + ts.tv_nsec;
}

func u64
mtb_perf_sys_freq()
{
    return billion(1); // nanoseconds
}

func MtbPerfBlockMarker
mtb_perf_block_enter(const char *name, u32 index)
{
    MtbPerfBlockMarker marker = {0};
#ifdef MTB_PERF_ENABLED
    marker.blockName = name;
    marker.blockIndex = index;
    marker.parentBlockIndex = _mtb_perf_current_block_index;
    _mtb_perf_current_block_index = index;

    marker.cpuTimeEnter = mtb_perf_cpu_time();
#endif
    return marker;
}

func void
mtb_perf_block_exit(MtbPerfBlockMarker *marker)
{
#ifdef MTB_PERF_ENABLED
    u64 cpuTimeExit = mtb_perf_cpu_time();

    u64 cpuTimeElapsed = cpuTimeExit - marker->cpuTimeEnter;

    MtbPerfBlock *block = &_mtb_perf_global_profile.blocks[marker->blockIndex];
    block->name = marker->blockName;
    block->cpuTimeElapsed += cpuTimeElapsed;
    block->cpuTimeElapsedWithChildren += cpuTimeElapsed;
    block->hitCount++;

    if (marker->parentBlockIndex) {
        MtbPerfBlock *parentBlock = &_mtb_perf_global_profile.blocks[marker->parentBlockIndex];
        parentBlock->cpuTimeElapsed -= cpuTimeElapsed;
    }

    _mtb_perf_current_block_index = marker->parentBlockIndex;
#endif
}

#endif //MTB_PERF_IMPLEMENTATION
#ifndef MTB_LIST_H
#define MTB_LIST_H

#ifdef MTB_IMPLEMENTATION
#define MTB_LIST_IMPLEMENTATION
#endif

#ifdef MTB_TESTS
#define MTB_LIST_TESTS
#endif


typedef struct mtb_list_node MtbList;
struct mtb_list_node
{
    MtbList *next;
    MtbList *prev;
};


/* Doubly Linked List API */

#define _mtb_list_foreach(head, var, _head) \
    for (MtbList *_head = (head), *var = _head->next; var != _head; var = var->next)
#define mtb_list_foreach(head, var) _mtb_list_foreach(head, var, mtb_id(_head))

func void mtb_list_init(MtbList *head);
func void mtb_list_reset(MtbList *head);
func bool mtb_list_is_empty(MtbList *head);

func MtbList *mtb_list_remove(MtbList *item);
func MtbList *mtb_list_insert_before(MtbList *pivot, MtbList *item);
func MtbList *mtb_list_insert_after(MtbList *pivot, MtbList *item);

func MtbList *mtb_list_add_last(MtbList *head, MtbList *item);
func MtbList *mtb_list_remove_last(MtbList *head);
func MtbList *mtb_list_get_last(MtbList *head);

func MtbList *mtb_list_add_first(MtbList *head, MtbList *item);
func MtbList *mtb_list_remove_first(MtbList *head);
func MtbList *mtb_list_get_first(MtbList *head);


/* Stack API */

func MtbList *mtb_list_push(MtbList *head, MtbList *item);
func MtbList *mtb_list_pop(MtbList *head);
func MtbList *mtb_list_top(MtbList *head);


/* Queue API */

func MtbList *mtb_list_enq(MtbList *head, MtbList *item);
func MtbList *mtb_list_deq(MtbList *head);
func MtbList *mtb_list_front(MtbList *head);


/* Iterator API */

typedef struct mtb_list_iter MtbListIter;
struct mtb_list_iter
{
    MtbList *head;
    MtbList *prev;
    MtbList *next;
};


func void mtb_list_iter_init(MtbListIter *it, MtbList *head);
func void mtb_list_iter_reset(MtbListIter *it);
func bool mtb_list_iter_has_next(MtbListIter *it);
func void *mtb_list_iter_next(MtbListIter *it);
func void *mtb_list_iter_remove(MtbListIter *it);

#endif //MTB_LIST_H


#ifdef MTB_LIST_IMPLEMENTATION

func void
mtb_list_init(MtbList *head)
{
    head->next = head->prev = head;
}

func void
mtb_list_reset(MtbList *head)
{
    mtb_list_init(head);
}

func bool
mtb_list_is_empty(MtbList *head)
{
    return head->next == head;
}

func MtbList *
mtb_list_remove(MtbList *item)
{
    item->prev->next = item->next;
    item->next->prev = item->prev;
    return item;
}

func MtbList *
mtb_list_insert_before(MtbList *pivot, MtbList *item)
{
    item->prev = pivot->prev;
    item->next = pivot;
    pivot->prev->next = item;
    pivot->prev = item;
    return item;
}

func MtbList *
mtb_list_insert_after(MtbList *pivot, MtbList *item)
{
    return mtb_list_insert_before(pivot->next, item);
}

func MtbList *
mtb_list_add_last(MtbList *head, MtbList *item)
{
    return mtb_list_insert_before(head, item);
}

func MtbList *
mtb_list_remove_last(MtbList *head)
{
    return mtb_list_is_empty(head) ? nil : mtb_list_remove(head->prev);
}

func MtbList *
mtb_list_get_last(MtbList *head)
{
    return mtb_list_is_empty(head) ? nil : head->prev;
}

func MtbList *
mtb_list_add_first(MtbList *head, MtbList *item)
{
    return mtb_list_insert_after(head, item);
}

func MtbList *
mtb_list_remove_first(MtbList *head)
{
    return mtb_list_is_empty(head) ? nil : mtb_list_remove(head->next);
}

func MtbList *
mtb_list_get_first(MtbList *head)
{
    return mtb_list_is_empty(head) ? nil : head->next;
}

func MtbList *
mtb_list_push(MtbList *head, MtbList *item)
{
    return mtb_list_add_last(head, item);
}

func MtbList *
mtb_list_pop(MtbList *head)
{
    return mtb_list_remove_last(head);
}

func MtbList *
mtb_list_top(MtbList *head)
{
    return mtb_list_get_last(head);
}

func MtbList *
mtb_list_enq(MtbList *head, MtbList *item)
{
    return mtb_list_add_last(head, item);
}

func MtbList *
mtb_list_deq(MtbList *head)
{
    return mtb_list_remove_first(head);
}

func MtbList *
mtb_list_front(MtbList *head)
{
    return mtb_list_get_first(head);
}

func void
mtb_list_iter_init(MtbListIter *it, MtbList *head)
{
    it->head = head;
    mtb_list_iter_reset(it);
}

func void
mtb_list_iter_reset(MtbListIter *it)
{
    it->prev = it->head;
    it->next = nil;
}

func bool
mtb_list_iter_has_next(MtbListIter *it)
{
    it->next = it->prev->next;
    return it->next != it->head;
}

func void *
mtb_list_iter_next(MtbListIter *it)
{
    mtb_assert_always(it->next != nil);
    it->prev = it->next;
    it->next = nil;
    return it->prev;
}

func void *
mtb_list_iter_remove(MtbListIter *it)
{
    mtb_assert_always(it->prev != it->head);
    return mtb_list_remove(it->prev);
}

#endif // MTB_LIST_IMPLEMENTATION


#ifdef MTB_LIST_TESTS

#include <assert.h>


func void
_test_mtb_list_list(void)
{
    MtbList list = {0};
    MtbList first = {0};
    MtbList last = {0};
    MtbList *expOrder[] = {&first, &last};

    mtb_list_init(&list);
    assert(mtb_list_is_empty(&list));

    mtb_list_add_first(&list, &first);
    mtb_list_add_last(&list, &last);
    assert(!mtb_list_is_empty(&list));

    u32 i = 0;
    mtb_list_foreach(&list, item) {
        MtbList *expItem = expOrder[i++];
        assert(item == expItem);
    }

    assert(mtb_list_get_first(&list) == &first);
    assert(mtb_list_get_last(&list) == &last);
    assert(mtb_list_remove_first(&list) == &first);
    assert(mtb_list_remove_last(&list) == &last);
    assert(mtb_list_is_empty(&list));
}

func void
_test_mtb_list_stack(void)
{
    MtbList stack = {0};
    MtbList itemA = {0};
    MtbList itemB = {0};
    MtbList itemC = {0};
    MtbList *expOrder[] = {&itemA, &itemB, &itemC};

    mtb_list_init(&stack);
    assert(mtb_list_is_empty(&stack));

    mtb_list_push(&stack, &itemA);
    mtb_list_push(&stack, &itemB);
    mtb_list_push(&stack, &itemC);
    assert(!mtb_list_is_empty(&stack));

    u32 i = 0;
    mtb_list_foreach(&stack, item) {
        MtbList *expItem = expOrder[i++];
        assert(item == expItem);
    }

    assert(mtb_list_top(&stack) == &itemC);
    assert(mtb_list_pop(&stack) == &itemC);
    assert(mtb_list_top(&stack) == &itemB);
    assert(mtb_list_pop(&stack) == &itemB);
    assert(mtb_list_top(&stack) == &itemA);
    assert(mtb_list_pop(&stack) == &itemA);
    assert(mtb_list_is_empty(&stack));
}

func void
_test_mtb_list_queue(void)
{
    MtbList queue = {0};
    MtbList itemA = {0};
    MtbList itemB = {0};
    MtbList itemC = {0};
    MtbList *expOrder[] = {&itemA, &itemB, &itemC};

    mtb_list_init(&queue);
    assert(mtb_list_is_empty(&queue));

    mtb_list_enq(&queue, &itemA);
    mtb_list_enq(&queue, &itemB);
    mtb_list_enq(&queue, &itemC);
    assert(!mtb_list_is_empty(&queue));

    u32 i = 0;
    mtb_list_foreach(&queue, item) {
        MtbList *expItem = expOrder[i++];
        assert(item == expItem);
    }

    assert(mtb_list_front(&queue) == &itemA);
    assert(mtb_list_deq(&queue) == &itemA);
    assert(mtb_list_front(&queue) == &itemB);
    assert(mtb_list_deq(&queue) == &itemB);
    assert(mtb_list_front(&queue) == &itemC);
    assert(mtb_list_deq(&queue) == &itemC);
    assert(mtb_list_is_empty(&queue));
}

func void
_test_mtb_list_iter(void)
{
    MtbList list = {0};
    MtbList itemA = {0};
    MtbList itemB = {0};
    MtbList itemC = {0};
    MtbList *expOrder[] = {&itemA, &itemB, &itemC};

    mtb_list_init(&list);
    assert(mtb_list_is_empty(&list));

    MtbListIter it = {0};
    mtb_list_iter_init(&it, &list);
    assert(!mtb_list_iter_has_next(&it));

    mtb_list_add_last(&list, &itemA);
    mtb_list_add_last(&list, &itemB);
    mtb_list_add_last(&list, &itemC);
    assert(!mtb_list_is_empty(&list));

    mtb_list_iter_reset(&it);
    assert(mtb_list_iter_has_next(&it));

    u32 i = 0;
    while (mtb_list_iter_has_next(&it)) {
        MtbList *expItem = expOrder[i++];
        assert(mtb_list_iter_next(&it) == expItem);
        assert(mtb_list_iter_remove(&it) == expItem);
    }

    mtb_list_iter_reset(&it);
    assert(!mtb_list_iter_has_next(&it));
    assert(mtb_list_is_empty(&list));
}

func void
_test_mtb_list(void)
{
    _test_mtb_list_list();
    _test_mtb_list_stack();
    _test_mtb_list_queue();
    _test_mtb_list_iter();
}

#endif // MTB_LIST_TESTS
#ifndef MTB_ARENA_H
#define MTB_ARENA_H

#ifdef MTB_IMPLEMENTATION
#define MTB_ARENA_IMPLEMENTATION
#endif

#ifdef MTB_TESTS
#define MTB_ARENA_TESTS
#endif


/* Arena Allocator */

typedef struct mtb_arena_allocator MtbArenaAllocator;
struct mtb_arena_allocator
{
    void *ctx;                                      /* custom context */
    void *(*alloc)(void *ctx, void *ptr, u64 size); /* allocate if size > 0, free otherwise */
    u64 (*size)(void *ctx, void *ptr);              /* return allocation size */
};

func u64 mtb_arena_def_size(void *ctx, void *ptr);
func void *mtb_arena_def_alloc(void *ctx, void *ptr, u64 size);

func u64 mtb_arena_def_virt_size(void *ctx, void *ptr);
func void *mtb_arena_def_virt_alloc(void *ctx, void *ptr, u64 size);


/* Arena */

#ifndef MTB_ARENA_DEF_ALIGN
#define MTB_ARENA_DEF_ALIGN sizeof(void *)
#endif

typedef struct mtb_arena MtbArena;
struct mtb_arena
{
    u8 *base;
    u64 offset;
    u64 size;
    MtbArenaAllocator *allocator;
};

typedef struct mtb_arena_bump_options MtbArenaBumpOptions;
struct mtb_arena_bump_options
{
    u64 align;
    bool no_zero;
};

func void mtb_arena_init(MtbArena *arena, u64 size, MtbArenaAllocator *allocator);
func void mtb_arena_deinit(MtbArena *arena);

func void *mtb_arena_bump_opt(MtbArena *arena, u64 size, MtbArenaBumpOptions opt);
#define mtb_arena_bump_raw(arena, size, ...) \
    mtb_arena_bump_opt(arena, size, (MtbArenaBumpOptions){ __VA_ARGS__ })
#define mtb_arena_bump(arena, type, count, ...) \
    (type *)mtb_arena_bump_opt(arena, mtb_mul_u64(sizeof(type), (count)), (MtbArenaBumpOptions){ .align = mtb_alignof(type), __VA_ARGS__ })

func void mtb_arena_clear(MtbArena *arena);

#endif //MTB_ARENA_H


#ifdef MTB_ARENA_IMPLEMENTATION

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>


#define MTB_ARENA_DEF_ALLOCATOR_HEADER_SIZE sizeof(u64)


global MtbArenaAllocator MTB_ARENA_DEF_ALLOCATOR = {
    .ctx = nil,
    .alloc = mtb_arena_def_alloc,
    .size = mtb_arena_def_size,
};

global MtbArenaAllocator MTB_ARENA_DEF_VIRT_ALLOCATOR = {
    .ctx = nil,
    .alloc = mtb_arena_def_virt_alloc,
    .size = mtb_arena_def_virt_size,
};


func u64
mtb_arena_def_size(void *ctx, void *ptr)
{
    u8 *base = (u8 *)ptr;
    u64 headerSize = MTB_ARENA_DEF_ALLOCATOR_HEADER_SIZE;
    u64 *header = (u64 *)(base - headerSize);
    u64 allocSize = *header;

    return allocSize;
}

func void *
mtb_arena_def_alloc(void *ctx, void *ptr, u64 size)
{
    u8 *base = (u8 *)ptr;
    u64 headerSize = MTB_ARENA_DEF_ALLOCATOR_HEADER_SIZE;

    if (size == 0) {
        mtb_assert_always(base != nil);
        free(base - headerSize);
        return nil;
    }

    mtb_assert_always(base == nil);

    u64 allocSize = mtb_add_u64(headerSize, size);
    u8 *mallocAddr = (u8 *)malloc(allocSize);
    mtb_assert(mallocAddr != nil);

    u64 *header = (u64 *)mallocAddr;
    *header = allocSize;

    return header + 1;
}

func u64
mtb_arena_def_virt_size(void *ctx, void *ptr)
{
    u8 *base = (u8 *)ptr;
    u64 headerSize = MTB_ARENA_DEF_ALLOCATOR_HEADER_SIZE;
    u64 *header = (u64 *)(base - headerSize);
    u64 allocSize = *header;

    return allocSize;
}

func void *
mtb_arena_def_virt_alloc(void *ctx, void *ptr, u64 size)
{
    u8 *base = (u8 *)ptr;
    u64 headerSize = MTB_ARENA_DEF_ALLOCATOR_HEADER_SIZE;
    u64 pageSize = (u64)sysconf(_SC_PAGE_SIZE);
    mtb_assert(mtb_is_pow2(pageSize));

    if (size == 0) {
        mtb_assert_always(base != nil);
        u64 allocSize = mtb_arena_def_virt_size(ctx, base);
        mtb_assert(munmap(base - headerSize - pageSize, allocSize) == 0);
        return nil;
    }

    mtb_assert_always(base == nil);

    // Allocate enough memory to fit: arena, header, 2 guard pages.
    u64 sizeWithHeader = mtb_add_u64(size, headerSize);
    u64 sizeWithHeaderAligned = mtb_align_pow2(sizeWithHeader, pageSize);
    u64 allocSize = mtb_add_u64(sizeWithHeaderAligned, 2 * pageSize);
    u8 *mmapAddr = (u8 *)mmap(nil, allocSize, PROT_NONE, MAP_ANON | MAP_PRIVATE, -1, 0);
    mtb_assert(mmapAddr != MAP_FAILED);

    u64 *header = (u64 *)(mmapAddr + pageSize);
    mtb_assert(mprotect(header, sizeWithHeaderAligned, PROT_READ | PROT_WRITE) == 0);
    *header = allocSize;

    return header + 1;
}

func void
mtb_arena_init(MtbArena *arena, u64 size, MtbArenaAllocator *allocator)
{
    arena->allocator = allocator;
    arena->base = allocator->alloc(allocator->ctx, nil, size);
    arena->offset = 0;
    arena->size = size;
}

func void
mtb_arena_deinit(MtbArena *arena)
{
    MtbArenaAllocator *allocator = arena->allocator;
    allocator->alloc(allocator->ctx, arena->base, 0);
    arena->base = nil;
    arena->offset = 0;
    arena->size = 0;
    arena->allocator = nil;
}

func void *
mtb_arena_bump_opt(MtbArena *arena, u64 size, MtbArenaBumpOptions opt)
{
    mtb_assert_always(size > 0);
    mtb_assert_always(mtb_is_pow2_or_zero(opt.align));

    u64 oldOffset = arena->offset;
    u64 align = mtb_max_u64(opt.align, MTB_ARENA_DEF_ALIGN);
    u64 padding = mtb_align_padding_pow2((u64)(arena->base + oldOffset), align);
    u64 oldOffsetAligned = mtb_add_u64(oldOffset, padding);
    u64 newOffset = mtb_add_u64(oldOffsetAligned, size);
    mtb_assert_always(newOffset <= arena->size);
    arena->offset = newOffset;

    u8 *result = arena->base + oldOffsetAligned;

    return opt.no_zero ? result : memset(result, 0, size);
}

func void
mtb_arena_clear(MtbArena *arena)
{
    arena->offset = 0;
}

#endif // MTB_ARENA_IMPLEMENTATION


#ifdef MTB_ARENA_TESTS

#include <assert.h>


func void
_test_mtb_allocator(MtbArenaAllocator *allocator)
{
    MtbArena arena = {0};
    u64 pageSize = (u64)sysconf(_SC_PAGE_SIZE);
    u64 arenaSize = 2 * pageSize;

    // init
    mtb_arena_init(&arena, arenaSize, allocator);
    assert(arena.size == arenaSize);
    assert(arena.allocator->size(arena.allocator->ctx, arena.base) >= arenaSize);

    // alloc
    u64 *tiny = mtb_arena_bump(&arena, u64, 1);
    assert(tiny != nil);
    assert(*tiny == 0);       // is zeroed out
    *tiny = U64_MAX;          // is writable
    assert(*tiny == U64_MAX); // is readable

    // page alloc
    u8 *page = mtb_arena_bump(&arena, u8, pageSize, .align = pageSize);
    assert(page != 0);
    assert((u64)page % pageSize == 0); // aligned to page boundary
    for (u32 i = 0; i < pageSize; i++) assert(page[i] == 0);      // is zeroed out
    for (u32 i = 0; i < pageSize; i++) page[i] = U8_MAX;          // is writable
    for (u32 i = 0; i < pageSize; i++) assert(page[i] == U8_MAX); // is readable

    // clear
    assert(arena.offset != 0);
    mtb_arena_clear(&arena);
    assert(arena.offset == 0);

    // deinit
    assert(mtb_arena_bump(&arena, u32, 1) != nil);
    assert(arena.base != nil);
    assert(arena.offset != 0);
    assert(arena.size != 0);
    assert(arena.allocator != nil);
    mtb_arena_deinit(&arena);
    assert(arena.base == nil);
    assert(arena.offset == 0);
    assert(arena.size == 0);
    assert(arena.allocator == nil);
}

func void
_test_mtb_def_virt_allocator(void)
{
    _test_mtb_allocator(&MTB_ARENA_DEF_VIRT_ALLOCATOR);
}

func void
_test_mtb_def_allocator(void)
{
    _test_mtb_allocator(&MTB_ARENA_DEF_ALLOCATOR);
}

func void
_test_mtb_arena(void)
{
    _test_mtb_def_virt_allocator();
    _test_mtb_def_allocator();
}

#endif // MTB_ARENA_TESTS
#ifndef MTB_DYNARR_H
#define MTB_DYNARR_H

#ifdef MTB_IMPLEMENTATION
#define MTB_DYNARR_IMPLEMENTATION
#endif

#ifdef MTB_TESTS
#define MTB_DYNARR_TESTS
#endif


typedef struct mtb_dynarr MtbDynArr;
struct mtb_dynarr
{
    MtbArena *arena;
    u8 *items;
    u64 itemSize;
    u64 length;
    u64 capacity;
};


/* Dynamic Array API */

#define _mtb_dynarr_foreach(array, var, _array, _end) \
    __auto_type _array = (array); \
    for (void *_end = _array->items + _array->length * _array->itemSize, *var = _array->items; var < _end; var += _array->itemSize)
#define mtb_dynarr_foreach(array, var) _mtb_dynarr_foreach(array, var, mtb_id(_array), mtb_id(_end))

func void mtb_dynarr_init(MtbDynArr *array, MtbArena *arena, u64 itemSize);
func void mtb_dynarr_grow(MtbDynArr *array, u64 capacity);
func bool mtb_dynarr_is_empty(MtbDynArr *array);
func void mtb_dynarr_clear(MtbDynArr *array);

func void *mtb_dynarr_insert_n(MtbDynArr *array, u64 begIndex, u64 n);
func void *mtb_dynarr_insert(MtbDynArr *array, u64 index);
func void *mtb_dynarr_remove_n(MtbDynArr *array, u64 begIndex, u64 n);
func void *mtb_dynarr_remove(MtbDynArr *array, u64 index);
func void *mtb_dynarr_get(MtbDynArr *array, u64 index);

func void *mtb_dynarr_copy_n(MtbDynArr *array, u64 begIndex, void *src, u64 n);


/* Stack API */

func void *mtb_dynarr_push(MtbDynArr *array);
func void *mtb_dynarr_pop(MtbDynArr *array);
func void *mtb_dynarr_top(MtbDynArr *array);


/* Queue API */

func void *mtb_dynarr_enq(MtbDynArr *array);
func void *mtb_dynarr_deq(MtbDynArr *array);
func void *mtb_dynarr_front(MtbDynArr *array);


/* Iterator API */

typedef struct mtb_dynarr_iter MtbDynArrIter;
struct mtb_dynarr_iter
{
    MtbDynArr *array;
    u64 index;
};


func void mtb_dynarr_iter_init(MtbDynArrIter *it, MtbDynArr *array);
func void mtb_dynarr_iter_reset(MtbDynArrIter *it);
func bool mtb_dynarr_iter_has_next(MtbDynArrIter *it);
func void *mtb_dynarr_iter_next(MtbDynArrIter *it);


#endif //MTB_DYNARR_H


#ifdef MTB_DYNARR_IMPLEMENTATION

#include <string.h>


func void
mtb_dynarr_init(MtbDynArr *array, MtbArena *arena, u64 itemSize)
{
    mtb_assert_always(itemSize > 0);

    array->arena = arena;
    array->items = nil;
    array->itemSize = itemSize;
    array->length = 0;
    array->capacity = 0;
}

func void
mtb_dynarr_grow(MtbDynArr *array, u64 capacity)
{
    mtb_assert_always(capacity > array->capacity);

    u8 *oldItems = array->items;
    array->items = mtb_arena_bump(array->arena, u8, capacity);
    if (!mtb_dynarr_is_empty(array)) {
        memmove(array->items, oldItems, array->length * array->itemSize);
    }
    array->capacity = capacity;
}

func bool
mtb_dynarr_is_empty(MtbDynArr *array)
{
    return array->length == 0;
}

func void
mtb_dynarr_clear(MtbDynArr *array)
{
    array->length = 0;
}

func void *
mtb_dynarr_insert_n(MtbDynArr *array, u64 begIndex, u64 n)
{
    mtb_assert_always(begIndex <= array->length);
    mtb_assert_always(n > 0);

    u64 capacity = array->capacity;
    u64 minCapacity = mtb_mul_u64(mtb_add_u64(array->length, n), array->itemSize);
    if (capacity < minCapacity) {
        capacity += capacity >> 1;
        mtb_dynarr_grow(array, mtb_max_u64(capacity, minCapacity));
    }

    u8 *beg = array->items + begIndex * array->itemSize;
    if (begIndex < array->length) {
        memmove(beg + n * array->itemSize, beg, (array->length - begIndex) * array->itemSize);
    }
    array->length += n;

    return beg;
}

func void *
mtb_dynarr_insert(MtbDynArr *array, u64 index)
{
    return mtb_dynarr_insert_n(array, index, 1);
}

func void *
mtb_dynarr_remove_n(MtbDynArr *array, u64 begIndex, u64 n)
{
    mtb_assert_always(begIndex < array->length);
    mtb_assert_always(n > 0);

    u64 endIndex = mtb_add_u64(begIndex, n);
    mtb_assert_always(endIndex <= array->length);

    u8 *beg = array->items + begIndex * array->itemSize;
    if (endIndex < array->length) {
        memmove(beg, array->items + endIndex * array->itemSize, (array->length - endIndex) * array->itemSize);
    }
    array->length -= n;

    return beg;
}

func void *
mtb_dynarr_remove(MtbDynArr *array, u64 index)
{
    return mtb_dynarr_remove_n(array, index, 1);
}

func void *
mtb_dynarr_get(MtbDynArr *array, u64 index)
{
    mtb_assert_always(index < array->length);
    return array->items + index * array->itemSize;
}

func void *
mtb_dynarr_copy_n(MtbDynArr *array, u64 begIndex, void *src, u64 n)
{
    return memmove(mtb_dynarr_insert_n(array, begIndex, n), src, n * array->itemSize);
}

func void *
mtb_dynarr_push(MtbDynArr *array)
{
    return mtb_dynarr_insert(array, array->length);
}

func void *
mtb_dynarr_pop(MtbDynArr *array)
{
    return mtb_dynarr_remove(array, array->length - 1);
}

func void *
mtb_dynarr_top(MtbDynArr *array)
{
    return mtb_dynarr_get(array, array->length - 1);
}

func void *
mtb_dynarr_enq(MtbDynArr *array)
{
    return mtb_dynarr_insert(array, 0);
}

func void *
mtb_dynarr_deq(MtbDynArr *array)
{
    return mtb_dynarr_remove(array, array->length - 1);
}

func void *
mtb_dynarr_front(MtbDynArr *array)
{
    return mtb_dynarr_get(array, array->length - 1);
}

func void
mtb_dynarr_iter_init(MtbDynArrIter *it, MtbDynArr *array)
{
    it->array = array;
    mtb_dynarr_iter_reset(it);
}

func void
mtb_dynarr_iter_reset(MtbDynArrIter *it)
{
    it->index = 0;
}

func bool
mtb_dynarr_iter_has_next(MtbDynArrIter *it)
{
    return it->index < it->array->length;
}

func void *
mtb_dynarr_iter_next(MtbDynArrIter *it)
{
    return mtb_dynarr_get(it->array, it->index++);
}

#endif //MTB_DYNARR_IMPLEMENTATION


#ifdef MTB_DYNARR_TESTS

#include <assert.h>


func void
_test_mtb_dynarr_insert(MtbArena arena)
{
    MtbDynArr array = {0};
    mtb_dynarr_init(&array, &arena, sizeof(u64));
    assert(mtb_dynarr_is_empty(&array));
    assert(array.itemSize == sizeof(u64));

    u64 init[] = { 3 };
    mtb_dynarr_copy_n(&array, 0, init, mtb_countof(init));
    assert(!mtb_dynarr_is_empty(&array));

    u64 prefix[] = { 0, 1, 2 };
    u64 prefixCount = mtb_countof(prefix);
    void *prefixBeg = mtb_dynarr_insert_n(&array, 0, prefixCount);
    memcpy(prefixBeg, prefix, prefixCount * sizeof(*prefix));
    assert(memcmp(mtb_dynarr_get(&array, 0), (u64[]){ 0, 1, 2, 3 }, array.length * array.itemSize) == 0);

    u64 suffix[] = { 7, 8, 9 };
    u64 suffixCount = mtb_countof(suffix);
    void *suffixBeg = mtb_dynarr_insert_n(&array, array.length, suffixCount);
    memcpy(suffixBeg, suffix, suffixCount * sizeof(*suffix));
    assert(memcmp(mtb_dynarr_get(&array, 0), (u64[]){ 0, 1, 2, 3, 7, 8, 9 }, array.length * array.itemSize) == 0);

    u64 middle[] = { 4, 5, 6 };
    u64 middleCount = mtb_countof(middle);
    void *middleBeg = mtb_dynarr_insert_n(&array, 4, middleCount);
    memcpy(middleBeg, middle, middleCount * sizeof(*middle));
    assert(memcmp(mtb_dynarr_get(&array, 0), (u64[]){ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, array.length * array.itemSize) == 0);

    u64 i = 0;
    mtb_dynarr_foreach(&array, item) {
        *(u64 *)item = i++;
    }
    assert(i == array.length);

    mtb_dynarr_clear(&array);
    assert(mtb_dynarr_is_empty(&array));
}

func void
_test_mtb_dynarr_remove(MtbArena arena)
{
    MtbDynArr array = {0};
    mtb_dynarr_init(&array, &arena, sizeof(u64));
    assert(mtb_dynarr_is_empty(&array));
    assert(array.itemSize == sizeof(u64));

    u64 init[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    mtb_dynarr_copy_n(&array, 0, init, mtb_countof(init));
    assert(!mtb_dynarr_is_empty(&array));

    u64 prefix[] = { 0, 1, 2 };
    u64 prefixCount = mtb_countof(prefix);
    mtb_dynarr_remove_n(&array, 0, prefixCount);
    assert(memcmp(mtb_dynarr_get(&array, 0), (u64[]){ 3, 4, 5, 6, 7, 8, 9 }, array.length * array.itemSize) == 0);

    u64 suffix[] = { 7, 8, 9 };
    u64 suffixCount = mtb_countof(suffix);
    mtb_dynarr_remove_n(&array, 4, suffixCount);
    assert(memcmp(mtb_dynarr_get(&array, 0), (u64[]){ 3, 4, 5, 6 }, array.length * array.itemSize) == 0);

    u64 middle[] = { 4, 5, 6 };
    u64 middleCount = mtb_countof(middle);
    mtb_dynarr_remove_n(&array, 1, middleCount);
    assert(memcmp(mtb_dynarr_get(&array, 0), (u64[]){ 3 }, array.length * array.itemSize) == 0);

    mtb_dynarr_clear(&array);
    assert(mtb_dynarr_is_empty(&array));
}

func void
_test_mtb_dynarr_grow(MtbArena arena)
{
    MtbDynArr array = {0};
    mtb_dynarr_init(&array, &arena, sizeof(u16));
    assert(mtb_dynarr_is_empty(&array));
    assert(array.capacity == 0);
    assert(array.itemSize == sizeof(u16));

    u64 count = 10;
    mtb_dynarr_grow(&array, count * array.itemSize);
    assert(mtb_dynarr_is_empty(&array));
    assert(array.capacity == count * array.itemSize);
    assert(array.length == 0);

    mtb_dynarr_insert_n(&array, 0, count);
    assert(array.capacity == count * array.itemSize);
    assert(array.length == count);

    mtb_dynarr_insert(&array, array.length);
    assert(array.capacity >= (count + 1) * array.itemSize);
    assert(array.length == count + 1);

    mtb_dynarr_clear(&array);
    assert(array.capacity >= (count + 1) * array.itemSize);
    assert(mtb_dynarr_is_empty(&array));
}

func void
_test_mtb_dynarr_stack(MtbArena arena)
{
    MtbDynArr stack = {0};
    mtb_dynarr_init(&stack, &arena, sizeof(u32));
    assert(mtb_dynarr_is_empty(&stack));
    assert(stack.itemSize == sizeof(u32));

    *(u32 *)mtb_dynarr_push(&stack) = 1;
    *(u32 *)mtb_dynarr_push(&stack) = 2;
    *(u32 *)mtb_dynarr_push(&stack) = 3;

    assert(*(u32 *)mtb_dynarr_top(&stack) == 3);
    assert(*(u32 *)mtb_dynarr_pop(&stack) == 3);
    assert(*(u32 *)mtb_dynarr_top(&stack) == 2);
    assert(*(u32 *)mtb_dynarr_pop(&stack) == 2);
    assert(*(u32 *)mtb_dynarr_top(&stack) == 1);
    assert(*(u32 *)mtb_dynarr_pop(&stack) == 1);

    assert(mtb_dynarr_is_empty(&stack));
}

func void
_test_mtb_dynarr_queue(MtbArena arena)
{
    MtbDynArr queue = {0};
    mtb_dynarr_init(&queue, &arena, sizeof(u32));
    assert(mtb_dynarr_is_empty(&queue));
    assert(queue.itemSize == sizeof(u32));

    *(u32 *)mtb_dynarr_enq(&queue) = 1;
    *(u32 *)mtb_dynarr_enq(&queue) = 2;
    *(u32 *)mtb_dynarr_enq(&queue) = 3;

    assert(*(u32 *)mtb_dynarr_front(&queue) == 1);
    assert(*(u32 *)mtb_dynarr_deq(&queue) == 1);
    assert(*(u32 *)mtb_dynarr_front(&queue) == 2);
    assert(*(u32 *)mtb_dynarr_deq(&queue) == 2);
    assert(*(u32 *)mtb_dynarr_front(&queue) == 3);
    assert(*(u32 *)mtb_dynarr_deq(&queue) == 3);

    assert(mtb_dynarr_is_empty(&queue));
}

func void
_test_mtb_dynarr_iter(MtbArena arena)
{
    MtbDynArr array = {0};
    mtb_dynarr_init(&array, &arena, sizeof(u32));
    assert(mtb_dynarr_is_empty(&array));

    MtbDynArrIter it = {0};
    mtb_dynarr_iter_init(&it, &array);
    assert(!mtb_dynarr_iter_has_next(&it));

    u32 n = 10;
    for (u32 i = 0; i < n; i++) {
        *(u32 *)mtb_dynarr_push(&array) = i;
    }

    mtb_dynarr_iter_reset(&it);
    assert(mtb_dynarr_iter_has_next(&it));

    u32 i = 0;
    while (mtb_dynarr_iter_has_next(&it)) {
        assert(*(u32 *)mtb_dynarr_iter_next(&it) == i++);
    }
}

func void
_test_mtb_dynarr(void)
{
    MtbArena arena = {0};
    mtb_arena_init(&arena, kb(1), &MTB_ARENA_DEF_ALLOCATOR);

    _test_mtb_dynarr_insert(arena);
    _test_mtb_dynarr_remove(arena);
    _test_mtb_dynarr_grow(arena);
    _test_mtb_dynarr_stack(arena);
    _test_mtb_dynarr_queue(arena);
    _test_mtb_dynarr_iter(arena);

    mtb_arena_deinit(&arena);
}

#endif // MTB_DYNARR_TESTS
#ifndef MTB_SEGARR_H
#define MTB_SEGARR_H

#ifdef MTB_IMPLEMENTATION
#define MTB_SEGARR_IMPLEMENTATION
#endif

#ifdef MTB_TESTS
#define MTB_SEGARR_TESTS
#endif


#ifndef MTB_SEGARR_SKIP_SEGMENTS
#define MTB_SEGARR_SKIP_SEGMENTS 4
#endif
#ifndef MTB_SEGARR_MAX_SEGMENTS
#define MTB_SEGARR_MAX_SEGMENTS (48 - MTB_SEGARR_SKIP_SEGMENTS)
#endif


// SEG |       LENGTH        |        START        |        END
//-----+---------------------+---------------------+--------------------
//   0 |                  16 |                   0 |                  15
//   1 |                  32 |                  16 |                  47
//   2 |                  64 |                  48 |                 111
//   3 |                 128 |                 112 |                 239
//   4 |                 256 |                 240 |                 495
//   5 |                 512 |                 496 |               1'007
//   6 |               1'024 |               1'008 |               2'031
//   7 |               2'048 |               2'032 |               4'079
//   8 |               4'096 |               4'080 |               8'175
//   9 |               8'192 |               8'176 |              16'367
//  10 |              16'384 |              16'368 |              32'751
//  11 |              32'768 |              32'752 |              65'519
//  12 |              65'536 |              65'520 |             131'055
//  13 |             131'072 |             131'056 |             262'127
//  14 |             262'144 |             262'128 |             524'271
//  15 |             524'288 |             524'272 |           1'048'559
//  16 |           1'048'576 |           1'048'560 |           2'097'135
//  17 |           2'097'152 |           2'097'136 |           4'194'287
//  18 |           4'194'304 |           4'194'288 |           8'388'591
//  19 |           8'388'608 |           8'388'592 |          16'777'199
//  20 |          16'777'216 |          16'777'200 |          33'554'415
//  21 |          33'554'432 |          33'554'416 |          67'108'847
//  22 |          67'108'864 |          67'108'848 |         134'217'711
//  23 |         134'217'728 |         134'217'712 |         268'435'439
//  24 |         268'435'456 |         268'435'440 |         536'870'895
//  25 |         536'870'912 |         536'870'896 |       1'073'741'807
//  26 |       1'073'741'824 |       1'073'741'808 |       2'147'483'631
//  27 |       2'147'483'648 |       2'147'483'632 |       4'294'967'279
//  28 |       4'294'967'296 |       4'294'967'280 |       8'589'934'575
//  29 |       8'589'934'592 |       8'589'934'576 |      17'179'869'167
//  30 |      17'179'869'184 |      17'179'869'168 |      34'359'738'351
//  31 |      34'359'738'368 |      34'359'738'352 |      68'719'476'719
//  32 |      68'719'476'736 |      68'719'476'720 |     137'438'953'455
//  33 |     137'438'953'472 |     137'438'953'456 |     274'877'906'927
//  34 |     274'877'906'944 |     274'877'906'928 |     549'755'813'871
//  35 |     549'755'813'888 |     549'755'813'872 |   1'099'511'627'759
//  36 |   1'099'511'627'776 |   1'099'511'627'760 |   2'199'023'255'535
//  37 |   2'199'023'255'552 |   2'199'023'255'536 |   4'398'046'511'087
//  38 |   4'398'046'511'104 |   4'398'046'511'088 |   8'796'093'022'191
//  39 |   8'796'093'022'208 |   8'796'093'022'192 |  17'592'186'044'399
//  40 |  17'592'186'044'416 |  17'592'186'044'400 |  35'184'372'088'815
//  41 |  35'184'372'088'832 |  35'184'372'088'816 |  70'368'744'177'647
//  42 |  70'368'744'177'664 |  70'368'744'177'648 | 140'737'488'355'311
//  43 | 140'737'488'355'328 | 140'737'488'355'312 | 281'474'976'710'639
typedef struct mtb_segarr MtbSegArr;
struct mtb_segarr
{
    MtbArena *arena;
    u8 *segments[MTB_SEGARR_MAX_SEGMENTS];
    u64 count;
    u64 itemSize;
};


/* Segment Array API */

func void mtb_segarr_init(MtbSegArr *array, MtbArena *arena, u64 itemSize);
func void mtb_segarr_clear(MtbSegArr *array);
func bool mtb_segarr_is_empty(MtbSegArr *array);

func void *mtb_segarr_add_last(MtbSegArr *array);
func void *mtb_segarr_remove_last(MtbSegArr *array);
func void *mtb_segarr_get(MtbSegArr *array, u64 index);

func void mtb_segarr_add_last_n(MtbSegArr *array, void *src, u64 n);


/* Stack API */

func void *mtb_segarr_push(MtbSegArr *array);
func void *mtb_segarr_pop(MtbSegArr *array);
func void *mtb_segarr_top(MtbSegArr *array);


/* Iterator API */

typedef struct mtb_segarr_iter MtbSegArrIter;
struct mtb_segarr_iter
{
    MtbSegArr *array;
    u64 index;
};


func void mtb_segarr_iter_init(MtbSegArrIter *it, MtbSegArr *array);
func void mtb_segarr_iter_reset(MtbSegArrIter *it);
func bool mtb_segarr_iter_has_next(MtbSegArrIter *it);
func void *mtb_segarr_iter_next(MtbSegArrIter *it);

#endif //MTB_SEGARR_H


#ifdef MTB_SEGARR_IMPLEMENTATION

#define _mtb_segarr_segment(i) (u64_lit(63) - mtb_leading_zeros_count((i >> MTB_SEGARR_SKIP_SEGMENTS) + u64_lit(1)))
#define _mtb_segarr_segment_length(s) ((u64_lit(1) << MTB_SEGARR_SKIP_SEGMENTS) << (s))
#define _mtb_segarr_segment_start(s) (_mtb_segarr_segment_length(s) - (u64_lit(1) << MTB_SEGARR_SKIP_SEGMENTS))
#define _mtb_segarr_item(s, i) (i - _mtb_segarr_segment_start(s))


func void
mtb_segarr_init(MtbSegArr *array, MtbArena *arena, u64 itemSize)
{
    array->arena = arena;
    array->itemSize = itemSize;
    array->count = 0;
}

func void
mtb_segarr_clear(MtbSegArr *array)
{
    array->count = 0;
}

func bool
mtb_segarr_is_empty(MtbSegArr *array)
{
    return array->count == 0;
}

func void *
mtb_segarr_add_last(MtbSegArr *array)
{
    u64 index = array->count++;
    u64 segment = _mtb_segarr_segment(index);
    u64 item = _mtb_segarr_item(segment, index);
    if (array->segments[segment] == nil) {
        u64 length = _mtb_segarr_segment_length(segment);
        u64 size = mtb_mul_u64(length, array->itemSize);
        array->segments[segment] = mtb_arena_bump(array->arena, u8, size);
    }
    return array->segments[segment] + item * array->itemSize;
}

func void *
mtb_segarr_remove_last(MtbSegArr *array)
{
    mtb_assert_always(array->count > 0);
    u64 index = --array->count;
    u64 segment = _mtb_segarr_segment(index);
    u64 item = _mtb_segarr_item(segment, index);
    return array->segments[segment] + item * array->itemSize;
}

func void *
mtb_segarr_get(MtbSegArr *array, u64 index)
{
    mtb_assert_always(index < array->count);
    u64 segment = _mtb_segarr_segment(index);
    u64 item = _mtb_segarr_item(segment, index);
    return array->segments[segment] + item * array->itemSize;
}

func void
mtb_segarr_add_last_n(MtbSegArr *array, void *src, u64 n)
{
    for (u64 i = 0; i < n; i++) {
        memmove(mtb_segarr_add_last(array), src + i * array->itemSize, array->itemSize);
    }
}

func void *
mtb_segarr_push(MtbSegArr *array)
{
    return mtb_segarr_add_last(array);
}

func void *
mtb_segarr_pop(MtbSegArr *array)
{
    return mtb_segarr_remove_last(array);
}

func void *
mtb_segarr_top(MtbSegArr *array)
{
    return mtb_segarr_get(array, array->count - 1);
}

func void
mtb_segarr_iter_init(MtbSegArrIter *it, MtbSegArr *array)
{
    it->array = array;
    mtb_segarr_iter_reset(it);
}

func void
mtb_segarr_iter_reset(MtbSegArrIter *it)
{
    it->index = 0;
}

func bool
mtb_segarr_iter_has_next(MtbSegArrIter *it)
{
    return it->index < it->array->count;
}

func void *
mtb_segarr_iter_next(MtbSegArrIter *it)
{
    return mtb_segarr_get(it->array, it->index++);
}

#endif // MTB_SEGARR_IMPLEMENTATION


#ifdef MTB_SEGARR_TESTS

#include <assert.h>


func void
_test_mtb_segarr_add_last(MtbArena arena)
{
    MtbSegArr array = {0};
    mtb_segarr_init(&array, &arena, sizeof(u64));

    u64 n = 100000;
    for (u64 i = 0; i < n; i++) {
        *(u64 *)mtb_segarr_add_last(&array) = i;
    }
    for (u64 i = 0; i < n; i++) {
        assert(*(u64 *)mtb_segarr_get(&array, i) == i);
    }
    mtb_segarr_clear(&array);
    assert(mtb_segarr_is_empty(&array));
}

func void
_test_mtb_segarr_remove_last(MtbArena arena)
{
    MtbSegArr array = {0};
    mtb_segarr_init(&array, &arena, sizeof(u64));

    u64 n = 100000;
    for (u64 i = 0; i < n; i++) {
        *(u64 *)mtb_segarr_add_last(&array) = i;
    }
    for (u64 i = n; i > 0 ; i--) {
        assert(*(u64 *)mtb_segarr_remove_last(&array) == (i - 1));
    }
    assert(mtb_segarr_is_empty(&array));
}

func void
_test_mtb_segarr_add_last_n(MtbArena arena)
{
    MtbSegArr array = {0};
    mtb_segarr_init(&array, &arena, sizeof(u64));

    u64 n = 100000;
    u64 *arr = mtb_arena_bump(&arena, u64, n);
    for (u64 i = 0; i < n; i++) {
        arr[i] = n - i - 1;
    }
    mtb_segarr_add_last_n(&array, arr, n);
    for (u64 i = 0; i < n; i++) {
        assert(*(u64 *)mtb_segarr_get(&array, i) == arr[i]);
    }
    mtb_segarr_clear(&array);
    assert(mtb_segarr_is_empty(&array));
}

func void
_test_mtb_segarr_stack(MtbArena arena)
{
    MtbSegArr array = {0};
    mtb_segarr_init(&array, &arena, sizeof(u64));

    u64 n = 100000;
    for (u64 i = 0; n > i; i++) {
        *(u64 *)mtb_segarr_push(&array) = i;
    }
    for (u64 i = n; i > 0 ; i--) {
        assert(*(u64 *)mtb_segarr_top(&array) == (i - 1));
        assert(*(u64 *)mtb_segarr_pop(&array) == (i - 1));
    }
    assert(mtb_segarr_is_empty(&array));
}

func void
_test_mtb_segarr_iter(MtbArena arena)
{
    MtbSegArr array = {0};
    mtb_segarr_init(&array, &arena, sizeof(u64));

    MtbSegArrIter it = {0};
    mtb_segarr_iter_init(&it, &array);

    assert(!mtb_segarr_iter_has_next(&it));

    u64 n = 100000;
    for (u64 i = 0; n > i; i++) {
        *(u64 *)mtb_segarr_push(&array) = i;
    }

    for (int i = 0; i < 5; i++) {
        mtb_segarr_iter_reset(&it);

        u64 k = 0;
        while (mtb_segarr_iter_has_next(&it)) {
            assert(*(u64 *)mtb_segarr_iter_next(&it) == k++);
        }
    }

    mtb_segarr_clear(&array);
    assert(mtb_segarr_is_empty(&array));
}

func void
_test_mtb_segarr(void)
{
    MtbArena arena = {0};
    mtb_arena_init(&arena, mb(10), &MTB_ARENA_DEF_ALLOCATOR);

    _test_mtb_segarr_add_last(arena);
    _test_mtb_segarr_remove_last(arena);
    _test_mtb_segarr_add_last_n(arena);
    _test_mtb_segarr_stack(arena);
    _test_mtb_segarr_iter(arena);

    mtb_arena_deinit(&arena);
}

#endif // MTB_SEGARR_TESTS
#ifndef MTB_HMAP_H
#define MTB_HMAP_H

#ifdef MTB_IMPLEMENTATION
#define MTB_HMAP_IMPLEMENTATION
#endif

#ifdef MTB_TESTS
#define MTB_HMAP_TESTS
#endif

#ifdef MTB_BENCH
#define MTB_HMAP_BENCH
#endif


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


func void mtb_hmap_init_opt(MtbHmap *hmap,
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
func void mtb_hmap_clear(MtbHmap *hmap);
func bool mtb_hmap_is_empty(MtbHmap *hmap);
func void mtb_hmap_grow(MtbHmap *hmap, u64 capacity);
func u64 mtb_hmap_calc_capacity(u64 n);

func void *mtb_hmap_put(MtbHmap *hmap, void *key);
func void *mtb_hmap_remove(MtbHmap *hmap, void *key);
func void *mtb_hmap_get(MtbHmap *hmap, void *key);


/* Iterator API */

typedef struct mtb_hmap_iter MtbHmapIter;
struct mtb_hmap_iter
{
    MtbHmap *hmap;
    u8 *prev;
    u8 *next;
};


func void mtb_hmap_iter_init(MtbHmapIter *it, MtbHmap *hmap);
func void mtb_hmap_iter_reset(MtbHmapIter *it);
func bool mtb_hmap_iter_has_next(MtbHmapIter *it);
func void *mtb_hmap_iter_next(MtbHmapIter *it);
func void *mtb_hmap_iter_next_key(MtbHmapIter *it);
func void *mtb_hmap_iter_next_value(MtbHmapIter *it);
func void *mtb_hmap_iter_remove(MtbHmapIter *it);

#endif //MTB_HMAP_H


#ifdef MTB_HMAP_IMPLEMENTATION

#define _mtb_hmap_threshold(capacity) ((capacity) - ((capacity) >> 2)) // 0.75 * capacity
#define _mtb_hmap_modulo_capacity(hmap, n) ((n) & ((hmap)->capacity - 1))


func void
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

func void
mtb_hmap_clear(MtbHmap *hmap)
{
    hmap->count = 0;
    memset(hmap->entries, 0, hmap->capacity * hmap->entrySize);
}

func bool
mtb_hmap_is_empty(MtbHmap *hmap)
{
    return hmap->count == 0;
}

func u64
mtb_hmap_calc_capacity(u64 n)
{
    u64 capacity = n + (n / 3);
    if (capacity < MTB_HMAP_MIN_CAPACITY) {
        return MTB_HMAP_MIN_CAPACITY;
    }
    return mtb_roundup_pow2(capacity);
}

func void
mtb_hmap_grow(MtbHmap *hmap, u64 capacity)
{
    mtb_perf_time_func();
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

func void *
mtb_hmap_put(MtbHmap *hmap, void *key)
{
    mtb_perf_time_func();
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

func void *
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

func void *
mtb_hmap_get(MtbHmap *hmap, void *key)
{
    mtb_perf_time_func();
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

func void
mtb_hmap_iter_init(MtbHmapIter *it, MtbHmap *hmap)
{
    it->hmap = hmap;
    mtb_hmap_iter_reset(it);
}

func void
mtb_hmap_iter_reset(MtbHmapIter *it)
{
    it->prev = nil;
    it->next = nil;
}

func bool
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

func void *
mtb_hmap_iter_next(MtbHmapIter *it)
{
    mtb_assert_always(it->next != nil);
    it->prev = it->next;
    it->next = nil;
    return it->prev;
}

func void *
mtb_hmap_iter_next_key(MtbHmapIter *it)
{
    return mtb_hmap_entry_key(it->hmap, mtb_hmap_iter_next(it));
}

func void *
mtb_hmap_iter_next_value(MtbHmapIter *it)
{
    return mtb_hmap_entry_value(it->hmap, mtb_hmap_iter_next(it));
}

func void *
mtb_hmap_iter_remove(MtbHmapIter *it)
{
    mtb_assert_always(it->prev != nil);
    mtb_hmap_entry_header(it->hmap, it->prev)->status = MTB_HMAP_ENTRY_REMOVED;
    it->hmap->count--;
    return it->prev;
}

#endif // MTB_HMAP_IMPLEMENTATION


#ifdef MTB_HMAP_TESTS

#include <assert.h>


func u64
_calc_hash_str(void *key)
{
    char *s = *(char **)key;
    u64 hash = 0;
    while (*s != '\0') hash = 17000069 * hash + *s++;
    return hash;
}

func bool
_is_equal_str(void *key1, void *key2)
{
    char *s1 = *(char **)key1;
    char *s2 = *(char **)key2;
    return strcmp(s1, s2) == 0;
}

func void
_test_mtb_hmap_calc_capacity(void)
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

func void
_test_mtb_hmap_put(MtbArena arena)
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
    mtb_hmap_init(&hmap, &arena, char *, u64, _calc_hash_str, _is_equal_str);
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

func void
_test_mtb_hmap_remove(MtbArena arena)
{
    MtbHmap hmap = {0};
    mtb_hmap_init(&hmap, &arena, sizeof(char *), sizeof(u64), _calc_hash_str, _is_equal_str);

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

func void
_test_mtb_hmap_iter(MtbArena arena)
{
    MtbHmap hmap = {0};
    mtb_hmap_init(&hmap, &arena, sizeof(char *), sizeof(u64), _calc_hash_str, _is_equal_str);

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

func void
_test_mtb_hmap(void)
{
    MtbArena arena = {0};
    mtb_arena_init(&arena, mb(1), &MTB_ARENA_DEF_ALLOCATOR);

    _test_mtb_hmap_put(arena);
    _test_mtb_hmap_remove(arena);
    _test_mtb_hmap_calc_capacity();
    _test_mtb_hmap_iter(arena);

    mtb_arena_deinit(&arena);
}

#endif // MTB_HMAP_TESTS


#ifdef MTB_HMAP_BENCH

#include <assert.h>


func u64
_calc_hash_str(void *key)
{
    char *s = *(char **)key;
    u64 hash = 0;
    while (*s != '\0') hash = 17000069 * hash + *s++;
    return hash;
}

func bool
_is_equal_str(void *key1, void *key2)
{
    char *s1 = *(char **)key1;
    char *s2 = *(char **)key2;
    return strcmp(s1, s2) == 0;
}

func void
_bench_mtb_hmap(void)
{
    mtb_perf_start();

    FILE *inputFile = fopen("data/shakespeare.txt", "r");
    fseek(inputFile, 0L, SEEK_END);
    u64 inputFileSize = ftell(inputFile);
    fseek(inputFile, 0L, SEEK_SET);

    MtbArena arena = {0};
    mtb_arena_init(&arena, inputFileSize * 10, &MTB_ARENA_DEF_ALLOCATOR);

    char *inputData = mtb_arena_bump(&arena, char, inputFileSize + 1);
    if (fread(inputData, sizeof(char), inputFileSize, inputFile) != inputFileSize) {
        fprintf(stderr, "fread() failed\n");
        exit(1);
    }
    fclose(inputFile);

    MtbDynArr tokens = {0};
    mtb_dynarr_init(&tokens, &arena, sizeof(char *));
    char *token = strtok(inputData, " \t\r\n");
    while (token != nil) {
        *(char **)mtb_dynarr_push(&tokens) = token;
        token = strtok(nil, " \n");
    }

    i32 iterationCount = 100;
    for (i32 i = 0; i < iterationCount; i++) {
        MtbArena arenaTmp = arena;

        MtbHmap hmap = {0};
        mtb_hmap_init(&hmap, &arenaTmp, char *, u64, _calc_hash_str, _is_equal_str);

        MtbDynArrIter tokensIterator = {0};
        mtb_dynarr_iter_init(&tokensIterator, &tokens);

        while (mtb_dynarr_iter_has_next(&tokensIterator)) {
            char *token = *(char **)mtb_dynarr_iter_next(&tokensIterator);
            u64 *count = mtb_hmap_get(&hmap, &token);
            if (count == nil) {
                *(u64 *)mtb_hmap_put(&hmap, &token) = 1;
            }
            else {
                *count += 1;
            }
        }

        // sanity check
        struct histogram {
            char *word;
            u64 count;
        } hist[] = {
            { .word = "god", .count = 62 },
            { .word = "dog", .count = 60 },
            { .word = "duck", .count = 2 },
            { .word = "poop", .count = 1 },
            { .word = "love", .count = 1364 },
        };
        for (u64 i = 0; i < mtb_countof(hist); i++) {
            struct histogram *wc = hist + i;
            assert(*(u64 *)mtb_hmap_get(&hmap, &wc->word) == wc->count);
        }
    }
    mtb_perf_print();

    mtb_arena_deinit(&arena);
}

#endif // MTB_HMAP_BENCH
#ifndef MTB_STRING_H
#define MTB_STRING_H

#ifdef MTB_IMPLEMENTATION
#define MTB_STRING_IMPLEMENTATION
#endif

#ifdef MTB_TESTS
#define MTB_STRING_TESTS
#endif


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

#include <stdio.h>
#include <string.h>


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

#endif //MTB_H
