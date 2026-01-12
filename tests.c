#define MTB_IMPLEMENTATION
#define MTB_TESTS
#include "mtb.h"

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
