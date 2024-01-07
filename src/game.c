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
#include <math.h>
#include <stddef.h>


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

    unsigned long frameCounter = 0;
    const int backgroundID = GetRandomValue(SPRITE_DIAMONDS_BACKGROUND_ID, SPRITE_HEARTS_BACKGROUND_ID);

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

        bool canPlaceAnchor = !areAllAppliancesConnected(&applianceList) && frameCounter > 3;
        frameCounter++;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && canPlaceAnchor) {
            PLACE_ANCHOR_RESULT result = tryCreateAnchor(&cable, &collider_list, &applianceList, computePlayerHandPosition(&player));
            const float messageDuration = 2;
            switch (result) {
                case ANCHOR_SUCCESS:
                    break;
                case ANCHOR_NOT_ENOUGH_ANCHORS:
                    addMessageToBeRendered(&rmessage, "Ran out of staples!", messageDuration);
                    break;
                case ANCHOR_NOT_ENOUGH_LENGTH:
                    addMessageToBeRendered(&rmessage, "Ran out of cable! :(", messageDuration);
                    break;
                case ANCHOR_OBSTRUDED_PATH:
                    addMessageToBeRendered(&rmessage, "Something is obstructing the path!", messageDuration);
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
                DrawTextureTiled(getSpriteFromID(backgroundID), sourceBackground, destBackground, origin, 0, 1.0f, WHITE);

                updatePlayerMovement(&player, &cable, &collider_list);
                renderTileMap(&tileMap);
                // renderCollisionCapsules(&collider_list);
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
            /* DrawFPS(50, 50); */
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

    stopAppliancesSounds();

    if (areAllConnected) {
        return nextScreen;
    }
    else return QUIT_GAME;
}

SCREEN showTitleScreen() {
    const float animation_screen_duration = 4.0f;
    const float title_screen_duration = 5.0f;
    const int MaxFrames = 5 * 60;
    size_t frameNumber = 0;
    // Title screen
    Texture2D titleScreen = LoadTexture("assets/sprites/titlescreen.png");
    SetTargetFPS(60);
    PlaySound(getSoundTrackFromID(SOUND_TRACK_INTRO_EFFECT_ID));
    while (!WindowShouldClose()) {
        frameNumber++;
        float titleTime = frameNumber*GetFrameTime();
        if (frameNumber == MaxFrames / 4) PlaySound(getSoundTrackFromID(SOUND_TRACK_PTERODACTYL_ID));
        if (MaxFrames < frameNumber) break;
        BeginDrawing();
        {
            ClearBackground(BLACK);
            Rectangle source = {0, 0, 1920, 1080};
            Rectangle destination = {0,0,GetScreenWidth(), GetScreenHeight()};
            float effect = (titleTime*titleTime) / (animation_screen_duration*animation_screen_duration);
            if(frameNumber/60.0 > animation_screen_duration)
                effect = 0.0;
            Color color = {255.0, 255.0, 255.0, 255.0*effect};

            DrawTexturePro(titleScreen, source, destination, (Vector2){0}, 0, color);
        }
        EndDrawing();
    }
    UnloadTexture(titleScreen);
    if (WindowShouldClose()) return QUIT_GAME;
    return MENU_SCREEN;
}

void drawOscilatingAppliance(int textureID, Vector2 position, Vector2 amplitude, Vector2 phase, Vector2 frequency) {
    static int counter = 0;
    Texture2D sprite = getSpriteFromID(textureID);
    const float virtualRatio = getVirtualRatio();
    Rectangle source = {0,0, sprite.width, sprite.height};
    Rectangle dest = {0,0, sprite.width*virtualRatio, sprite.height*virtualRatio};
    Vector2 displacement = Vector2Multiply(amplitude, (Vector2) {sinf(((float) counter)*frequency.x + phase.x), sinf(((float) counter)*frequency.y + phase.y)});
    Vector2 pos = Vector2Add(position, displacement);
    DrawTexturePro(sprite, source, dest, Vector2Negate(pos), 0, WHITE);
    counter++;
}


SCREEN showMenuScreen() {
    SCREEN currentScreen;
    size_t frameNumber = 0;
    Color buttonColPlay = GRAY;
    Color buttonColCtrl = GRAY;
    Color buttonColQuit = GRAY;

    Sound menuMusicTrack = LoadSound("assets/sound/menu-music.wav");

    size_t buttonHeight = 50;
    size_t buttonWidth = 200;
    Texture2D menuScreen = LoadTexture("assets/sprites/game-title.png");

    PlaySound(menuMusicTrack);
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        ClearBackground(BLACK);
        const Rectangle gameTitleSourceRec = {0, 0, 128, 128};
        const float gameTitleWidth = GetScreenWidth() / 3.0f;
        const Vector2 gameTitlePosition = {-gameTitleWidth, 0};
        const Rectangle gameTitleDestRec = {0,0, gameTitleWidth, gameTitleWidth};

        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        size_t centerx =screenWidth/2.0;
        size_t centery = screenHeight * 0.66f;
        Rectangle buttonPlay = {centerx - buttonWidth/2.0, centery - buttonHeight/2.0 - 2.0*buttonHeight, buttonWidth, buttonHeight};
        Rectangle buttonCtrl = {centerx - buttonWidth/2.0, centery - buttonHeight/2.0, buttonWidth, buttonHeight};
        Rectangle buttonQuit = {centerx - buttonWidth/2.0, centery - buttonHeight/2.0 + 2.0* buttonHeight, buttonWidth, buttonHeight};


        if(CheckCollisionPointRec(GetMousePosition(), buttonPlay)) {
            buttonColPlay = RED;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = LEVEL_1;
                break;
            }
        }
        else  buttonColPlay = GRAY;
        if(CheckCollisionPointRec(GetMousePosition(), buttonCtrl)) {
            buttonColCtrl = RED;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = CONTROLS_SCREEN;
                break;
            }
        }
        else
            buttonColCtrl = GRAY;
        if(CheckCollisionPointRec(GetMousePosition(), buttonQuit)) {
            buttonColQuit = RED;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = QUIT_GAME;
                break;
            }
        }
        else
            buttonColQuit = GRAY;

        BeginDrawing();
        {
            ClearBackground(GOLD);

            {
                Texture2D bg = getSpriteFromID(SPRITE_POKER_BACKGROUND_ID);
                const float virtualRatio = getVirtualRatio();
                Rectangle bgSource = {0,0, bg.width, bg.height};
                Rectangle bgDest = {0, 0, bg.width*virtualRatio, bg.height*virtualRatio};
                const float speedX = fmod(frameNumber * 0.6f, bg.width);
                const float speedY = fmod(frameNumber * 0.6f, bg.height);
                Vector2 bgOrigin = {-128 + speedX, -128 - speedY};
                Vector2 imageGrid = {4,4};
                /* DrawTexturePro(bg, bgSource, bgDest, bgOrigin, 0, WHITE); */
                for (size_t i = 0; i < imageGrid.x; i++) {
                    for (size_t j = 0; j < imageGrid.y; j++) {
                        Vector2 offset = {bg.width * i + bgOrigin.x, bg.height * j + bgOrigin.y};
                        DrawTexturePro(bg, bgSource, bgDest, Vector2MultiplyS(-virtualRatio,offset), 0, WHITE);
                    }
                }
            }

            const float time = frameNumber * GetFrameTime();

            drawOscilatingAppliance(SPRITE_WASHING_MACHINE_OFF_ID,
                                    (Vector2){screenWidth * 0.2f, screenHeight * 0.6f},
                                    (Vector2){screenWidth * 0.2f, screenHeight * 0.1f},
                                    (Vector2){0.0f, 90.0f},
                                    (Vector2){0.02f, 0.02f});

            drawOscilatingAppliance(SPRITE_TELEVISION_OFF_ID,
                                    (Vector2){screenWidth * 0.8f, screenHeight * 0.3f},
                                    (Vector2){screenWidth * 0.1f, screenHeight * 0.2f},
                                    (Vector2){0.0f, 90.0f},
                                    (Vector2){0.01f, 0.03f});

            drawOscilatingAppliance(SPRITE_BLENDER_OFF_STAGE1_ID,
                                    (Vector2){screenWidth * 0.8f, screenHeight * 0.8f},
                                    (Vector2){screenWidth * 0.14f, screenHeight * 0.1f},
                                    (Vector2){0.0f, 180.0f},
                                    (Vector2){0.02f, 0.01f});

            DrawTexturePro(menuScreen, gameTitleSourceRec, gameTitleDestRec, gameTitlePosition, 0, WHITE);

            // Buttons
            DrawRectangleRec(buttonPlay, buttonColPlay);
            drawTextInsideRectangle(buttonPlay, "Play Game", 20, WHITE);
            DrawRectangleRec(buttonCtrl, buttonColCtrl);
            drawTextInsideRectangle(buttonCtrl, "Controls", 20, WHITE);
            DrawRectangleRec(buttonQuit, buttonColQuit);
            drawTextInsideRectangle(buttonQuit, "Quit Game", 20, WHITE);
            frameNumber++;
        }
        EndDrawing();
    }
    StopSound(menuMusicTrack);
    UnloadSound(menuMusicTrack);
    UnloadTexture(menuScreen);
    if (WindowShouldClose()) {
        return QUIT_GAME;
    };
    return currentScreen;
}


SCREEN showCreditsScreen() {
    // Title screen
    SCREEN currentScreen = QUIT_GAME;
    SetTargetFPS(60);
    Texture2D creditsScreen = LoadTexture("assets/sprites/credits.png");

    while (!WindowShouldClose()) {
        if (GetRandomValue(0, 1000) == 1) {
            PlaySound(getSoundTrackFromID(SOUND_TRACK_PTERODACTYL_ID));
        }
        BeginDrawing();
        {
            ClearBackground(BLACK);
            Rectangle source = {0, 0, 1920, 1080};
            Rectangle destination = {0,0,GetScreenWidth(), GetScreenHeight()};
            DrawTexturePro(creditsScreen, source, destination, (Vector2){0}, 0, WHITE);

            size_t centerx = GetScreenWidth() * 0.6f;
            size_t centery = GetScreenHeight() * 0.6;
            size_t buttonHeight = 50;
            size_t buttonWidth = 200;
            Rectangle buttonMenu = {centerx - buttonWidth/2.0, centery - buttonHeight/2.0, buttonWidth, buttonHeight};
            Rectangle buttonQuit = {centerx - buttonWidth/2.0, centery - buttonHeight/2.0 + 2.0* buttonHeight, buttonWidth, buttonHeight};
            if (CheckCollisionPointRec(GetMousePosition(), buttonMenu)) {
                DrawRectangleRec(buttonMenu, RED);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    currentScreen = MENU_SCREEN;
                    break;
                }
            }
            else {
                DrawRectangleRec(buttonMenu, GRAY);
            }
            drawTextInsideRectangle(buttonMenu, "Back to menu", 20, WHITE);

            if (CheckCollisionPointRec(GetMousePosition(), buttonQuit)) {
                DrawRectangleRec(buttonQuit, RED);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    currentScreen = QUIT_GAME;
                    break;
                }
            }
            else {
                DrawRectangleRec(buttonQuit, GRAY);
            }
            drawTextInsideRectangle(buttonQuit, "Quit", 20, WHITE);
        }
        EndDrawing();
    }
    UnloadTexture(creditsScreen);
    return currentScreen;
}


SCREEN showControlsScreen() {
    // Title screen
    SCREEN currentScreen = QUIT_GAME;
    SetTargetFPS(60);
    Texture2D creditsScreen = LoadTexture("assets/sprites/controls.png");

    while (!WindowShouldClose()) {
        if (GetRandomValue(0, 1000) == 1) {
            PlaySound(getSoundTrackFromID(SOUND_TRACK_PTERODACTYL_ID));
        }
        BeginDrawing();
        {
            ClearBackground(BLACK);
            Rectangle source = {0, 0, 1920, 1080};
            Rectangle destination = {0,0,GetScreenWidth(), GetScreenHeight()};
            DrawTexturePro(creditsScreen, source, destination, (Vector2){0}, 0, WHITE);

            size_t centerx = GetScreenWidth() * 0.8f;
            size_t centery = GetScreenHeight() * 0.8f;
            size_t buttonHeight = 50;
            size_t buttonWidth = 200;
            Rectangle buttonMenu = {centerx - buttonWidth/2.0, centery - buttonHeight/2.0, buttonWidth, buttonHeight};
            Rectangle buttonQuit = {centerx - buttonWidth/2.0, centery - buttonHeight/2.0 + 2.0* buttonHeight, buttonWidth, buttonHeight};
            if (CheckCollisionPointRec(GetMousePosition(), buttonMenu)) {
                DrawRectangleRec(buttonMenu, RED);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    currentScreen = MENU_SCREEN;
                    break;
                }
            }
            else {
                DrawRectangleRec(buttonMenu, GRAY);
            }
            drawTextInsideRectangle(buttonMenu, "Back to menu", 20, WHITE);

            if (CheckCollisionPointRec(GetMousePosition(), buttonQuit)) {
                DrawRectangleRec(buttonQuit, RED);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    currentScreen = QUIT_GAME;
                    break;
                }
            }
            else {
                DrawRectangleRec(buttonQuit, GRAY);
            }
            drawTextInsideRectangle(buttonQuit, "Quit", 20, WHITE);
        }
        EndDrawing();
    }
    UnloadTexture(creditsScreen);
    return currentScreen;
}
