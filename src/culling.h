#ifndef CULLING_H
#define CULLING_H

#include <raylib.h>
#include <stdbool.h>

#include "planes.h"

bool IsPointInsidePlane(Plane plane, Vector3 point);
bool IsPointInsideFrustum(const Frustum *frustum, Vector3 point);
bool IsBoxOutsidePlane(Plane plane, BoundingBox box);
bool IsBoxInsideFrustum(const Frustum *frustum, BoundingBox box);

#endif // !CULLING_H
