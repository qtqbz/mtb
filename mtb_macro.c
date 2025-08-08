#ifdef MTB_MACRO_TESTS

#include <assert.h>


intern void
test_mtb_defer(void)
{
#if defined(_MTB_COMPILER_GCC)
    int i = 0;
    {
        mtb_defer(i++);
        {
            mtb_defer(i++);
            assert(i == 0);
        }
        assert(i == 1);
    }
    assert(i == 2);
#endif
}

intern void
test_mtb_swap(void)
{
    int a = 5;
    int b = 10;
    mtb_swap(a, b);
    assert(a == 10);
    assert(b == 5);
}

intern void
test_mtb_macro(void)
{
    test_mtb_defer();
    test_mtb_swap();
}

#endif // MTB_MACRO_TESTS
