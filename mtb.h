#ifndef MTB_H
#define MTB_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifdef MTB_IMPLEMENTATION
#define MTB_MACRO_IMPLEMENTATION
#define MTB_TYPE_IMPLEMENTATION
#define MTB_PERF_IMPLEMENTATION
#define MTB_LIST_IMPLEMENTATION
#define MTB_ARENA_IMPLEMENTATION
#define MTB_DYNARR_IMPLEMENTATION
#define MTB_SEGARR_IMPLEMENTATION
#define MTB_HMAP_IMPLEMENTATION
#define MTB_STRING_IMPLEMENTATION
#define MTB_RNG_IMPLEMENTATION
#endif

#ifdef MTB_TESTS
#define MTB_MACRO_TESTS
#define MTB_TYPE_TESTS
#define MTB_PERF_TESTS
#define MTB_LIST_TESTS
#define MTB_ARENA_TESTS
#define MTB_DYNARR_TESTS
#define MTB_SEGARR_TESTS
#define MTB_HMAP_TESTS
#define MTB_STRING_TESTS
#define MTB_RNG_TESTS
#endif

#ifdef MTB_BENCH
#define MTB_MACRO_BENCH
#define MTB_TYPE_BENCH
#define MTB_PERF_BENCH
#define MTB_LIST_BENCH
#define MTB_ARENA_BENCH
#define MTB_DYNARR_BENCH
#define MTB_SEGARR_BENCH
#define MTB_HMAP_BENCH
#define MTB_STRING_BENCH
#define MTB_RNG_BENCH
#endif

#include "mtb_macro.h"
#include "mtb_type.h"
#include "mtb_perf.h"
#include "mtb_list.h"
#include "mtb_arena.h"
#include "mtb_dynarr.h"
#include "mtb_segarr.h"
#include "mtb_hmap.h"
#include "mtb_string.h"
#include "mtb_rng.h"

#endif //MTB_H
