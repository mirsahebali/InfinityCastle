#include "utils.h"

Vector2 Vec3ToVec2(Vector3 value, Vector3Directions3D dir)
{
    switch (dir)
    {
    case DIRECTION_XZ:
        return VEC2(value.x, value.z);
    case DIRECTION_XY:
        return VEC2(value.x, value.y);
    case DIRECTION_YZ:
        return VEC2(value.y, value.z);
    }
}
