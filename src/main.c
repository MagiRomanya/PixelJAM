#include "physics.h"
#include "raylib.h"
#include "pixel_perfect.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>

#define VIRTUAL_SCREEN_WIDTH 160
#define VIRTUAL_SCREEN_HEIGHT 90

//-----------------------------------------------------------------------------------
// Enum  for game screens 
//-----------------------------------------------------------------------------------
typedef enum GameSreen { OPENING = 0, TITLE, GAMEPLAY, ENDING } GameScreen;
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int initialScreenWidth = 800;
    const int initialScreenHeight = 450; // 16:9

    InitWindow(initialScreenWidth, initialScreenHeight, "PixelJAM 24");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    
    GameScreen currentScreen = OPENING;
    int frameCounter = 0;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    Texture2D tree = LoadTexture("assets/sprites/Tree001.png");

    PhysicsState state = allocate_physics_state(3*2);
    state.x[0] = 100;
    state.x[1] = 100;

    state.x[2] = 300;
    state.x[3] = 100;

    state.x[4] = 500;
    state.x[5] = 100;

    //--------------------------------------------------------------------------------------
    // Main game loop
    SetTargetFPS(120);
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        {
            ClearBackground(WHITE);
            solve_physics(&state);
            const Color colors[] = {BLUE, GREEN, RED};
            for (size_t i = 0; i < state.n_dof / 2; i++) {
                Vector2 x = {state.x[2*i+0], state.x[2*i+1]};
                DrawCircle(x.x, x.y, 10, colors[i]);
            }

        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
