#include "planes.h"

#include <math.h>
#include <raylib.h>
#include <raymath.h>

Plane createPlane(Vector3 point, Vector3 normal)
{
    Plane p = {0};
    p.normal = Vector3Normalize(normal);
    p.distance = -Vector3DotProduct(p.normal, point);
    return p;
}

Frustum createFrustumFromCamera(Camera3D camera, f32 aspect, f32 zNear, f32 zFar)
{
    Frustum frustum = {0};
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));
    Vector3 up = Vector3Normalize(Vector3CrossProduct(right, forward));

    // Near & far centers
    Vector3 farCenter = Vector3Add(camera.position, Vector3Scale(forward, zFar));
    Vector3 nearCenter = Vector3Add(camera.position, Vector3Scale(forward, zNear));
    
    f32 tanFov = tanf(camera.fovy * 0.5 * DEG2RAD);
    
    f32 nearHeight = zNear * tanFov;
    f32 nearWidth = aspect * nearHeight;

    f32 farHeight = zFar * tanFov;
    f32 farWidth = aspect * farHeight;
    
    // Far Plane corners
    Vector3 farTopLeft = Vector3Add(farCenter, Vector3Add(Vector3Scale(up, farHeight), Vector3Scale(right, -farWidth)));
    Vector3 farTopRight = Vector3Add(farCenter, Vector3Add(Vector3Scale(up, farHeight), Vector3Scale(right, farWidth)));
    Vector3 farBottomLeft = Vector3Add(farCenter, Vector3Add(Vector3Scale(up, -farHeight), Vector3Scale(right, -farWidth)));
    Vector3 farBottomRight = Vector3Add(farCenter, Vector3Add(Vector3Scale(up, -farHeight), Vector3Scale(right, farWidth)));

    // Near and Far planes
    frustum.nearFace = createPlane(nearCenter, forward);
    frustum.farFace = createPlane(farCenter, Vector3Negate(forward));

    // Right Plane
    {
      Vector3 point1 = Vector3Subtract(farTopRight, camera.position);
      Vector3 point2 = Vector3Subtract(farBottomRight, camera.position);
      Vector3 normal = Vector3Normalize(Vector3CrossProduct(point1, point2));
      frustum.rightFace = createPlane(camera.position, normal);
    }

    // Left Plane
    {
      Vector3 point1 = Vector3Subtract(farBottomLeft, camera.position);
      Vector3 point2 = Vector3Subtract(farTopLeft, camera.position);
      Vector3 normal = Vector3Normalize(Vector3CrossProduct(point1, point2));
      frustum.leftFace = createPlane(camera.position, normal);
    }

    // Top Plane
    {
      Vector3 point1 = Vector3Subtract(farTopLeft, camera.position);
      Vector3 point2 = Vector3Subtract(farTopRight, camera.position);
      Vector3 normal = Vector3Normalize(Vector3CrossProduct(point1, point2));
      frustum.topFace = createPlane(camera.position, normal);
    }

    // Bottom Plane
    {
      Vector3 point1 = Vector3Subtract(farBottomRight, camera.position);
      Vector3 point2 = Vector3Subtract(farBottomLeft, camera.position);
      Vector3 normal = Vector3Normalize(Vector3CrossProduct(point1, point2));
      frustum.bottomFace = createPlane(camera.position, normal);
    }

    return frustum;
}
