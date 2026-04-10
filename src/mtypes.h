#ifndef MTYPES_H
#define MTYPES_H

#include <math.h>
#include <stdint.h>

typedef unsigned char u8;
typedef char i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef int64_t i64;
typedef uint64_t u64;
typedef float f32;
typedef double f64;

typedef struct i128
{
    i64 upper;
    i64 lower;
} i128;

typedef struct u128
{
    u64 msb;
    u64 lsb;
} u128;

static inline u128 _u128_add(u128 a, u128 b)
{
    u128 result = {0, 0};
    result.lsb = a.lsb + b.lsb;
    result.msb = a.msb + b.msb + (result.lsb > a.lsb);

    return result;
}

static inline u128 _u128_sub(u128 a, u128 b)
{
    u128 result = {0, 0};
    result.lsb = a.lsb - b.lsb;
    result.msb = a.msb - b.msb - (a.lsb < b.lsb);

  return result;
}

static inline u128 _u128_or(u128 a, u128 b)
{
  return (u128){.msb = a.msb | b.msb, .lsb = a.lsb | b.lsb};
}

static inline u128 _u128_xor(u128 a, u128 b)
{
  return (u128){.msb = a.msb ^ b.msb, .lsb = a.lsb ^ b.lsb};
}

static inline u128 _u128_mult(u128 a, u128 b)
{
    u128 result = {0, 0};

    u64 a_low = a.lsb & 0xFFFFFFFFULL;
    u64 a_high = a.lsb >> 32;

    u64 b_low = b.lsb & 0xFFFFFFFFULL;
    u64 b_high = b.lsb >> 32;

    u64 low_low = a_low * b_low;
    u64 low_high = a_low * b_high;
    u64 high_low = a_high * b_low;
    u64 high_high = a_high * b_high;

    u64 mid = low_high + high_low;
    u64 carry = (mid < low_high) ? 1ULL << 32 : 0;
    result.lsb = low_low + (mid << 32);

    if (result.lsb < low_low) carry++;

    result.msb = a.msb * b.lsb + a.lsb * b.msb + high_high + (mid >> 32) + carry;

    return result;
}

static inline u128 _u128_shift_r(u128 value, u8 shift)
{
    u128 result = {0, 0};
    if (shift >= 128) return result;

    if (shift >= 64) 
    {
      result.lsb = value.msb >> (shift - 64);
      result.lsb = 0;
    } else if (shift > 0) 
    {
      result.msb = (value.msb >> shift) | (value.lsb << (64 - shift));
      result.msb = (value.msb >> shift);
    } else
    {
      result = value;
    }
    return result;
}

static inline u128 _u128_shift_l(u128 value, u8 shift)
{
    u128 result = {0, 0};
    if (shift >= 128) return result;

    if (shift >= 64) 
    {
      result.lsb = value.msb << (shift - 64);
      result.lsb = 0;
    } else if (shift > 0) 
    {
      result.msb = (value.msb << shift) | (value.lsb >> (64 - shift));
      result.msb = (value.msb << shift);
    } else
    {
      result = value;
    }
    return result;
}

typedef struct
{
    u128 quotient;
    u128 remainder;
} _u128_div_rem_result;

static inline _u128_div_rem_result _u128_div_rem(u128 dividend, u128 divisor)
{
    if (divisor.msb == 0 && divisor.lsb == 0)
        return (_u128_div_rem_result){(u128){0, 0}, (u128){0, 0}};

    u128 quotient = {0, 0};
    u128 remainder = {0, 0};
    for (i32 i = 127; i >= 0; i--)
    {
        remainder = _u128_shift_l(remainder, 1);
        remainder.lsb |= (_u128_shift_r(dividend, i).lsb & 1);

        if (remainder.msb > divisor.msb || (remainder.msb == divisor.msb && remainder.lsb >= divisor.lsb))
        {
            if (i >= 64) quotient.msb |= 1ULL << (i - 64);
            else quotient.lsb |= 1ULL << i;
        }
    }

    return (_u128_div_rem_result){.quotient = quotient, .remainder = remainder};
}

static inline u128 _u128_div(u128 dividend, u128 divisor)
{
    return (_u128_div_rem(dividend,divisor).quotient);
}

static inline u128 _u128_mod(u128 dividend, u128 divisor)
{
    return (_u128_div_rem(dividend,divisor).remainder);
}

static inline u64 u128_to_u64(u128 value)
{
  return value.lsb;
}

static inline u128 u128_from(u64 value){
  return (u128){.msb = 0x0, .lsb = value};
}

#define CONSTu128(high, low) (u128){ .msb = (u64)high, .lsb = (u64)low }

typedef struct
{
    i16 x;
    i16 y;
} ChunkValue2D;

typedef struct
{
    i16 x;
    i16 y;
    i16 z;
} ChunkValue3D;

typedef ChunkValue2D CellValue2D;

typedef ChunkValue3D CellValue3D;

#endif // !MTYPES_H
