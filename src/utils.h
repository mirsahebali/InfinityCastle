#ifndef UTILS_H
#define UTILS_H

#include "mtypes.h"

#define VEC2(_x, _y) ((Vector2){.x = (f32)(_x), .y = (f32)(_y)})
#define VEC3(_x, _y, _z) ((Vector3){.x = (f32)(_x), .y = (f32)(_y), .z = (f32)(_z)})
#define VEC4(_x, _y, _z, _w) ((Vector4){.x = (f32)(_x), .y = (f32)(_y), .z = (f32)(_z), .w = (f32)(_w)})

#define RECT(_x, _y, _width, _height)                                                                                  \
    ((Rectangle){.x = (f32)(_x), .y = (f32)(_y), .width = (f32)(_width), .height = (f32)(_height)})

#define TO_RADIANS(deg) ((PI / 180) * (deg))

#define EVAL_PRINT(specifier, left, right)                                                                             \
    printf("Assertion failed at: %s %s:%d\n", __FUNCTION__, __FILE__, __LINE__);                                       \
    printf("left: ");                                                                                                  \
    printf("%s = " specifier, #left, left);                                                                            \
    printf("\n");                                                                                                      \
    printf("right: ");                                                                                                 \
    printf("%s = " specifier, #right, right);                                                                          \
    printf("\n");

#define ASSERT_EVAL(specifier, left, right)                                                                            \
    {                                                                                                                  \
        EVAL_PRINT(specifier, left, right);                                                                            \
        assert(false);                                                                                                 \
    }

#define ASSERT_EQ(specifier, left, right)                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        if (left != right)                                                                                             \
            ASSERT_EVAL(specifier, left, right)                                                                        \
    } while (0)

#define ASSERT_NEQ(specifier, left, right)                                                                             \
    do                                                                                                                 \
    {                                                                                                                  \
        if (left == right)                                                                                             \
            ASSERT_EVAL(specifier, left, right)                                                                        \
    } while (0)

typedef struct
{
    i16 left;
    i16 right;
} UnMaskedBitsI16;

typedef struct
{
    i32 left;
    i32 right;
} UnMaskedBitsI32;

static inline UnMaskedBitsI32 unMaskI32(u64 data)
{
    return (UnMaskedBitsI32){
        .right = (i32)(data),
        .left = (i32)(data >> 32),
    };
}

static inline UnMaskedBitsI16 unMaskI16(u32 data)
{
    return (UnMaskedBitsI16){
        .right = (i16)(data),
        .left = (i16)(data >> 16) & 0xFFFF,
    };
}

static inline u32 genUniqueU32(i16 left, i16 right)
{
    return ((u32)(u16)left << 16) | (u16)right;
}

static inline u64 genUniqueU64(i32 left, i32 right)
{
    return ((u64)(u32)left << 32) | (u32)right;
}

#endif // !UTILS_H
