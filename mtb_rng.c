// Based on "PCG: A Family of Simple Fast Space-Efficient Statistically Good Algorithms
// for Random Number Generation", M.E. O'Neill, 2014, https://www.pcg-random.org

#include <math.h>

#include "mtb_rng.h"


intern u64 _mtb_rng32_splitmix64(u64 *seed) {
    u64 result = (*seed += u64_lit(0x9e3779b97f4a7c15));
    result = (result ^ (result >> 30)) * u64_lit(0xbf58476d1ce4e5b9);
    result = (result ^ (result >> 27)) * u64_lit(0x94d049bb133111eb);
    return result ^ (result >> 31);
}

public void
mtb_rng32_init(MtbRng32 *rng, u64 seed)
{
    // Use splitmix64 for proper seeding
    rng->state = _mtb_rng32_splitmix64(&seed);
    rng->inc = _mtb_rng32_splitmix64(&seed) | 1; // ensure oddness
}

public void
mtb_rng64_init(MtbRng64 *rng, u64 seed)
{
    mtb_rng32_init(&rng->gens[0], seed);
    do {
        mtb_rng32_init(&rng->gens[1], _mtb_rng32_splitmix64(&seed));
    } while (rng->gens[0].inc == rng->gens[1].inc); // must be distinct!
}

public u32
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

public u64
mtb_rng64_next(MtbRng64 *rng)
{
    u32 h = mtb_rng32_next(&rng->gens[0]);
    u32 l = mtb_rng32_next(&rng->gens[1]);
    return ((u64)h << 32) | l;
}

public u32
mtb_rng32_next_bounded(MtbRng32 *rng, u32 range)
{
    mtb_assert_always(range > 0);
    u32 mask = U32_MAX >> __builtin_clz((range - 1) | 1);
    u32 x;
    do {
        x = mtb_rng32_next(rng) & mask;
    } while (x >= range);
    return x;
}

public u64
mtb_rng64_next_bounded(MtbRng64 *rng, u64 range)
{
    mtb_assert_always(range > 0);
    u64 mask = U64_MAX >> __builtin_clzl((range - 1) | 1);
    u64 x;
    do {
        x = mtb_rng64_next(rng) & mask;
    } while (x >= range);
    return x;
}

public f32
mtb_rng32_next_unit(MtbRng32 *rng)
{
    return ldexpf((f32)mtb_rng32_next(rng), -32);
}

public f64
mtb_rng64_next_unit(MtbRng64 *rng)
{
    return ldexp((f64)mtb_rng64_next(rng), -64);
}


#ifdef MTB_RNG_TESTS

#include <assert.h>
#include <time.h>


intern void
test_mtb_rng32()
{
    MtbRng32 rng = {0};
    mtb_rng32_init(&rng, time(nil));

    i32 n = 100000000;

    for (i32 i = 0; i < n; i++) {
        u32 x = mtb_rng32_next_bounded(&rng, 10);
        assert(x < 10);
    }

    for (i32 i = 0; i < n; i++) {
        f32 x = mtb_rng32_next_unit(&rng);
        assert(0.0f <= x && x <= 1.0f);
    }
}

intern void
test_mtb_rng64()
{
    MtbRng64 rng = {0};
    mtb_rng64_init(&rng, time(nil));

    i32 n = 100000000;

    for (i32 i = 0; i < n; i++) {
        u64 x = mtb_rng64_next_bounded(&rng, 10);
        assert(x < 10);
    }

    for (i32 i = 0; i < n; i++) {
        f64 x = mtb_rng64_next_unit(&rng);
        assert(0.0f <= x && x <= 1.0f);
    }
}

intern void
test_mtb_rng()
{
    test_mtb_rng32();
    test_mtb_rng64();
}

#endif // MTB_RNG_TESTS
