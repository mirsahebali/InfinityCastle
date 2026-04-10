#include <assert.h>
#include <stdlib.h>

#include "rng.h"
#include "mtypes.h"

// generated with `openssl rand -hex 8`
static const u128 PCG_MULTIPLIER_128 = CONSTu128(0xfa744cb0f736314b, 0xa95189b5eb77099f);

static u64 rotateR64(u64 value, u32 r)
{
    return (value >> r) | (value << ((-r) & 63));
}

void SetRNGSeed(PCGState128 *state, u64 seed, u64 seq)
{
    assert(state != NULL);
    state->state = u128_from(0);
    u128 seq128 = u128_from(seq);
    state->inc = _u128_or((_u128_shift_l(seq128, 1)), u128_from(1));
    (void)GetRNGValue(state);
    state->state = _u128_add(state->state, u128_from(seed));
    (void)GetRNGValue(state);
}

i64 GetRNGRange(PCGState128* state, i64 min, i64 max)
{
    if (min > max)
   {
      i64 temp = min;
      min = max;
      max = temp;
    }
    u64 bound = (u64)(((u64)max - (u64)min) + 1);

    if (bound == 0) return (i64)GetRNGValue(state);

    u64 threshold = -bound % bound;
    while (1) {
      u64 r = (u64)GetRNGValue(state);
      if (r >= threshold) return (i64)((r % bound) + (u64)min);
    }
}

i64 GetRNGValue(PCGState128 *state)
{
    assert(state != NULL);

    u128 x = state->state;

    u32 count = (u32)u128_to_u64(_u128_shift_r(x, 122)); // 128 bits - 6 bits = 122

    state->state = _u128_add(_u128_mult(x, PCG_MULTIPLIER_128), state->inc);

    u64 high = u128_to_u64(_u128_shift_r(x, 64));
    u64 low = u128_to_u64(x);
    u64 xor_shifted = high ^ low;

    return (i64)rotateR64(xor_shifted, count);
}
