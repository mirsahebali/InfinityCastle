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
    BoundingBox box;
} Building;

typedef struct AABBVertex
{
    Vector3 corners[8];
} AABBVertex;

typedef Building *BuildingArray;

Rectangle genRandomBuilding2D(Vector2 cell, i32 cellWidth, i32 cellHeight);

BoundingBox genRandomBoundingBox2D(CellValue2D cell, i32 cellWidth, i32 cellHeight);

BoundingBox genRandomBoundingBox3D(CellValue3D cell, i32 cellWidth, i32 cellHeight, i32 cellLength);
AABBVertex getAABBVerticesOfRect(Building *building);

BuildingArray generateBuildings(Arena *arena, u32 width, u32 height, i32 offsetX, i32 offsetY, i32 spacing);

void DrawBuildingsTopDownView(BuildingArray arr);
void DrawBuildingModels(BuildingArray arr);
void DrawBuildingModel(Building building);
void DrawFilledBoundingBox(BoundingBox box, Color color);
void DrawBoundingBoxWires(BoundingBox box, Color color);

#endif // !BUILDINGS_H
