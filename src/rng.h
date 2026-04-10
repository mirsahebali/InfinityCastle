#ifndef RNG_H
#define RNG_H

#include "mtypes.h"

typedef struct {
  u128 state;
  u128 inc;
} PCGState128;

extern PCGState128 GlobalPCGState;

void SetRNGSeed(PCGState128 *state, u64 seed, u64 seq);

i64 GetRNGRange(PCGState128* state, i64 min, i64 max);

i64 GetRNGValue(PCGState128 *state);

#endif // !RNG_H
