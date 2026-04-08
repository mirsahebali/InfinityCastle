#include "rays.h"
#include "utils.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

RayIntersections GetRayCellIntersections(Ray r, Vector3Directions3D vecDir, i32 stepSize, i32 xSpacing, i32 ySpacing,
                                         f32 scale)
{
    Vector2 startVec = Vec3ToVec2(r.position, vecDir);
    Vector2 endVec = Vector2Scale(Vec3ToVec2(r.direction, vecDir), scale);

    return DetectRayCollidedCells(startVec, endVec, stepSize, xSpacing, ySpacing);
}

RayIntersections DetectRayCollidedCells(Vector2 start, Vector2 end, i32 stepSize, i32 xSpacing, i32 ySpacing)
{
    RayIntersections intersections = {0};
    i32 startX = start.x;
    i32 startY = start.y;
    i32 endX = end.x;
    i32 endY = end.y;

    i32 dx = abs(endX - startX);
    i32 dy = abs((i32)endY - (i32)startY);

    i32 stepX = startX < endX ? stepSize : -stepSize;
    i32 stepY = startY < endY ? stepSize : -stepSize;
    i32 error = dx - dy;

    i32 i = 0;
    while (true)
    {
        intersections.cells[i].x = startX / xSpacing;
        intersections.cells[i].y = startY / ySpacing;

        if (startX == endX && endY == startY)
            break;
        i32 err2 = 2 * error;
        if (err2 >= -dy)
        {

            error -= dy;
            startX += stepX;
        }
        if (err2 < dx)
        {
            error += dx;
            startY += stepY;
        }

        i++;
        if (i >= MAX_RAY_INTERSECTIONS)
            return intersections;
    }

    return intersections;
}

// we'll be generating rays in the  XZ plane, with y set at 0.0f
CameraRays GenerateCameraRays(Camera camera, float distance)
{
    CameraRays cr = {0};

    Vector3 cameraForward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector2 cameraForward2D = VEC2(cameraForward.x, cameraForward.z);

    float fovRadian = DEG2RAD * camera.fovy;
    float angleStart = -fovRadian * 0.5f;
    float angleStep = fovRadian / (MAX_CAMERA_RAYS - 1);

    for (i32 i = 0; i < MAX_CAMERA_RAYS; i++)
    {
        float angle = angleStart + i * angleStep;
        Vector2 dir = Vector2Rotate(cameraForward2D, angle);
        cr.direction[i] = dir;
    }

    return cr;
}
