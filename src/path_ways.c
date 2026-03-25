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

    graph.nodes = ARRAY_WITH_CAPACITY(arena, PathNode, capacity);
    return graph;
}

PathNode createPathNode(Vector2 pos)
{
    PathNode new_node = {0};
    new_node.pos = pos;
    new_node.edgeCount = 0;
    for (int i = 0; i < NEIGHBOURS_MAX; i++)
    {
        new_node.neighbours[i] = 0;
    }
    new_node.selfIndex = 0;
    return new_node;
}

// it does mutates it
void addPathNode(PathGraph *self, PathNodeRef data)
{
    assert(self != NULL);
    data->selfIndex = self->size;
    self->nodes = ARRAY_PUSH(self->nodes, PathNode, data);
    assert(self->nodes != NULL);

    assert(self->nodes != NULL);
    self->size++;
    self->capacity = ARRAY_CAPACITY(self->nodes, PathNodeRef);
}

void connectPathNode(PathGraph *self, size_t srcIndex, size_t destIndex)
{
    assert(self != NULL);
    if (self->nodes[srcIndex].edgeCount >= NEIGHBOURS_MAX || self->nodes[destIndex].edgeCount >= NEIGHBOURS_MAX)
    {
        return;
    }
    size_t src_len = self->nodes[srcIndex].edgeCount;
    size_t dest_len = self->nodes[destIndex].edgeCount;
    self->nodes[srcIndex].neighbours[src_len] = destIndex;
    self->nodes[destIndex].neighbours[dest_len] = srcIndex;
    self->nodes[srcIndex].edgeCount++;
    self->nodes[destIndex].edgeCount++;
}

PathNode getPathNode(PathGraph *graph, size_t idx)
{
    assert(graph != NULL);
    assert(idx >= 0 || idx < graph->size);

    return graph->nodes[idx];
}

void DrawPathNodes(PathGraph *graph)
{
    for (size_t i = 0; i < graph->size; i++)
    {
        PathNode node = getPathNode(graph, i);
#ifndef NDEBUG
        DrawCircleV(node.pos, 5.0, RED);
#endif /* ifndef NDEBUG */
        Vector2 startPos = node.pos;
        for (size_t j = 0; j < node.edgeCount; j++)
        {
            Vector2 endPos = getPathNode(graph, node.neighbours[j]).pos;
            Vector2 splinePoints[3] = {
                startPos, VEC2(((startPos.x + endPos.x) / 2.0) - 10, ((startPos.y + endPos.y) / 2.0) - 10), endPos};
            DrawSplineLinear(splinePoints, 3, 2.0, BLUE);
            // DrawLineEx(startPos, endPos, 2.0, BLUE);
        }
    }
}

PathGraph generateRandomGraph(Arena *arena)
{
    PathGraph pathGraph = pathGraphInit(arena, 16);

    for (int i = 0; i < 10; i++)
    {
        PathNode pathNode = createPathNode(VEC2(GetRandomValue(0, 1000), GetRandomValue(0, 1000)));
        addPathNode(&pathGraph, &pathNode);
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
PathGraph generateCircularPaths(Arena *arena, Vector2 seed_point, uint32_t depth, int angle, int direction)
{
    PathGraph graph = pathGraphInit(arena, 16);
    PathNode newNode = createPathNode(seed_point);
    addPathNode(&graph, &newNode);

    // pick a direction
    for (size_t i = 0; i < depth; i++)
    {
        PathNode parentNode = getPathNode(&graph, i);
        Vector2 nextPosL = {};
        size_t length = 100;
        nextPosL.x = parentNode.pos.x + (length * (cos(TO_RADIANS(direction))));
        nextPosL.y = parentNode.pos.y + (length * (sin(TO_RADIANS(direction))));
        direction -= angle;
        PathNode nextNodeL = createPathNode(nextPosL);
        addPathNode(&graph, &nextNodeL);
        connectPathNode(&graph, parentNode.selfIndex, nextNodeL.selfIndex);
    }

    return graph;
}
