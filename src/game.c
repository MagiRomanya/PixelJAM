#include "game.h"
#include "appliance.h"
#include "cable.h"
#include "entity.h"
#include "map_loader.h"
#include "physics.h"
#include "pixel_perfect.h"
#include "raylib.h"
#include "sprite_manager.h"
#include "ultilities.h"


static HAT_TYPE playerHat = IMPORTANT_HAT;

void SetPlayerHat(HAT_TYPE hat) { playerHat = hat; }

HAT_TYPE GetPlayerHat() { return playerHat; }

void SetNextPlayerHat() {
    playerHat = (playerHat+1) % 2;
}

bool runLevel(char* map_filename, float maxCableLength, int maxAnchors) {

    GameColliderList collider_list = createGameColliderList();

    Player player = createPlayer();

    Cable cable = createCable((Vector2){0,100}, maxAnchors, maxCableLength);

    RenderTexture2D WorldRenderTexture = LoadRenderTexture(getVirtualScreenWidth(), getVirtualScreenHeight());

    RenderMessage rmessage = {0};

    TileMap tileMap = createTileMap();

    ApplianceList applianceList = createApplianceList();
    loadMap(map_filename, &tileMap, &collider_list, &applianceList, &cable, &player);
    printf("N colliders = %zu\n", collider_list.size);
    printf("N tiles = %zu\n", tileMap.size);

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

        if (IsKeyPressed(KEY_G)) {
            SetNextPlayerHat();
        }

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
            PLACE_ANCHOR_RESULT result = tryCreateAnchor(&cable, &collider_list, &applianceList, computePlayerHandPosition(&player));
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
            tryRemoveLastAnchor(&cable, &applianceList, Vector2Add(player.position, (Vector2){16,16}));
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
                renderAppliances(&applianceList);
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
            if (!renderVictoryScreen(&applianceList)) break;
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    UnloadRenderTexture(WorldRenderTexture);
    destroyPlayer(&player);
    destroyCable(&cable);
    destroyGameColliderList(&collider_list);
    destroyGameTileMap(&tileMap);
    destroyApplianceList(&applianceList);
    return areAllAppliancesConnected(&applianceList);
}
