#include "game.h"
#include "appliance.h"
#include "cable.h"
#include "entity.h"
#include "map_loader.h"
#include "physics.h"
#include "pixel_perfect.h"
#include "raylib.h"
#include "raymath.h"
#include "sprite_manager.h"
#include "ultilities.h"


static HAT_TYPE playerHat = IMPORTANT_HAT;

void SetPlayerHat(HAT_TYPE hat) { playerHat = hat; }

HAT_TYPE GetPlayerHat() { return playerHat; }

void SetNextPlayerHat() {
    playerHat = (playerHat+1) % 6;
}

// Draw part of a texture (defined by a rectangle) with rotation and scale tiled into dest.
void DrawTextureTiled(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, float scale, Color tint)
{
    if ((texture.id <= 0) || (scale <= 0.0f)) return;  // Wanna see a infinite loop?!...just delete this line!
    if ((source.width == 0) || (source.height == 0)) return;

    int tileWidth = (int)(source.width*scale), tileHeight = (int)(source.height*scale);
    if ((dest.width < tileWidth) && (dest.height < tileHeight))
    {
        // Can fit only one tile
        DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)dest.width/tileWidth)*source.width, ((float)dest.height/tileHeight)*source.height},
                    (Rectangle){dest.x, dest.y, dest.width, dest.height}, origin, rotation, tint);
    }
    else if (dest.width <= tileWidth)
    {
        // Tiled vertically (one column)
        int dy = 0;
        for (;dy+tileHeight < dest.height; dy += tileHeight)
        {
            DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)dest.width/tileWidth)*source.width, source.height}, (Rectangle){dest.x, dest.y + dy, dest.width, (float)tileHeight}, origin, rotation, tint);
        }

        // Fit last tile
        if (dy < dest.height)
        {
            DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)dest.width/tileWidth)*source.width, ((float)(dest.height - dy)/tileHeight)*source.height},
                        (Rectangle){dest.x, dest.y + dy, dest.width, dest.height - dy}, origin, rotation, tint);
        }
    }
    else if (dest.height <= tileHeight)
    {
        // Tiled horizontally (one row)
        int dx = 0;
        for (;dx+tileWidth < dest.width; dx += tileWidth)
        {
            DrawTexturePro(texture, (Rectangle){source.x, source.y, source.width, ((float)dest.height/tileHeight)*source.height}, (Rectangle){dest.x + dx, dest.y, (float)tileWidth, dest.height}, origin, rotation, tint);
        }

        // Fit last tile
        if (dx < dest.width)
        {
            DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)(dest.width - dx)/tileWidth)*source.width, ((float)dest.height/tileHeight)*source.height},
                        (Rectangle){dest.x + dx, dest.y, dest.width - dx, dest.height}, origin, rotation, tint);
        }
    }
    else
    {
        // Tiled both horizontally and vertically (rows and columns)
        int dx = 0;
        for (;dx+tileWidth < dest.width; dx += tileWidth)
        {
            int dy = 0;
            for (;dy+tileHeight < dest.height; dy += tileHeight)
            {
                DrawTexturePro(texture, source, (Rectangle){dest.x + dx, dest.y + dy, (float)tileWidth, (float)tileHeight}, origin, rotation, tint);
            }

            if (dy < dest.height)
            {
                DrawTexturePro(texture, (Rectangle){source.x, source.y, source.width, ((float)(dest.height - dy)/tileHeight)*source.height},
                    (Rectangle){dest.x + dx, dest.y + dy, (float)tileWidth, dest.height - dy}, origin, rotation, tint);
            }
        }

        // Fit last column of tiles
        if (dx < dest.width)
        {
            int dy = 0;
            for (;dy+tileHeight < dest.height; dy += tileHeight)
            {
                DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)(dest.width - dx)/tileWidth)*source.width, source.height},
                        (Rectangle){dest.x + dx, dest.y + dy, dest.width - dx, (float)tileHeight}, origin, rotation, tint);
            }

            // Draw final tile in the bottom right corner
            if (dy < dest.height)
            {
                DrawTexturePro(texture, (Rectangle){source.x, source.y, ((float)(dest.width - dx)/tileWidth)*source.width, ((float)(dest.height - dy)/tileHeight)*source.height},
                    (Rectangle){dest.x + dx, dest.y + dy, dest.width - dx, dest.height - dy}, origin, rotation, tint);
            }
        }
    }
}

SCREEN runLevel(char* map_filename, float maxCableLength, int maxAnchors, SCREEN nextScreen) {
    SCREEN currentScreen = QUIT_GAME;
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

                Rectangle sourceBackground = {0,0,128,128};
                Rectangle destBackground = {0,0,128*10,128*10};
                Vector2 origin = {300, 300};
                DrawTextureTiled(getSpriteFromID(SPRITE_BRICK_BACKGROUND_ID), sourceBackground, destBackground, origin, 0, 1.0f, WHITE);

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
    bool areAllConnected = areAllAppliancesConnected(&applianceList);
    UnloadRenderTexture(WorldRenderTexture);
    destroyPlayer(&player);
    destroyCable(&cable);
    destroyGameColliderList(&collider_list);
    destroyGameTileMap(&tileMap);
    destroyApplianceList(&applianceList);
    if (areAllConnected) {
        return nextScreen;
    }
    else return QUIT_GAME;
}
