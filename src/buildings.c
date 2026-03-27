#include "buildings.h"

#include "external/stb_perlin.h"

#include "map.h"

#include "mtypes.h"
#include "raylib.h"

#include "utils.h"

#include "carena.h"

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

static const Vector2 OFFSET_SCALING = {0.3f, 0.3f};
Rectangle genRandomBuilding2D(Vector2 cell, int cellWidth, int cellHeight)
{
    SetRandomSeed(genUniqueU32((i16)cell.x, (i16)cell.y));
    int cellInitX = cell.x * cellWidth;
    int cellInitY = cell.y * cellHeight;

    Vector2 endPosOffsetInsideCell = {
        .x = cellInitX + (cellWidth * OFFSET_SCALING.x),
        .y = cellInitY + (cellHeight * OFFSET_SCALING.y),
    };
    Vector2 pos = {0};
    pos.x = GetRandomValue(cellInitX, endPosOffsetInsideCell.x);
    pos.y = GetRandomValue(cellInitY, endPosOffsetInsideCell.y);

    int finalCellWidth = GetRandomValue(cellWidth * OFFSET_SCALING.x, cellInitX + cellWidth - pos.x);
    int finalCellHeight = GetRandomValue(cellHeight * OFFSET_SCALING.y, cellInitY + cellHeight - pos.y);

    return (Rectangle){
        .x = pos.x,
        .y = pos.y,
        .width = finalCellWidth,
        .height = finalCellHeight,
    };
}

BuildingArray generateBuildings(Arena *arena, u32 width, u32 height, i32 offsetX, i32 offsetY, i32 spacing)
{
    BuildingArray newBuildingsArray2D = ARRAY_WITH_CAPACITY(arena, Building, 4 * width * height);

    for (i32 y = -((i32)height); y < (i32)height; y++)
    {
        for (i32 x = -((i32)width); x < (i32)width; x++)
        {
            SetRandomSeed(y * width + x);

            Building data = (Building){.id = genUniqueU32(x, y),
                                       .rect = genRandomBuilding2D(VEC2(x, y), spacing, spacing),
                                       .bType = GetRandomValue(BUILDING_TYPE_1, BUILDING_TYPE_7)};

            newBuildingsArray2D = ARRAY_PUSH(newBuildingsArray2D, Building, &data);
        }
    }
    return newBuildingsArray2D;
}

void DrawBuidlingsTopDownView(BuildingArray arr)
{
    assert(arr != NULL);
#ifndef NDEBUG
    DrawMapGridSpaces(VEC2(0, 0), GetScreenWidth(), GetScreenHeight(), 50, BLACK);
#endif /* ifndef #ifndef NDEBUG */

    for (size_t i = 0; i < ARRAY_LENGTH(arr, Building); i++)
    {
        Building building = ARRAY_GET(arr, Building, i);

        Color buildingColor = map_building_type_to_color(building.bType);

        DrawRectangleRec(building.rect, buildingColor);
    }
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

    DrawCube(VEC3(building.rect.x, 2.0f, building.rect.y), building.rect.width, 20.0f, building.rect.height,
             map_building_type_to_color(building.bType));
    DrawCubeWires(VEC3(building.rect.x, 2.0f, building.rect.y), building.rect.width, 20.0f, building.rect.height,
                  BLACK);
}
