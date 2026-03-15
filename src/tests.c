
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "tests.h"

#include "carena.h"

#include "path_ways.h"
#include "raylib.h"
#include "utils.h"

void test_ajd_graph_list()
{
    Arena scratch = arena_init(10 << 20);
    PathGraph g = pathGraphInit(&scratch, 16);

    for (int i = 0; i < 10; i++)
    {
        addPathNode(&g, createPathNode(&scratch, VEC2(GetRandomValue(1, 100), GetRandomValue(1, 100))));
    }

    for (int i = 0; i < GetRandomValue(10, 100); i++)
    {
        size_t src_index = GetRandomValue(0, 9);
        size_t dest_index = GetRandomValue(0, 9);
        if (src_index == dest_index)
            continue;
        connectPathNode(&g, src_index, dest_index);
    }

    for (size_t i = 0; i < g.size; i++)
    {
        PathNodeRef node = getPathNode(&g, i);
        for (size_t j = 0; j < node->edgeCount; j++)
        {
            PathNodeRef neigh = g.nodes[node->neighbours[j]];
            printf("(neigh->pos.x = %f,", neigh->pos.x);
            printf("neigh->pos.y = %f) -> ", neigh->pos.y);
        }
        printf("\n");
    }

    arena_destroy(&scratch);
}

void run_tests(void)
{
    test_ajd_graph_list();
}
