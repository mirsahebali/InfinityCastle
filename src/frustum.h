#ifndef PLANES_H
#define PLANES_H

#include "mtypes.h"
#include <raylib.h>
typedef struct Plane
{
    Vector3 normal;
    f32 distance;
} Plane;

typedef struct Frustum
{
    Plane topFace;
    Plane bottomFace;

    Plane rightFace;
    Plane leftFace;

    Plane farFace;
    Plane nearFace;
} Frustum;

Frustum CreateFrustumFromCamera(Camera3D camera, f32 aspect, f32 zNear, f32 zFar);

#endif // !PLANES_H
