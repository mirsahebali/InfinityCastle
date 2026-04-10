/*******************************************************************************************
 *
 *   Infinity Castle
 *
 *
 *   This game has been created using raylib (www.raylib.com)
 *   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
 *
 *   Raylib credits:
 *   Copyright (c) 2021 Ramon Santamaria (@raysan5)
 *
 *  Project credits:
 *  Copyright (c) 2026 Mir Saheb Ali (@mirsahebali)
 ********************************************************************************************/

#include "culling.h"
#include "mtypes.h"
#include "planes.h"
#include "raylib.h"
#include "raymath.h"
#include "rays.h"
#include "rlgl.h"

#include "external/stb_perlin.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "global_state.h"

#include "buildings.h"
#include "map.h"
#include "path_ways.h"
#include "utils.h"

#define CARENA_IMPLEMENTATION
#include "carena.h"

#include "external/stb_perlin.h"

#if defined(RUN_TESTS)
#include "tests.h"
#endif

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif
static const int SCREEN_WIDTH = 1920;
static const int SCREEN_HEIGHT = 1080;
static const float FAR_PLANE = 3000.0f;
static const float NEAR_PLANE = 1.0f;
static const float CAMERA_MOVEMENT_SPEED = 400.0f;
static const float MOUSE_SENSITIVITY = 0.09f;
static const float ZOOM_SCALING = 100.0f;

// Window initialization and global state setup
static void GameInit(void);

static void GameLoop(void);
// Input handling is done here
static void GameControls(void);
// Data update on each frame
static void GameUpdate(void);
// rendering logic
static void GameDraw(void);
// Cleanup
static void GameEnd(void);

// INFO: global mutable shared state from 'global_state.h'
float mouseSensitivity = 1.0f;
Camera3D camera3D = {0};
Camera2D camera2D = {0};
float mapWidth = 0.0f;
float mapHeight = 0.0f;
Vector2 mapOffset = {0};
int rngSeed = 0;
int chunkSize = 400;
float delta = 0.0f;

int main(void)
{
#if defined(RUN_TESTS)
    runTests();
    return 0;
#endif

    GameInit();
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(GameLoop, 60, 1);
#else
    // SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        GameLoop();
    }
#endif
    GameEnd();

    return 0;
}

#define MAX_RAYS 20
#define MAX_RAY_DISTANCE 1000
#define MAX_CHUNKS (MAX_CAMERA_RAYS * MAX_RAY_INTERSECTIONS)

// Localized shared variables

// Map mutation tracker
bool isMapChanged = false;
// Zoom scale of the 2D map
float mapScale = 0.0f;
// Global Arena
Arena arena = {0};
// Array of buildings to be rendered(arena allocated)
BuildingArray buildings = NULL;
const float CAMERA_2D_MOVEMENT_SPEED = 100.0f;
i32 buildingXDirCount = 10;
i32 buildingYDirCount = 10;
i32 chunkCount = 0;
u32 maxChunks = 0;
float offsetChange = 1.0f;
float aspect = SCREEN_WIDTH / (float)SCREEN_HEIGHT;
ChunkValue2D cells[MAX_CHUNKS] = {0};
Frustum cameraFrustum = {0};

static void GameInit(void)
{
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    // Initialization:
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Infinity Castle");
    rlEnableDepthTest();
    rlEnableBackfaceCulling();

    InitAudioDevice(); // Initialize audio device

    mouseSensitivity = 1.0f;

    rlSetClipPlanes(NEAR_PLANE, FAR_PLANE);

    camera3D.position = VEC3(0, 0, 400);
    camera3D.target = VEC3(0, 2, 20);
    camera3D.up = VEC3(0, 1, 0);
    camera3D.fovy = 45.0f;
    camera3D.projection = CAMERA_PERSPECTIVE;

    camera2D.target = VEC2(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0);
    camera2D.offset = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    camera2D.rotation = 0.0f;
    camera2D.zoom = 1.0f;

    mapWidth = SCREEN_HEIGHT;
    mapHeight = SCREEN_HEIGHT;
    mapOffset = VEC2(0, 0);
    rngSeed = 100;
    mapScale = 0.5f;

    arena = arena_init(20 << 20); // 20 MB
    cameraFrustum = createFrustumFromCamera(camera3D, SCREEN_WIDTH/(float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);

}

// Update and draw game frame
static void GameLoop(void)
{
    delta = GetFrameTime();
    GameControls();
    GameUpdate();
    GameDraw();
}

static void GameControls(void)
{
    camera2D.target.x -= IsKeyDown(KEY_H) * CAMERA_2D_MOVEMENT_SPEED * delta;
    camera2D.target.y -= IsKeyDown(KEY_J) * CAMERA_2D_MOVEMENT_SPEED * delta;
    camera2D.target.y += IsKeyDown(KEY_K) * CAMERA_2D_MOVEMENT_SPEED * delta;
    camera2D.target.x += IsKeyDown(KEY_L) * CAMERA_2D_MOVEMENT_SPEED * delta;

    // Uses log scaling to provide consistent zoom speed
    camera2D.zoom = expf(logf(camera2D.zoom) + ((float)GetMouseWheelMove() * 0.1f));
    if (camera2D.zoom > 3.0f)
        camera2D.zoom = 3.0f;
    else if (camera2D.zoom < 0.1f)
        camera2D.zoom = 0.1f;

    // camera2D reset (zoom and rotation)
    if (IsKeyPressed(KEY_R))
    {
        camera2D.zoom = 1.0f;
        camera2D.rotation = 0.0f;

        camera3D.position = VEC3(0, 0, 400);
        camera3D.target = VEC3(0, 2, 20);
        camera3D.up = VEC3(0, 1, 0);
        camera3D.fovy = 45.0f;
        camera3D.projection = CAMERA_PERSPECTIVE;
    }

    if (IsKeyPressed(KEY_P))
    {
        camera3D.projection = camera3D.projection == CAMERA_PERSPECTIVE ? CAMERA_ORTHOGRAPHIC : CAMERA_PERSPECTIVE;
    }

    if (IsKeyDown(KEY_EQUAL))
    {
        mapScale += 0.01f;
        isMapChanged = true;
    }

    if (IsKeyDown(KEY_MINUS))
    {
        mapScale -= 0.01f;
        isMapChanged = true;
    }

    if (IsKeyDown(KEY_L))
    {
        isMapChanged = true;
        mapOffset.x += offsetChange * delta;
    }

    if (IsKeyDown(KEY_H))
    {
        isMapChanged = true;
        mapOffset.x -= offsetChange * delta;
    }

    if (IsKeyDown(KEY_J))
    {
        isMapChanged = true;
        mapOffset.y -= offsetChange * delta;
    }

    if (IsKeyDown(KEY_K))
    {
        isMapChanged = true;
        mapOffset.y += offsetChange * delta;
    }

    UpdateCameraPro(
        &camera3D,
        VEC3((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) * (CAMERA_MOVEMENT_SPEED * delta) - // Move forward-backward
             (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) * (CAMERA_MOVEMENT_SPEED * delta),
             (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) * (CAMERA_MOVEMENT_SPEED * delta) - // Move right-left
             (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) * (CAMERA_MOVEMENT_SPEED * delta),
             IsKeyDown(KEY_K) * (CAMERA_MOVEMENT_SPEED * delta) - // Move up-down
             IsKeyDown(KEY_J) * (CAMERA_MOVEMENT_SPEED * delta)), 
        VEC3(GetMouseDelta().x * MOUSE_SENSITIVITY,                   // Rotation: yaw
             GetMouseDelta().y * MOUSE_SENSITIVITY,                   // Rotation: pitch
             0.0f                                                     // Rotation: roll
             ),
        GetMouseWheelMove() * ZOOM_SCALING); // Move to target (zoom)
}
static void GameUpdate(void)
{
    if (isMapChanged)
    {
        isMapChanged = false;
    }

    cameraFrustum = createFrustumFromCamera(camera3D, SCREEN_WIDTH/(float)SCREEN_HEIGHT, NEAR_PLANE, FAR_PLANE);
}

static void GameDraw(void)
{
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        BeginMode3D(camera3D);
        {
            chunkCount = 0;
            i16 minX = floorf((camera3D.position.x - FAR_PLANE) / (float)chunkSize);
            i16 maxX = floorf((camera3D.position.x + FAR_PLANE) / (float)chunkSize);

            i16 minY = floorf((camera3D.position.y - FAR_PLANE) / (float)chunkSize);
            i16 maxY = floorf((camera3D.position.y + FAR_PLANE) / (float)chunkSize);


            i16 minZ = floorf((camera3D.position.z - FAR_PLANE) / (float)chunkSize);
            i16 maxZ = floorf((camera3D.position.z + FAR_PLANE) / (float)chunkSize);
            for (i16 chunkCountX = minX; chunkCountX < maxX; chunkCountX++)
            {
                for (i16 chunkCountY = minY; chunkCountY < maxY; chunkCountY++)
                {
                    for (i16 chunkCountZ = minZ; chunkCountZ < maxZ; chunkCountZ++)
                    {
                        Building building = GenRandomBuilding((CellValue3D){chunkCountX, chunkCountY, chunkCountZ}, chunkSize, chunkSize, chunkSize);
                        if(IsBoxInsideFrustum(&cameraFrustum, building.box)) { chunkCount++; DrawBuildingModel(building); }
                    }
                }
            }
        }
        DrawGrid(1000, 100);
        EndMode3D();
    }
#ifndef NDEBUG

#define cameraPositionStr                                                                                              \
    TextFormat("Position: (x = %.2f, y = %.2f, z = %.2f)", camera3D.position.x, camera3D.position.y,                   \
               camera3D.position.z)
#define cameraUpStr TextFormat("Up: (x = %.2f, y = %.2f, z = %.2f)", camera3D.up.x, camera3D.up.y, camera3D.up.z)
#define cameraTargetStr                                                                                                \
    TextFormat("Target: (x = %.2f, y = %.2f, z = %.2f)", camera3D.target.x, camera3D.target.y, camera3D.target.z)
#define fovAngleStr TextFormat("Angle: %.2f", camera3D.fovy)
#define chunkStr TextFormat("Chunk Count: %d", chunkCount)
#define chunkValueStr \
  TextFormat("Chunk Value: (%d, %d, %d)", \
  (i16)(camera3D.position.x / chunkSize), \
  (i16)(camera3D.position.y / chunkSize), \
  (i16)(camera3D.position.z / chunkSize))

    const int fontSize = 25;
    const int gapY = 5;
    const int cameraPositionStrWidth = MeasureText(cameraPositionStr, fontSize);
    const int cameraTargetStrWidth = MeasureText(cameraTargetStr, fontSize);
    const int rectWidth = cameraPositionStrWidth > cameraTargetStrWidth ? cameraPositionStrWidth : cameraTargetStrWidth;
    DrawRectangle(GetScreenWidth() - rectWidth, 0, rectWidth, fontSize * 8, ColorAlpha(BLUE, 0.4f));
    DrawRectangle(GetScreenWidth() - rectWidth, 0, rectWidth, fontSize * 8, ColorAlpha(WHITE, 0.4f));
    DrawRectangleLinesEx(RECT(GetScreenWidth() - rectWidth, 0, rectWidth, fontSize * 8), 3.0f, BLUE);

    DrawText(cameraPositionStr, GetScreenWidth() - MeasureText(cameraPositionStr, fontSize), gapY, fontSize, BLACK);
    DrawText(cameraTargetStr, GetScreenWidth() - MeasureText(cameraTargetStr, fontSize), fontSize + (2 * gapY),
             fontSize, BLACK);
    DrawText(cameraUpStr, GetScreenWidth() - MeasureText(cameraUpStr, fontSize), (fontSize * 2) + (3 * gapY), fontSize,
             BLACK);
    DrawText(fovAngleStr, GetScreenWidth() - MeasureText(fovAngleStr, fontSize), (fontSize * 3) + (4 * gapY), fontSize,
             BLACK);
    DrawText(chunkStr, GetScreenWidth() - MeasureText(chunkStr, fontSize), (fontSize * 4) + (5 * gapY), fontSize,
             BLACK);
    DrawText(chunkValueStr, GetScreenWidth() - MeasureText(chunkValueStr, fontSize), (fontSize * 5) + (6 * gapY), fontSize,
             BLACK);

    DrawFPSFull(VEC2(8, 8), 20, 5, 5, BLACK);

#endif /* ifndef NDEBUG */
    EndDrawing();
}

static void GameEnd(void)
{
    arena_destroy(&arena);
    // De-Initialization:
    CloseAudioDevice();
    CloseWindow();
}
