#include "camera.h"

#include "mtypes.h"
#include "raymath.h"

ViewXZPlane GetXZViewPlane(Camera camera, float zFar, float zNear, float aspect)
{
    ViewXZPlane out = {0};
    Vector3 cameraForward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector3 cameraRight = Vector3CrossProduct(camera.up, cameraForward);
    Vector3 nearCenter = Vector3Subtract(camera.position, Vector3Scale(cameraForward, zNear));
    Vector3 farCenter = Vector3Subtract(camera.position, Vector3Scale(cameraForward, zFar));
    f32 nearHeight = 2 * tan(camera.fovy * DEG2RAD / 2) * zNear;
    f32 farHeight = 2 * tan(camera.fovy * DEG2RAD / 2) * zFar;

    f32 nearWidth = nearHeight * aspect;
    f32 farWidth = farHeight * aspect;

    Vector3 camRightMultFarWidth = Vector3Scale(cameraRight, farWidth * 0.5);
    Vector3 farTopLeft =
        Vector3Subtract(Vector3Add(farCenter, Vector3Scale(camera.up, farHeight * 0.5)), camRightMultFarWidth);
    Vector3 farTopRight =
        Vector3Add(Vector3Add(farCenter, Vector3Scale(camera.up, farHeight * 0.5)), camRightMultFarWidth);
    Vector3 farBottomLeft =
        Vector3Subtract(Vector3Subtract(farCenter, Vector3Scale(camera.up, farHeight * 0.5)), camRightMultFarWidth);
    Vector3 farBottomRight =
        Vector3Add(Vector3Subtract(farCenter, Vector3Scale(camera.up, farHeight * 0.5)), camRightMultFarWidth);

    out.farPos[0] = farTopLeft;
    out.farPos[1] = farTopRight;
    out.farPos[2] = farBottomLeft;
    out.farPos[3] = farBottomRight;

    Vector3 camRightMultNearWidth = Vector3Scale(cameraRight, nearWidth * 0.5);
    Vector3 nearTopLeft =
        Vector3Subtract(Vector3Add(nearCenter, Vector3Scale(camera.up, nearHeight * 0.5)), camRightMultNearWidth);
    Vector3 nearTopRight =
        Vector3Add(Vector3Add(nearCenter, Vector3Scale(camera.up, nearHeight * 0.5)), camRightMultNearWidth);
    Vector3 nearBottomLeft =
        Vector3Subtract(Vector3Subtract(nearCenter, Vector3Scale(camera.up, nearHeight * 0.5)), camRightMultNearWidth);
    Vector3 nearBottomRight =
        Vector3Add(Vector3Subtract(nearCenter, Vector3Scale(camera.up, nearHeight * 0.5)), camRightMultNearWidth);

    out.nearPos[0] = nearTopLeft;
    out.nearPos[1] = nearTopRight;
    out.nearPos[2] = nearBottomLeft;
    out.nearPos[3] = nearBottomRight;
    return out;
}
