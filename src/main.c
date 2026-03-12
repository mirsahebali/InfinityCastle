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
 *  Game credits:
 *  Copyright (c) 2026 Mir Saheb Ali (@mirsahebali)
 ********************************************************************************************/

#include "raylib.h"

#include "stb_perlin.h"

#include <assert.h>
#include <stdio.h>

#include "global_state.h"

#include "map.h"
#include "utils.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif
static const int screenWidth = 800;
static const int screenHeight = 450;

static void GameInit(void);
static void GameLoop(void);
static void GameUpdate(void);
static void GameDraw(void);
static void GameEnd(void);

// INFO: global state from 'global_state.h'
float mouseSensitivity = 1.0f;
Camera3D camera = {0};
float mapWidth = 0.0f;
float mapHeight = 0.0f;
Vector2 mapOffset = {0};
int seed = 0;

int main(void)
{
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

    return 0;
}

Texture mapPerlinTexture = {0};
int perlinScale = 0;
bool isMapChanged = false;

static void GameInit(void)
{
    // Initialization:
    InitWindow(screenWidth, screenHeight, "Infinity Castle");

    InitAudioDevice(); // Initialize audio device

    mouseSensitivity = 1.0f;

    camera.position = VEC3(0.2, 0.4, 0.2);
    camera.target = VEC3(0.2, 0.4, 0.2);
    camera.up = VEC3(0.0f, 1.0f, 0.0f);
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    mapWidth = 100;
    mapHeight = 100;
    mapOffset = VEC2(0, 0);
    seed = 100;
    perlinScale = 5;

    Image mapPerlinImage = GenImagePerlinNoise(mapWidth, mapHeight, mapOffset.x, mapOffset.y, perlinScale);
    mapPerlinTexture = LoadTextureFromImage(mapPerlinImage);
    UnloadImage(mapPerlinImage);
}

// Update and draw game frame
static void GameLoop(void)
{
    GameUpdate();
    GameDraw();
}

static void GameUpdate(void)
{
    if (isMapChanged)
    {
        UnloadTexture(mapPerlinTexture);
        Image mapPerlinImage = GenImagePerlinNoise(mapWidth, mapHeight, mapOffset.x, mapOffset.y, perlinScale);
        mapPerlinTexture = LoadTextureFromImage(mapPerlinImage);
        UnloadImage(mapPerlinImage);
        isMapChanged = false;
    }
    UpdateCamera(&camera, CAMERA_FREE);

#ifndef NDEBUG
    printf("GetFPS() = %d\n", GetFPS());
#endif /* ifndef NDEBUG */

    if (IsKeyDown(KEY_L))
    {
        isMapChanged = true;
        mapOffset.x += 1;
    }

    if (IsKeyDown(KEY_H))
    {
        isMapChanged = true;
        mapOffset.x -= 1;
    }

    if (IsKeyDown(KEY_J))
    {
        isMapChanged = true;
        mapOffset.y -= 1;
    }

    if (IsKeyDown(KEY_K))
    {
        isMapChanged = true;
        mapOffset.y += 1;
    }
}

static void GameDraw(void)
{
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        DrawMap(mapPerlinTexture, VEC2(0, 0), GRAY);

        BeginMode3D(camera);
        {
        }
        EndMode3D();
    }
    EndDrawing();
}

static void GameEnd(void)
{
    // De-Initialization:
    UnloadTexture(mapPerlinTexture);
    CloseAudioDevice();
    CloseWindow();
}
