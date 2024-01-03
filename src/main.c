#include "cable.h"
#include "entity.h"
#include "physics.h"
#include "raylib.h"
#include "pixel_perfect.h"
#include "sprite_manager.h"
#include "ultilities.h"
#include "map_loader.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int initialScreenWidth = 800;
    const int initialScreenHeight = 450; // 16:9

    InitWindow(initialScreenWidth, initialScreenHeight, "PixelJAM 24");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    
    GameColliderList collider_list = createGameColliderList();

    Player player = createPlayer();

    Cable cable = createCable((Vector2){0,100}, 10, 1000);

    RenderTexture2D WorldRenderTexture = LoadRenderTexture(getVirtualScreenWidth(), getVirtualScreenHeight());

    RenderMessage rmessage = {0};

    TileMap tileMap = createTileMap();

    initializeSprites();
    loadMap("assets/maps/map-test.png", &tileMap, &collider_list, &cable, &player);
    printf("N colliders = %zu\n", collider_list.size);
    printf("N tiles = %zu\n", tileMap.size);

    //--------------------------------------------------------------------------------------
    // Main game loop
    showTitleScreen();

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
        if (player.grounded) {
            player.canDoubleJump = true;
            if (IsKeyPressed(KEY_SPACE)) {
                player.input_vector.y -= 60;
            }
            if (IsKeyPressed(KEY_F)) {
                PLACE_ANCHOR_RESULT result = tryCreateAnchor(&cable, &collider_list, player.position);
                switch (result) {
                    case ANCHOR_SUCCESS:
                        break;
                    case ANCHOR_NOT_ENOUGH_ANCHORS:
                        addMessageToBeRendered(&rmessage, "Not enough available anchors!", 5);
                        break;
                    case ANCHOR_NOT_ENOUGH_LENGTH:
                        addMessageToBeRendered(&rmessage, "Not enough cable length!", 5);
                        break;
                    case ANCHOR_OBSTRUDED_PATH:
                        addMessageToBeRendered(&rmessage, "Something is obstructing the path!", 5);
                        break;
                }
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
                updatePlayerMovement(&player, &cable, &collider_list);
                renderTileMap(&tileMap);
                // renderCollisionCapsules(&collider_list);
                // printf("Player position = {%f, %f}\n", player.position.x, player.position.y);
                drawCable(&cable, &player);
                DrawTexture(player.sprite, player.position.x, player.position.y, WHITE);

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
            drawMessage(&rmessage, (Vector2){screenWidth*0.5, 0.45*screenHeight});
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
