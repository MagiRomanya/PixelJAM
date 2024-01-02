#include "entity.h"
#include "physics.h"
#include "raylib.h"
#include "pixel_perfect.h"
#include "ultilities.h"
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

    PhysicsState state = allocate_physics_state(4*2);
    state.x[0] = 100;
    state.x[1] = 100;

    state.x[2] = 300;
    state.x[3] = 50;

    state.x[4] = 500;
    state.x[5] = 100;

    state.x[6] = 700;
    state.x[7] = 100;

    EntityList entity_list = createEntityList();
    Entity entity;
    entity.collision_mask = CABLE_COLLIDE;
    Vector2 x1 = {50, 200};
    Vector2 x2 = {700, 200};
    entity.capsule_collider = (CapsuleCollider){x1, x2, 50};
    addEntityToList(&entity_list, &entity);

    bool pause = true;
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
            if (IsKeyPressed(KEY_P)) {
                pause = !pause;
            }
            if (pause) {
                if (IsKeyPressed(KEY_N)) {
                    ClearBackground(WHITE);
                    solve_physics(&state, &entity_list);
                }
            }
            else {
                ClearBackground(WHITE);
                solve_physics(&state, &entity_list);
            }
            for (size_t i = 0; i < entity_list.size; i++) {
                Entity* e = getEntityFromList(&entity_list, i);
                renderCapsule(&e->capsule_collider);
            }
            const Color colors[] = {BLUE, GREEN, RED, YELLOW, MAROON};
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
