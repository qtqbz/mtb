#ifndef MTB_MTB_RNG_H
#define MTB_MTB_RNG_H

// Based on "PCG: A Family of Simple Fast Space-Efficient Statistically Good Algorithms
// for Random Number Generation", M.E. O'Neill, 2014, https://www.pcg-random.org

#include "mtb_macro.h"
#include "mtb_type.h"


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


public void mtb_rng32_init(MtbRng32 *rng, u64 seed);
public void mtb_rng64_init(MtbRng64 *rng, u64 seed);

// [0, MAX]
public u32 mtb_rng32_next(MtbRng32 *rng);
public u64 mtb_rng64_next(MtbRng64 *rng);

// [0, range)
public u32 mtb_rng32_next_bounded(MtbRng32 *rng, u32 range);
public u64 mtb_rng64_next_bounded(MtbRng64 *rng, u64 range);

// [0.0, 1.0]
public f32 mtb_rng32_next_unit(MtbRng32 *rng);
public f64 mtb_rng64_next_unit(MtbRng64 *rng);

#endif //MTB_MTB_RNG_H
