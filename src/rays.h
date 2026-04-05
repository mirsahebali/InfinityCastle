#ifndef RAYS_H
#define RAYS_H
#include <raylib.h>

#include "mtypes.h"
#include "utils.h"

#define MAX_RAY_DISTANCE 1000
#define MAX_RAY_INTERSECTIONS 30
#define MAX_CAMERA_RAYS 10

typedef struct
{
    ChunkValue2D cells[MAX_RAY_INTERSECTIONS];
} RayIntersections;

typedef struct
{
    Vector2 direction[MAX_CAMERA_RAYS];
} CameraRays;

// Uses Bresenham Algorithm
RayIntersections DetectCollidedCells(Vector2 start, Vector2 end, i32 stepSize, i32 xSpacing, i32 ySpacing);

RayIntersections GetRayCellIntersections(Ray r, Vector3Directions3D vecDir, i32 stepSize, i32 xSpacing, i32 ySpacing,
                                         f32 scale);

CameraRays GenerateCameraRays(Camera camera, float distance);

#define GetLineCellIntersections DetectCollidedCells

#endif // !RAYS_H
