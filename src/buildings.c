#include "buildings.h"

#include "external/stb_perlin.h"

#include "map.h"

#include "mtypes.h"
#include "raylib.h"

#include "utils.h"

#include "carena.h"

#include <raymath.h>
#include <stdint.h>
#include <stdio.h>

Color map_building_type_to_color(BuildingType t)
{
    switch (t)
    {
    case BUILDING_TYPE_NONE:
        return BLANK;
    case BUILDING_TYPE_1:
        return RED;
    case BUILDING_TYPE_2:
        return BLUE;
    case BUILDING_TYPE_3:
        return GREEN;
    case BUILDING_TYPE_4:
        return YELLOW;
    case BUILDING_TYPE_5:
        return PINK;
    case BUILDING_TYPE_6:
        return MAGENTA;
    case BUILDING_TYPE_7:
        return BLACK;
    case BUILDING_TYPE_COUNT:
        return BLANK;
    default:
        return BLANK;
    }
}

static const Vector2 OFFSET_SCALING_FORWARD = {0.3f, 0.3f};
static const Vector2 OFFSET_SCALING_BACKWARD = {0.2f, 0.3f};
Rectangle genRandomBuilding2D(Vector2 cell, i32 cellWidth, i32 cellHeight)
{
    SetRandomSeed(genUniqueU32((i16)cell.x, (i16)cell.y));
    i32 cellInitX = cell.x * cellWidth;
    i32 cellInitY = cell.y * cellHeight;

    Vector2 endPosOffsetInsideCell = {
        .x = cellInitX + (cellWidth * OFFSET_SCALING_FORWARD.x),
        .y = cellInitY + (cellHeight * OFFSET_SCALING_FORWARD.y),
    };
    Vector2 pos = {0};
    pos.x = GetRandomValue(cellInitX, endPosOffsetInsideCell.x);
    pos.y = GetRandomValue(cellInitY, endPosOffsetInsideCell.y);

    i32 finalCellWidth = GetRandomValue(cellWidth * OFFSET_SCALING_FORWARD.x, cellInitX + cellWidth - pos.x);
    i32 finalCellHeight = GetRandomValue(cellHeight * OFFSET_SCALING_FORWARD.y, cellInitY + cellHeight - pos.y);

    return (Rectangle){
        .x = pos.x,
        .y = pos.y,
        .width = finalCellWidth,
        .height = finalCellHeight,
    };
}

const f32 BOX_Y_SIZE = 40.0f;
BoundingBox genRandomBoundingBox2D(CellValue2D cell, i32 cellWidth, i32 cellHeight)
{
    BoundingBox out = {0};
    SetRandomSeed(genUniqueU32((i16)cell.x, (i16)cell.y));
    Vector3 minPos = {0};
    minPos.x = cell.x * cellWidth;
    minPos.y = 0.0f;
    minPos.z = cell.y * cellHeight;

    Vector3 maxPos = Vector3Add(minPos, VEC3(cellWidth, BOX_Y_SIZE, cellHeight));
    out.min.x = GetRandomValue(minPos.x, minPos.x + (cellWidth * OFFSET_SCALING_FORWARD.x));
    out.min.y = minPos.y;
    out.min.z = GetRandomValue(minPos.z, minPos.z + (cellHeight * OFFSET_SCALING_FORWARD.y));

    out.max.x = GetRandomValue(maxPos.x, maxPos.x - (cellWidth * OFFSET_SCALING_BACKWARD.x));
    out.max.y = maxPos.y;
    out.max.z = GetRandomValue(maxPos.z, maxPos.z - (cellHeight * OFFSET_SCALING_BACKWARD.y));

    return out;
}

BoundingBox genRandomBoundingBox3D(CellValue3D cell, i32 cellWidth, i32 cellHeight, i32 cellLength)
{
    BoundingBox box = {0};
    NOT_IMPLEMENTED;
    return box;
}

BuildingArray generateBuildings(Arena *arena, u32 width, u32 height, i32 offsetX, i32 offsetY, i32 spacing)
{
    BuildingArray newBuildingsArray2D = ARRAY_WITH_CAPACITY(arena, Building, 4 * width * height);

    for (i32 y = -((i32)height); y < (i32)height; y++)
    {
        for (i32 x = -((i32)width); x < (i32)width; x++)
        {

            Building data = {0};
            data.id = genUniqueU32(x, y);
            data.box = (BoundingBox){0};

            SetRandomSeed(data.id);
            // Rectangle rect = genRandomBuilding2D(VEC2(x, y), spacing, spacing);
            BoundingBox box = genRandomBoundingBox2D((CellValue2D){x, y}, spacing, spacing);
            data.box = box;
            data.bType = GetRandomValue(BUILDING_TYPE_1, BUILDING_TYPE_7);

            newBuildingsArray2D = ARRAY_PUSH(newBuildingsArray2D, Building, &data);
        }
    }
    return newBuildingsArray2D;
}

void DrawBuildingsTopDownView(BuildingArray arr)
{
    assert(arr != NULL);
#ifndef NDEBUG
    DrawMapGridSpaces(VEC2(0, 0), GetScreenWidth(), GetScreenHeight(), 50, BLACK);
#endif /* ifndef #ifndef NDEBUG */

    for (size_t i = 0; i < ARRAY_LENGTH(arr, Building); i++)
    {
        Building building = ARRAY_GET(arr, Building, i);

        Color buildingColor = map_building_type_to_color(building.bType);

        DrawRectangle(building.box.min.x, building.box.min.z, building.box.max.x - building.box.min.x,
                      building.box.max.z - building.box.min.z, buildingColor);
    }
}

AABBVertex getAABBVerticesOfRect(Building *building)
{
    AABBVertex out = {0};
    for (i32 i = 0; i < 8; i++)
    {
    }
    return out;
}

void DrawBuildingModels(BuildingArray arr)
{
    size_t length = ARRAY_LENGTH(arr, Building);
    for (size_t i = 0; i < length; i++)
    {
        Building building = ARRAY_GET(arr, Building, i);
        DrawBuildingModel(building);
    }
}

void DrawBuildingModel(Building building)
{
    DrawBoundingBox(building.box, map_building_type_to_color(building.bType));
}

void DrawFilledBoundingBox(BoundingBox box, Color color)
{
    float width = box.max.x - box.min.x;
    float height = box.max.y - box.min.y;
    float depth = box.max.z - box.min.z;

    Vector3 pos = box.min;

    DrawCube(pos, width, height, depth, color);
}

void DrawBoundingBoxWires(BoundingBox box, Color color)
{
    float width = box.max.x - box.min.x;
    float height = box.max.y - box.min.y;
    float depth = box.max.z - box.min.z;

    Vector3 pos = box.min;

    DrawCubeWires(pos, width, height, depth, color);
}
