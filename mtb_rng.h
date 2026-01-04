#ifndef MTB_MTB_RNG_H
#define MTB_MTB_RNG_H

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

#endif //MTB_MTB_RNG_H


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
