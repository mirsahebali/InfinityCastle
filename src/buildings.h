#ifndef BUILDINGS_H
#define BUILDINGS_H

#include <raylib.h>
#include <stdint.h>

#include "carena.h"
#include "mtypes.h"

// Placeholder just for now, we'll update it with smth better later
typedef enum
{
    BUILDING_TYPE_NONE, // render nothing for this one
    BUILDING_TYPE_1,
    BUILDING_TYPE_2,
    BUILDING_TYPE_3,
    BUILDING_TYPE_4,
    BUILDING_TYPE_5,
    BUILDING_TYPE_6,
    BUILDING_TYPE_7,
    BUILDING_TYPE_COUNT,

} BuildingType;

Color map_building_type_to_color(BuildingType t);

typedef struct
{
    u32 id;
    BuildingType bType;
    Rectangle rect;
} Building;

typedef Building *BuildingArray;

Rectangle genRandomBuilding2D(Vector2 cell, int cellWidth, int cellHeight);

BuildingArray generateBuildings(Arena *arena, u32 width, u32 height, i32 offsetX, i32 offsetY, i32 spacing);

void DrawBuidlingsTopDownView(BuildingArray arr);

void DrawBuildingModels(BuildingArray arr);

#endif // !BUILDINGS_H
