#ifndef PATH_WAYS_H
#define PATH_WAYS_H

#include <stddef.h>

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

typedef PathNode **NodeList;

typedef struct
{
    size_t capacity;
    size_t size;
    NodeList nodes;
} PathGraph;

PathGraph pathGraphInit(Arena *arena, size_t capacity);

PathNodeRef createPathNode(Arena *arena, Vector2 pos);

void addPathNode(PathGraph *graph, PathNodeRef data);

void connectPathNode(PathGraph *graph, size_t src_index, size_t dest_index);

PathNodeRef getPathNode(PathGraph *graph, size_t src_index);

void DrawPathNodes(PathGraph *graph);

PathGraph generateRandomGraph(Arena *arena);

// Algorithmic agnostic graph path implementation -- End

PathGraph generateOrganicPaths(Arena *arena, Vector2 seed_point);

#endif // !PATH_WAYS_H
