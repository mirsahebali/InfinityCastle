
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "tests.h"

#include "carena.h"

#include "path_ways.h"
#include "raylib.h"
#include "rays.h"
#include "utils.h"

static int testCount = 0;

static inline void fnRun(const char *message, void (*func)(void));
static void endTests(void);

int cmp_uint64(const void *a, const void *b)
{
    u64 ua = *(const u64 *)a;
    u64 ub = *(const u64 *)b;

    if (ua < ub)
        return -1;
    if (ua > ub)
        return 1;
    return 0;
}

int cmp_uint32(const void *a, const void *b)
{
    u32 ua = *(const u32 *)a;
    u32 ub = *(const u32 *)b;

    if (ua < ub)
        return -1;
    if (ua > ub)
        return 1;
    return 0;
}

void testAjdGraphList()
{
    Arena scratch = arena_init(10 << 20);
    PathGraph g = pathGraphInit(&scratch, 16);

    for (int i = 0; i < 10; i++)
    {

        PathNode newNode = createPathNode(VEC2(GetRandomValue(1, 100), GetRandomValue(1, 100)));
        addPathNode(&g, &newNode);
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
        PathNode node = getPathNode(&g, i);
        for (size_t j = 0; j < node.edgeCount; j++)
        {
            PathNode neigh = g.nodes[node.neighbours[j]];
            printf("(neigh->pos.x = %f,", neigh.pos.x);
            printf("neigh->pos.y = %f) -> ", neigh.pos.y);
        }
        printf("\n");
    }

    arena_destroy(&scratch);
}

void testID64Generation(void)
{
    int width = 1000;
    int height = 1000;
    Arena arena = arena_init(80 << 20);
    uint64_t *array = ARRAY_INIT(&arena, uint64_t);
    for (int y = -height; y < height; y++)
    {
        for (int x = -width; x < width; x++)

        {
            uint64_t uID = BitPackU64(x, y);
            array = ARRAY_PUSH(array, uint64_t, &uID);
        }
    }

    int count = ARRAY_LENGTH(array, uint64_t);
    int dups = 0;

    qsort(array, count, sizeof(uint64_t), cmp_uint64);

    for (int i = 1; i < count; i++)
    {
        if (array[i] == array[i - 1])
        {
            dups++;
        }
    }

    printf("dups = %d\n", dups);
    assert(dups == 0);
    arena_destroy(&arena);
}

void testID32Generation(void)
{
    int width = 1000;
    int height = 1000;
    Arena arena = arena_init(80 << 20);
    u32 *array = ARRAY_INIT(&arena, u32);
    for (int y = -height; y < height; y++)
    {
        for (int x = -width; x < width; x++)

        {
            uint64_t uID = BitPackU32(x, y);
            array = ARRAY_PUSH(array, u32, &uID);
        }
    }

    int count = ARRAY_LENGTH(array, u32);
    int dups = 0;

    qsort(array, count, sizeof(u32), cmp_uint32);

    for (int i = 1; i < count; i++)
    {
        if (array[i] == array[i - 1])
        {
            dups++;
        }
    }

    printf("dups = %d\n", dups);
    assert(dups == 0);
    arena_destroy(&arena);
}

void testUnmaskBits(void)
{
    for (int i = 0; i < 1999; i++)
    {
        i16 left = GetRandomValue(INT16_MIN, INT16_MAX);
        i16 right = GetRandomValue(INT16_MIN, INT16_MAX);
        u32 input = BitPackU32(left, right);
        UnMaskedBitsI16 res = UnPackI16(input);
        assert(res.left == left);
        assert(res.right == right);
    }

    for (int i = 0; i < 1999; i++)
    {
        i32 left = GetRandomValue(INT32_MIN, INT32_MAX);
        i32 right = GetRandomValue(INT32_MIN, INT32_MAX);
        u64 input = BitPackU64(left, right);
        UnPackedBitsI32 res = UnPackI32(input);

        ASSERT_EQ("%d", res.left, left);
        ASSERT_EQ("%d", res.right, right);
    }
}

void testRayPlaneIntersection(void)
{
    RayIntersections intersections = GetLineCellIntersections(VEC2(89, 22), VEC2(0, 5), 1, 1, 1);

    for (i32 i = 0; i < 10; i++)
    {
        printf("( x = %d, y = %d )\n", intersections.cells[i].x, intersections.cells[i].y);
    }
}

void runTests(void)
{
    printf("Running tests...\n");
    fnRun("test adjacency list", testAjdGraphList);
    fnRun("test unique i64 id generation", testID64Generation);
    fnRun("test unique i32 id generation", testID32Generation);
    fnRun("test unique unmasking", testUnmaskBits);
    fnRun("test ray plane intersection", testRayPlaneIntersection);
    endTests();
}

static inline void fnRun(const char *message, void (*func)(void))
{
    testCount++;
    printf("------\n");
    printf("Test Started: %s\n", message);
    func();
    printf("Test Passed: %s\n", message);
    printf("------\n");
}

static void endTests(void)
{
    printf("Ran %d tests\n", testCount);
    printf("Tests Complete! :)\n");
}
