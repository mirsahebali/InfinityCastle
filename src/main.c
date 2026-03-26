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

#include "raylib.h"
#include "rlgl.h"

#include "external/stb_perlin.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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
static const float VIEW_DISTANCE = 10000.0f;
static const float CAMERA_MOVEMENT_SPEED = 0.2f;
static const float MOUSE_SENSITIVITY = 0.09f;
static const float ZOOM_SCALING = 2.0f;

static void GameInit(void);
static void GameLoop(void);
static void GameControls(void);
static void GameUpdate(void);
static void GameDraw(void);
static void GameEnd(void);

// INFO: global state from 'global_state.h'
float mouseSensitivity = 1.0f;
Camera3D camera3D = {0};
Camera2D camera2D = {0};
float mapWidth = 0.0f;
float mapHeight = 0.0f;
Vector2 mapOffset = {0};
int seed = 0;
int gridSpacing = 50;

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

bool isMapChanged = false;
float mapScale = 0.0f;

float delta = 0.0f;
Arena arena = {0};
BuildingArray buildings = NULL;
const float CAMERA_2D_MOVEMENT_SPEED = 100.0f;

static void GameInit(void)
{
    // Initialization:
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Infinity Castle");

    InitAudioDevice(); // Initialize audio device

    mouseSensitivity = 1.0f;
    rlSetClipPlanes(1.0f, VIEW_DISTANCE);
    // camera3D.position = VEC3(0.0f, 2.0f, 4.0f);
    camera3D.position = VEC3(0, 2, 400);
    camera3D.target = VEC3(0, 0, 0);
    camera3D.up = VEC3(0, 1, 0);
    camera3D.fovy = 60.0f;
    camera3D.projection = CAMERA_PERSPECTIVE;

    camera2D.target = VEC2(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0);
    camera2D.offset = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    camera2D.rotation = 0.0f;
    camera2D.zoom = 1.0f;

    mapWidth = SCREEN_HEIGHT;
    mapHeight = SCREEN_HEIGHT;
    mapOffset = VEC2(0, 0);
    seed = 100;

    mapScale = 0.5f;

    arena = arena_init(20 << 20); // 20 MB
    buildings = generateBuildings(&arena, 10, 10, 0, 0, gridSpacing);
    assert(ARRAY_LENGTH(buildings, Building) != 0);
}

// Update and draw game frame
static void GameLoop(void)
{
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
    }

    if (IsKeyPressed(KEY_R))
    {

        camera3D.position = VEC3(0, 2, 400);
        camera3D.target = VEC3(0, 0, 0);
        camera3D.up = VEC3(0, 1, 0);
        camera3D.fovy = 60.0f;
        camera3D.projection = CAMERA_PERSPECTIVE;
    }

    if (IsKeyPressed(KEY_P))
    {
        camera3D.projection = camera3D.projection == CAMERA_PERSPECTIVE ? CAMERA_ORTHOGRAPHIC : CAMERA_PERSPECTIVE;
        if (camera3D.projection == CAMERA_PERSPECTIVE)
        {
            camera3D.position = VEC3(0, 2, 400);
            camera3D.target = VEC3(0, 0, 0);
            camera3D.up = VEC3(0, 1, 0);
            camera3D.fovy = 60.0f;
        }
        else
        {
            camera3D.position = VEC3(0, 2, 400);
        }
    }
}

static void GameUpdate(void)
{

    delta = GetFrameTime();
    float offsetChange = 1.0f;
    if (isMapChanged)
    {
        isMapChanged = false;
    }
    UpdateCameraPro(
        &camera3D,
        VEC3((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) * CAMERA_MOVEMENT_SPEED - // Move forward-backward
                 (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) * CAMERA_MOVEMENT_SPEED,
             (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) * CAMERA_MOVEMENT_SPEED - // Move right-left
                 (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) * CAMERA_MOVEMENT_SPEED,
             IsKeyDown(KEY_K) * CAMERA_MOVEMENT_SPEED - IsKeyDown(KEY_J) * CAMERA_MOVEMENT_SPEED), // Move up-down
        VEC3(GetMouseDelta().x * MOUSE_SENSITIVITY,                                                // Rotation: yaw
             GetMouseDelta().y * MOUSE_SENSITIVITY,                                                // Rotation: pitch
             0.0f                                                                                  // Rotation: roll
             ),
        GetMouseWheelMove() * ZOOM_SCALING); // Move to target (zoom)

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
}

static void GameDraw(void)
{
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        // BeginMode2D(camera2D);
        // {
        //     DrawBuidlingsTopDownView(buildings);
        // }
        // EndMode2D();
        BeginMode3D(camera3D);
        {
            DrawBuildingModels(buildings);
            DrawGrid(1000, 10.0f);
        }
        EndMode3D();
    }
#ifndef NDEBUG
    DrawFPS(10, 10);
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
