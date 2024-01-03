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

    GameColliderList collider_list = createGameColliderList();
    GameCollider entity = {0};
    entity.collision_mask = PLAYER_CABLE_COLLIDE;
    entity.friction_damping = 1.0f;
    Vector2 x1 = {-100, 150};
    Vector2 x2 = {100, 150};
    entity.capsule_collider = (CapsuleCollider){x1, x2, 20};
    addGameColliderToList(&collider_list, &entity);

    Player player = createPlayer();

    RenderTexture2D WorldRenderTexture = LoadRenderTexture(getVirtualScreenWidth(), getVirtualScreenHeight());

    // Camera target position
    float cameraX = 0;
    float cameraY = 0;

    //--------------------------------------------------------------------------------------
    // Main game loop
    SetTargetFPS(150);
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
        if (player.grounded) {
            player.canDoubleJump = true;
            if (IsKeyPressed(KEY_SPACE)) {
                player.input_vector.y -= 60;
            }
        }
        else if (player.canDoubleJump) {
            if (IsKeyPressed(KEY_SPACE)) {
                player.input_vector.y -= 60;
                player.canDoubleJump = false;
            }
        }

        // Draw
        //----------------------------------------------------------------------------------
        Vector2 cameraPosition = Vector2Add(player.position, (Vector2){-getVirtualScreenWidth()/2.0 + 16,-0.5*getVirtualScreenHeight()});
        PixelPerfectData pp_data = computePixelPerfectData(cameraPosition);

        BeginTextureMode(WorldRenderTexture);
        {

            ClearBackground(RAYWHITE);
            BeginMode2D(pp_data.worldCamera);
            {
                updatePlayerMovement(&player, &collider_list);
                DrawTexture(player.sprite, player.position.x, player.position.y, WHITE);

                renderCapsule(entity.capsule_collider);
                /* renderCapsule(playerComputeCollider(&player)); */
                /* printf("Player position = {%f, %f}\n", player.position.x, player.position.y); */
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
    destroyPlayer(&player);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
