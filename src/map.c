#include <stdio.h>

#include "map.h"

#include "global_state.h"

#include "utils.h"

#include "buildings.h"

const int MAP_GRID_LINE_WIDTH = 1;

void DrawMapGrid(Vector2 pos, int width, int height, int lines, Color border);
void DrawMap(BuildingType mapData[MAP_MAX][MAP_MAX], Vector2 pos, int width, int height, int seed, Color border)
{
    int spacing = width / MAP_MAX;
    for (int y = 0; y < MAP_MAX; y++)
    {
        for (int x = 0; x < MAP_MAX; x++)
        {
            Color buildingColor = BLANK;
            // printf("mapData[y][x] = %u\n", mapData[y][x]);
            switch (mapData[y][x])
            {
            case BUILDING_TYPE_NONE:
                buildingColor = BLANK;
                continue;
            case BUILDING_TYPE_1:
                buildingColor = BLUE;
                goto render_rect;
            case BUILDING_TYPE_2:
                buildingColor = RED;
                goto render_rect;
            case BUILDING_TYPE_3:
                buildingColor = GREEN;
                goto render_rect;
            case BUILDING_TYPE_4:
                buildingColor = PINK;
                goto render_rect;
            case BUILDING_TYPE_5:
                buildingColor = GRAY;
                goto render_rect;
            case BUILDING_TYPE_6:
                buildingColor = BLACK;
                goto render_rect;
            case BUILDING_TYPE_7:
                buildingColor = YELLOW;
                goto render_rect;
            case BUILDING_TYPE_COUNT:
                buildingColor = BLANK;
                continue;
            default:
                buildingColor = BLANK;
                continue;
            }

        render_rect:
            DrawRectangle(pos.x + (spacing * x), pos.y + (spacing * y), spacing, spacing, buildingColor);
        }
    }
#ifndef NDEBUG
    DrawMapGrid(pos, width, height, MAP_MAX, border);
#endif /* ifndef  NDEBUG */
}

void DrawMapGrid(Vector2 pos, int width, int height, int lines, Color border)
{
    Vector2 end_pos = VEC2(pos.x + width, pos.y + height);
    int spacing = width / lines;
    for (int x = pos.x; x <= end_pos.x;)
    {
        DrawLineEx(VEC2(x, pos.y), VEC2(x, pos.y + height), MAP_GRID_LINE_WIDTH, border);
        x += spacing;
    }

    for (int y = pos.y; y <= end_pos.y;)
    {
        DrawLineEx(VEC2(pos.x, y), VEC2(pos.x + width, y), MAP_GRID_LINE_WIDTH, border);
        y += spacing;
    }
}
