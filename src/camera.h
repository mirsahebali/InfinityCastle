#ifndef CAMERA_H
#define CAMERA_H
#include "raylib.h"

typedef struct
{
    Vector3 farPos[4];
    Vector3 nearPos[4];
} ViewXZPlane;

ViewXZPlane GetXZViewPlane(Camera camera, float zFar, float zNear, float aspect);

#endif // !CAMERA_H
