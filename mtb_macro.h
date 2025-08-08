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

#define intern static
#define global static __attribute__((unused))
#ifndef MTB_NOSTU
#define public intern
#elif
#define public
#endif

#define nil ((void *)0)


/* Macro Helpers */

#define _mtb_stringify(s) #s
#define mtb_stringify(s) _mtb_stringify(s)

#define _mtb_cat(a, b) a##b
#define mtb_cat(a, b) _mtb_cat(a, b)

#define mtb_id(a) mtb_cat(a, __COUNTER__)


/* Array Helpers */

#define mtb_countof(a) (sizeof(a) / sizeof(*(a)))
#define mtb_lengthof(s) (mtb_countof(s) - 1)


/* Arith Helpers */

#define mtb_is_pow2_or_zero(s) ((bool)((((s) - 1) & (s)) == 0))
#define mtb_is_pow2(s) ((bool)((s) != 0 && mtb_is_pow2_or_zero((s))))


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


/* Code Helpers */

#define mtb_swap(a, b) do { __auto_type _t = a; a = b; b = _t; } while(0)

#if defined(_MTB_COMPILER_GCC)
#define _mtb_defer(_v, _d, _p, ...) \
    void _d(void **_p) { __VA_ARGS__; } \
    void *_v __attribute__((__cleanup__(_d)))
#define mtb_defer(...) _mtb_defer(mtb_id(_v), mtb_id(_d), mtb_id(_p), __VA_ARGS__)
#endif

#endif // MTB_MACRO_H
