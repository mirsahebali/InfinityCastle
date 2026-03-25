#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include <stdint.h>

#include "raylib.h"

#include "global_state.h"

#include "buildings.h"

// determines the grid based on layout
void DrawMapGridLines(Vector2 pos, int width, int height, int lines, Color border);

void DrawMapGridSpaces(Vector2 pos, int width, int height, int spaces, Color border);

void DrawMap(BuildingType mapData[MAP_MAX][MAP_MAX], Vector2 pos, int width, int height, int seed, Color border);

#endif // !MAP_H
