#define MTB_MACRO_TESTS
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
    test_mtb_macro();
    test_mtb_type();
    test_mtb_list();
    test_mtb_arena();
    test_mtb_dynarr();
    test_mtb_segarr();
    test_mtb_hmap();
    test_mtb_string();
    test_mtb_rng();
}
