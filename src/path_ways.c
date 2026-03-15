#include "path_ways.h"

#include "carena.h"
#include "raylib.h"

#include "raymath.h"

#include "utils.h"
#include <math.h>
#include <stdio.h>

PathGraph pathGraphInit(Arena *arena, size_t capacity)
{
    assert(arena != NULL);
    PathGraph graph = {
        .capacity = capacity,
        .size = 0,
        .nodes = NULL,
    };

    graph.nodes = ARRAY_INIT(arena, PathNodeRef);
    return graph;
}

PathNodeRef createPathNode(Arena *arena, Vector2 pos)
{
    assert(arena != NULL);
    PathNodeRef new_node = ARENA_ALLOC(arena, PathNode, 1);
    assert(new_node != NULL);
    new_node->pos = pos;
    new_node->edgeCount = 0;
    for (int i = 0; i < NEIGHBOURS_MAX; i++)
    {
        new_node->neighbours[i] = 0;
    }
    new_node->selfIndex = 0;
    return new_node;
}

void addPathNode(PathGraph *self, PathNodeRef data)
{
    assert(self != NULL);
    ARRAY_PUSH(self->nodes, PathNodeRef, data);
    data->selfIndex = self->size;

    assert(self->nodes != NULL);
    self->size++;
    self->capacity = ARRAY_CAPACITY(self->nodes, PathNodeRef);
}

void connectPathNode(PathGraph *self, size_t srcIndex, size_t destIndex)
{
    assert(self != NULL);
    if (self->nodes[srcIndex]->edgeCount >= NEIGHBOURS_MAX || self->nodes[destIndex]->edgeCount >= NEIGHBOURS_MAX)
    {
        return;
    }
    size_t src_len = self->nodes[srcIndex]->edgeCount;
    size_t dest_len = self->nodes[destIndex]->edgeCount;
    self->nodes[srcIndex]->neighbours[src_len] = destIndex;
    self->nodes[destIndex]->neighbours[dest_len] = srcIndex;
    self->nodes[srcIndex]->edgeCount++;
    self->nodes[destIndex]->edgeCount++;
}

PathNodeRef getPathNode(PathGraph *graph, size_t idx)
{
    assert(graph != NULL);
    assert(idx >= 0 || idx < graph->size);

    return graph->nodes[idx];
}

void DrawPathNodes(PathGraph *graph)
{
    for (size_t i = 0; i < graph->size; i++)
    {
        PathNodeRef node = getPathNode(graph, i);
#ifndef NDEBUG
        DrawCircleV(node->pos, 5.0, RED);
#endif /* ifndef NDEBUG */
        Vector2 start_pos = node->pos;
        for (size_t j = 0; j < node->edgeCount; j++)
        {
            Vector2 end_pos = getPathNode(graph, node->neighbours[j])->pos;
            DrawLineEx(start_pos, end_pos, 2.0, BLUE);
        }
    }
}

PathGraph generateRandomGraph(Arena *arena)
{
    PathGraph pathGraph = pathGraphInit(arena, 16);

    for (int i = 0; i < 10; i++)
    {
        addPathNode(&pathGraph, createPathNode(arena, VEC2(GetRandomValue(0, 1000), GetRandomValue(0, 1000))));
    }

    for (int i = 0; i < GetRandomValue(10, 100); i++)
    {
        size_t srcIndex = GetRandomValue(0, 9);
        size_t destIndex = GetRandomValue(0, 9);
        if (srcIndex == destIndex)
            continue;
        connectPathNode(&pathGraph, srcIndex, destIndex);
    }

    return pathGraph;
}
typedef enum
{
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_TOP,
    DIRECTION_BOTTOM,
} Direction;
PathGraph generateOrganicPaths(Arena *arena, Vector2 seed_point)
{
    PathGraph graph = pathGraphInit(arena, 16);
    addPathNode(&graph, createPathNode(arena, seed_point));
    // pick a direction

    for (size_t i = 1; i < 4; i++)
    {
        PathNodeRef parentNode = getPathNode(&graph, i - 1);
        // 30 degree turns
        size_t directionAngle = GetRandomValue(DIRECTION_LEFT, DIRECTION_BOTTOM) * 30;
        // pick a length
        size_t length = GetRandomValue(100, 100);

        Vector2 newNodePos = VEC2(length * cos(TO_RADIANS(directionAngle)) + seed_point.x,
                                  length * sin(TO_RADIANS(directionAngle)) + seed_point.y);

#ifndef NDEBUG
        printf("directionAngle = %zu\n", directionAngle);
        printf("length = %zu\n", length);
        printf("newNodePos.x = %f\n", newNodePos.x + seed_point.x);
        printf("newNodePos.y = %f\n", newNodePos.y + seed_point.y);
#endif /* ifndef NDEBUG */

        PathNodeRef newNode = createPathNode(arena, newNodePos);
        addPathNode(&graph, newNode);
        connectPathNode(&graph, parentNode->selfIndex, newNode->selfIndex);
    }
    return graph;
}
