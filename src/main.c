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

    EntityList entity_list = createEntityList();
    Entity entity;
    entity.collision_mask = CABLE_COLLIDE;
    Vector2 x1 = {50, 200};
    Vector2 x2 = {700, 200};
    entity.capsule_collider = (CapsuleCollider){x1, x2, 50};
    addEntityToList(&entity_list, &entity);

    Player player = {0};
    player.position = (Vector2){100, 100};
    {
        Vector2 x1 = {0, 30};
        Vector2 x2 = {0, 0};
        player.base_capsule_collider = (CapsuleCollider){x1, x2, 20};
    }
    player.mass = 10.0f;

    RenderTexture2D WorldRenderTexture = LoadRenderTexture(getVirtualScreenWidth(), getVirtualScreenHeight());

    // Camera target position
    float cameraX = 0;
    float cameraY = 0;

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

        playerFrameReset(&player);

        if (IsKeyDown(KEY_D)) {
            player.input_vector.x += 1;
        }
        if (IsKeyDown(KEY_A)) {
            player.input_vector.x += -1;
        }
        if (IsKeyDown(KEY_SPACE)) {
            player.input_vector.y -= 10;
        }

        // Draw
        //----------------------------------------------------------------------------------
        PixelPerfectData pp_data = computePixelPerfectData((Vector2){cameraX, cameraY});

        BeginTextureMode(WorldRenderTexture);
        {

            ClearBackground(RAYWHITE);
            BeginMode2D(pp_data.worldCamera);
            {
                DrawCircle(0.5*getVirtualScreenWidth(), getVirtualScreenHeight()/2, 10, RED);
                DrawTexture(tree, 0.5*getVirtualScreenWidth(), getVirtualScreenHeight()/2, WHITE);
            }
            EndMode2D();
        }
        EndTextureMode();
        BeginDrawing();
        {
            ClearBackground(WHITE);
            BeginMode2D(pp_data.screenSpaceCamera);
            {
                DrawTexturePro(WorldRenderTexture.texture, pp_data.worldRec, pp_data.screenSpaceRec, (Vector2){0}, 0.0f, WHITE);
            }
            EndMode2D();
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(WorldRenderTexture);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
