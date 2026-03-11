#include "raylib.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif
static const int screenWidth = 800;
static const int screenHeight = 450;

static void GameLoop(void);
static void GameUpdate(void);
static void GameDraw(void);

int main(void)
{
    // Initialization:
    InitWindow(screenWidth, screenHeight, "raylib game template");

    InitAudioDevice(); // Initialize audio device

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(GameLoop, 60, 1);
#else
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        GameLoop();
    }
#endif

    // De-Initialization:

    CloseAudioDevice(); // Close audio context

    CloseWindow(); // Close window and OpenGL context

    return 0;
}

// Update and draw game frame
static void GameLoop(void)
{
    GameUpdate();
    GameDraw();
}

static void GameUpdate(void)
{
}

static void GameDraw(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    EndDrawing();
}
