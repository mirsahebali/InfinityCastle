#ifndef UTILS_H
#define UTILS_H

#include "mtypes.h"
#include <raylib.h>

#define VEC2(_x, _y) ((Vector2){.x = (f32)(_x), .y = (f32)(_y)})
#define VEC3(_x, _y, _z) ((Vector3){.x = (f32)(_x), .y = (f32)(_y), .z = (f32)(_z)})
#define VEC4(_x, _y, _z, _w) ((Vector4){.x = (f32)(_x), .y = (f32)(_y), .z = (f32)(_z), .w = (f32)(_w)})

#define VEC2_STR(v) (TextFormat("Vector2( x = %f, y = %f )", v.x, v.y))
#define VEC3_STR(v) (TextFormat("Vector3( x = %f, y = %f, z = %f )", v.x, v.y, v.z))


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

#define NOT_IMPLEMENTED                                                                                                \
    do                                                                                                                 \
    {                                                                                                                  \
        printf("%s:%s:%d not implemented\n", __FILE__, __FUNCTION__, __LINE__);                                        \
        exit(1);                                                                                                       \
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
} UnPackedBitsI32;

static inline UnPackedBitsI32 UnPackI32(u64 data)
{
    return (UnPackedBitsI32){
        .right = (i32)(data),
        .left = (i32)(data >> 32) & 0xFFFFFFFF,
    };
}

static inline UnMaskedBitsI16 UnPackI16(u32 data)
{
    return (UnMaskedBitsI16){
        .right = (i16)(data),
        .left = (i16)(data >> 16) & 0xFFFF,
    };
}

static inline u64 BitPackU64_3D(i16 x, i16 y, i16 z)
{
    return ((u64)(u16)x << 32 | ((u64)(u16)y << 16) | (u64)(u16)z);
}

static inline CellValue3D UnPackU64_3D(u64 data)
{
    return (CellValue3D){
        .x = (i16)(data >> 32),
        .y = (i16)(data >> 16),
        .z = (i16)(data),
    };
}

static inline u64 BitPackU64(i32 left, i32 right)
{
    return ((u64)(u32)left << 32) | (u32)right;
}

static inline u32 BitPackU32(i16 left, i16 right)
{
    return ((u32)(u16)left << 16) | (u16)right;
}

typedef enum
{
    DIRECTION_XZ,
    DIRECTION_XY,
    DIRECTION_YZ,
} Vector3Directions3D;

Vector2 Vec3ToVec2(Vector3, Vector3Directions3D);

void DrawFPSFull(Vector2 position, int fontSize, int paddingX, int paddingY, Color color);

#endif // !UTILS_H
