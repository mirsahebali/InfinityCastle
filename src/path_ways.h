#ifndef PATH_WAYS_H
#define PATH_WAYS_H

#include <stddef.h>
#include <stdint.h>

#include "carena.h"

#include "raylib.h"

#define NEIGHBOURS_MAX 5

// Algorithmic agnostic graph path implementation -- Start
typedef struct
{
    size_t selfIndex;
    Vector2 pos;
    size_t neighbours[NEIGHBOURS_MAX];
    size_t edgeCount;
} PathNode;

typedef PathNode *PathNodeRef;

typedef PathNode *NodeList;

typedef struct
{
    size_t capacity;
    size_t size;
    NodeList nodes;
} PathGraph;

PathGraph pathGraphInit(Arena *arena, size_t capacity);

PathNode createPathNode(Vector2 pos);

void addPathNode(PathGraph *graph, PathNodeRef data);

void connectPathNode(PathGraph *graph, size_t src_index, size_t dest_index);

PathNode getPathNode(PathGraph *graph, size_t src_index);

void DrawPathNodes(PathGraph *graph);

// Algorithmic agnostic graph path implementation -- End

PathGraph generateRandomGraph(Arena *arena);

PathGraph generateCircularPaths(Arena *arena, Vector2 seed_point, uint32_t depth, int angle, int direction);

#endif // !PATH_WAYS_H
