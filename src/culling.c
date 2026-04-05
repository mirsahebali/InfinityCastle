#include "culling.h"
#include "mtypes.h"

#include <raylib.h>
#include <raymath.h>

bool IsPointInsidePlane(Plane plane, Vector3 point)
{
    f32 distance = Vector3DotProduct(plane.normal, point) + plane.distance;
    return distance >= 0;
}
bool IsPointInsideFrustum(const Frustum *frustum, Vector3 point)
{
    return (IsPointInsidePlane(frustum->leftFace, point) && 
            IsPointInsidePlane(frustum->rightFace, point) &&
            IsPointInsidePlane(frustum->topFace, point) && 
            IsPointInsidePlane(frustum->bottomFace, point) &&
            IsPointInsidePlane(frustum->nearFace, point) && 
            IsPointInsidePlane(frustum->farFace, point));
}

bool IsBoxOutsidePlane(Plane plane, BoundingBox box)
{
    Vector3 p = {0};
    p.x = plane.normal.x >= 0 ? box.max.x : box.min.x;
    p.y = plane.normal.y >= 0 ? box.max.y : box.min.y;
    p.z = plane.normal.z >= 0 ? box.max.z : box.min.z;

    f32 distance = Vector3DotProduct(plane.normal, p) + plane.distance;
    return distance < 0;
}


bool IsBoxInsideFrustum(const Frustum *frustum, BoundingBox box)
{
    if(IsBoxOutsidePlane(frustum->leftFace,   box))   return false;
    if(IsBoxOutsidePlane(frustum->rightFace,  box))   return false;
    if(IsBoxOutsidePlane(frustum->topFace,    box))   return false;
    if(IsBoxOutsidePlane(frustum->bottomFace, box))   return false;
    if(IsBoxOutsidePlane(frustum->nearFace,   box))   return false;
    if(IsBoxOutsidePlane(frustum->farFace,    box))   return false;

    return true;
}
