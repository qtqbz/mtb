#ifndef MTB_TYPE_H
#define MTB_TYPE_H

#include <stdint.h>
#include <stdbool.h>

#include "mtb_macro.h"


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

global u64 U64_MAXM = UINT64_MAX;


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

#endif // MTB_TYPE_H
