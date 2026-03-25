#ifndef GLOBAL_STATE_H
#define GLOBAL_STATE_H

#include "raylib.h"

extern float mouseSensitivity;
extern Camera3D camera3D;
extern Camera2D camera2D;
extern float mapWidth;
extern float mapHeight;
extern Vector2 mapOffset;
extern int seed;
extern int gridSpacing;
#define MAP_MAX 20

#endif // !GLOBAL_STATE_H
