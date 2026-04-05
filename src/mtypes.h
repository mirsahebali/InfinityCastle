#ifndef MTYPES_H
#define MTYPES_H

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
