#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MTB_TYPE_TESTS
#define MTB_LIST_TESTS
#define MTB_ARENA_TESTS
#define MTB_DYNARR_TESTS
#define MTB_SEGARR_TESTS
#define MTB_HMAP_TESTS
#define MTB_STRING_TESTS
#define MTB_RNG_TESTS
#include "mtb_all.h"


int
main(void)
{
    _test_mtb_type();
    _test_mtb_list();
    _test_mtb_arena();
    _test_mtb_dynarr();
    _test_mtb_segarr();
    _test_mtb_hmap();
    _test_mtb_string();
    _test_mtb_rng();
}
