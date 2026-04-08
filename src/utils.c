#include "utils.h"
#include <stdio.h>

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

int globalFPSLow = 0;
int globalFPSHigh = 0;
int currentFPS = 0;
void DrawFPSFull(Vector2 position, int fontSize, int paddingX, int paddingY, Color color)
{
  currentFPS = GetFPS();
  if(globalFPSLow == 0) globalFPSLow = currentFPS;
  if(currentFPS < globalFPSLow) globalFPSLow = currentFPS;
  if(currentFPS > globalFPSHigh) globalFPSHigh = currentFPS;
#define FPSStr TextFormat("FPS: %d", GetFPS())
#define LowFPSStr TextFormat("Low: %d", globalFPSLow)
#define HighFPSStr TextFormat("High: %d", globalFPSHigh)
#define AvgFPSStr TextFormat("Avg: %d", (globalFPSHigh + globalFPSLow) / 2)


    DrawRectangle(position.x, position.y, MeasureText(HighFPSStr, fontSize) + paddingX, fontSize * 4 + (paddingY * 4), WHITE);
    DrawRectangleLines(position.x, position.y, MeasureText(HighFPSStr, fontSize) + paddingX, fontSize * 4 + (paddingY * 4), BLUE);
    DrawText(FPSStr, position.x + 2, position.y + 2 + paddingY, fontSize, color);
    DrawText(LowFPSStr, position.x + 2, position.y + 2 + fontSize + paddingY, fontSize, color);
    DrawText(AvgFPSStr, position.x + 2, position.y + 2 + (2 * fontSize) + paddingY, fontSize, color);
    DrawText(HighFPSStr, position.x + 2, position.y + 2 + (3 * fontSize) + paddingY, fontSize, color);
}
