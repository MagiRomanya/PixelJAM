#include "cable.h"
#include "entity.h"
#include "physics.h"
#include "raylib.h"
#include "pixel_perfect.h"
#include "raymath.h"
#include "sprite_manager.h"
#include "ultilities.h"
#include "map_loader.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int initialScreenWidth = 800;
    const int initialScreenHeight = 450; // 16:9

    InitWindow(initialScreenWidth, initialScreenHeight, "PixelJAM 24");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    InitAudioDevice();
    
    GameColliderList collider_list = createGameColliderList();

    Player player = createPlayer();

    Cable cable = createCable((Vector2){0,100}, 10, 1000);

    RenderTexture2D WorldRenderTexture = LoadRenderTexture(getVirtualScreenWidth(), getVirtualScreenHeight());

    RenderMessage rmessage = {0};

    TileMap tileMap = createTileMap();

    initializeAssets();
    loadMap("assets/maps/map-test2.png", &tileMap, &collider_list, &cable, &player);
    printf("N colliders = %zu\n", collider_list.size);
    printf("N tiles = %zu\n", tileMap.size);

    //--------------------------------------------------------------------------------------
    // Main game loop
    /* showTitleScreen(); */
    /* showMenuScreen(); */
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
            player.input_vector.x += 2;
        }
        if (IsKeyDown(KEY_A)) {
            player.input_vector.x += -2;
        }
        if (player.grounded) {
            player.canDoubleJump = true;
            if (IsKeyPressed(KEY_SPACE)) {
                playerJump(&player);
            }
        }
        else if (player.canDoubleJump) {
            if (IsKeyPressed(KEY_SPACE)) {
                playerJump(&player);
                player.canDoubleJump = false;
            }
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PLACE_ANCHOR_RESULT result = tryCreateAnchor(&cable, &collider_list, computePlayerHandPosition(&player));
            switch (result) {
                case ANCHOR_SUCCESS:
                    break;
                case ANCHOR_NOT_ENOUGH_ANCHORS:
                    addMessageToBeRendered(&rmessage, "Ran out of staples!", 5);
                    break;
                case ANCHOR_NOT_ENOUGH_LENGTH:
                    addMessageToBeRendered(&rmessage, "Ran out of cable! :(", 5);
                    break;
                case ANCHOR_OBSTRUDED_PATH:
                    addMessageToBeRendered(&rmessage, "Something is obstructing the path!", 5);
                    break;
            }
        }
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            tryRemoveLastAnchor(&cable, Vector2Add(player.position, (Vector2){16,16}));
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
                drawCable(&cable, &player, &collider_list);
                /* renderCapsule(playerComputeCollider(&player)); */
                renderPlayer(&player);
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
            renderCableLengthUI(&cable, &player);
            renderAnchorsLeftUI(&cable);
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(WorldRenderTexture);
    destroyPlayer(&player);
    destroyCable(&cable);
    destroyGameColliderList(&collider_list);
    destroyGameTileMap(&tileMap);
    destroyAssets();
    CloseAudioDevice();
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
