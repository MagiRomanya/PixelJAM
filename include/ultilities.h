#ifndef ULTILITIES_H_
#define ULTILITIES_H_

#include "appliance.h"
#include "cable.h"
#include "physics.h"
#include "entity.h"
#include "pixel_perfect.h"
#include "raylib.h"
#include "raymath.h"
#include "sprite_manager.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static inline void renderCapsule(CapsuleCollider collider) {
    const float radius = collider.radius;
    DrawCircle(collider.x1.x, collider.x1.y, radius, MAROON);
    DrawCircle(collider.x2.x, collider.x2.y, radius, MAROON);
    DrawRectangle(collider.x1.x, collider.x1.y-radius, collider.x2.x - collider.x1.x, collider.x2.y - collider.x1.y + 2*radius, MAROON);
    DrawRectangle(collider.x1.x-radius, collider.x1.y,
                  collider.x2.x - collider.x1.x + 2*radius, collider.x2.y - collider.x1.y,
                  MAROON);

}

typedef struct {
    char message[512];
    size_t ticks;
    size_t lifetime;
} RenderMessage;

static inline void addMessageToBeRendered(RenderMessage* rmessage, const char* message, float seconds) {
    strcpy(rmessage->message, message);
    rmessage->lifetime = (size_t) seconds / GetFrameTime();
    rmessage->ticks = 0;
}

static inline void drawMessage(RenderMessage* message, Vector2 position) {
    message->ticks++;
    if (message->ticks > message->lifetime) return;
    const float MESSAGE_FONT_SIZE = GetScreenWidth()* 0.01;
    DrawText(message->message, position.x, position.y, MESSAGE_FONT_SIZE, RED);
}


typedef enum {
LEVEL_1,
LEVEL_2,
MENU_SCREEN,
TITLE_SCREEN,
QUIT_GAME,
CONTROLS_SCREEN,
CREDITS_SCREEN,
} SCREEN;


static inline SCREEN showTitleScreen() {
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

static inline void drawTextInsideRectangle(Rectangle rectangle, char* text, int fontSize, Color color) {
    DrawText(text, rectangle.x + rectangle.width/2.0f - MeasureText(text, 20)/2.0f, rectangle.y + (rectangle.height-fontSize)/2.0f, fontSize, color);
}
static inline SCREEN showMenuScreen() {
    SCREEN currentScreen;
    size_t frameNumber = 0;
    // Menu screen
    //size_t centerx = GetScreenWidth()/2.0;     NOT
    //size_t centery = GetScreenHeigth()/2.0;    HERE
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

        const Rectangle applianceSourceRec = {0,0,32,32};
        const float applianceWidth = gameTitleWidth * 0.5f;
        const Rectangle applianceDestRec = {0,0,applianceWidth, applianceWidth};
        // Rectangle buttonPlay = {centerx - buttonWidth/2.0, centerx - buttonWidth/2.0};
            
        size_t centerx = GetScreenWidth()/2.0;
        size_t centery = GetScreenHeight()/2.0;
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
            ClearBackground(WHITE);
            const float time = frameNumber * GetFrameTime();
            DrawTexturePro(menuScreen, gameTitleSourceRec, gameTitleDestRec, gameTitlePosition, 0, WHITE);
            // Flying appliances
            /* Vector2 appliancePosition = Vector2Add((Vector2){0,0}, Vector2MultiplyS(100*time, (Vector2){-1,-1})); */
            /* const float applianceRotation = 0; */
            /* DrawTexturePro(getSpriteFromID(SPRITE_BLENDER_OFF_ID), applianceSourceRec, applianceDestRec, appliancePosition, applianceRotation, WHITE); */

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
        printf(":/\n");
        return QUIT_GAME;
    };
    return currentScreen;
}


static inline void renderCollisionCapsules(GameColliderList* c_list) {
    for (size_t i = 0; i < c_list->size; i++) {
        GameCollider *c = getGameColliderFromList(c_list, i);
        renderCapsule(c->capsule_collider);
    }
}

static inline void renderCableLengthUI(Cable* cable, Player* player) {
    Rectangle sourceRec = {0.0f, 0.0f, 32.0f, 32.0f};
    const float virtualRatio = getVirtualRatio();
    const float width = 32*virtualRatio;
    Rectangle destRec = {0, 0, width, width};
    const float paddingA = 4.0f * virtualRatio;
    const float paddingB = 7.0f * virtualRatio;
    Rectangle cableLengthBackgound = {8*virtualRatio + paddingA, 8*virtualRatio +paddingA, width -2*paddingA, width -paddingB};
    const float cableLength = Vector2Distance(cableGetLastAnchor(cable)->position, computePlayerHandPosition(player)) + computeCableLength(cable);
    const float remainingCable = (cable->maxLength - cableLength) / cable->maxLength;
    Rectangle cableLengthProgress = {8*virtualRatio + paddingA, 8*virtualRatio +paddingA, remainingCable * (width -2*paddingA), width -paddingB};
    /* DrawRectangleRec(cableLengthBackgound, RED); */
    DrawRectangleRec(cableLengthProgress, GRAY);
    DrawTexturePro(getSpriteFromID(SPRITE_CABLE_LENGTH_ID), sourceRec, destRec, (Vector2){-8*virtualRatio,-8*virtualRatio}, 0, WHITE);
}

static inline void renderAnchorsLeftUI(Cable* cable) {
    const float oWidth = 16.0f;
    Rectangle sourceRec = {0.0f, 0.0f, oWidth, oWidth};
    const float virtualRatio = getVirtualRatio();
    const float width = oWidth*virtualRatio;
    Rectangle destRec = {0, 0, width, width};
    Vector2 origin = {-(16.0 + 32.0)*virtualRatio, -width};
    DrawTexturePro(getSpriteFromID(SPRITE_ANCHOR_ID), sourceRec, destRec, origin, 0, GRAY);
    DrawText(TextFormat("%i", cable->nMaxAnchors - cable->nAnchors), -origin.x + virtualRatio*4, -origin.y, width*0.9, GOLD);
}

static inline void renderAnimation(Texture2D texture, int x, int y, int nFrames, int frameSpeed, int* frameCounter, int* currentFrame) {
    int width = texture.width / nFrames;
    Rectangle frameRec = {0.0f, 0.0f, width, texture.height};
    if ((*frameCounter) >= 1.0f/(GetFrameTime()*frameSpeed)) {
        (*frameCounter) = 0;
        (*currentFrame)++;
        if ((*currentFrame) > nFrames - 1) (*currentFrame) = 0;
    }
    frameRec.x = (*currentFrame) * width;
    (*frameCounter)++;
    DrawTextureRec(texture, frameRec, (Vector2) {x, y}, WHITE);
}

static inline bool renderVictoryScreen(ApplianceList* a_list) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    if (areAllAppliancesConnected(a_list)) {
        const float fontSize = screenHeight/5.0f;
        const Vector2 textPosition = {screenWidth/4.0f, screenHeight/2.0f};
        DrawText("VICTORY!", textPosition.x, textPosition.y, fontSize, DARKGREEN);
        const float buttonWidth = screenWidth / 10.0f;
        const float buttonHeight = screenHeight / 20.0f;
        const float buttonPositionX = screenWidth/2.0f - buttonWidth/2.0f;
        const float buttonPositionY = screenHeight/2.0f - buttonHeight / 2.0f + fontSize;
        Rectangle continueButtonRec = {buttonPositionX, buttonPositionY, buttonWidth, buttonHeight};
        DrawRectangleRec(continueButtonRec, DARKGREEN);
        const float continueSize = buttonWidth/5.f;
        DrawText("Continue", buttonPositionX + buttonWidth/2.0f - MeasureText("Continue", continueSize)/2.0, buttonPositionY + buttonHeight*0.2f, continueSize, BLUE);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            if (CheckCollisionPointRec(GetMousePosition(), continueButtonRec)) return false;
    }
    return true;
}

static inline SCREEN showCreditsScreen() {
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

static inline SCREEN showControlsScreen() {
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

#endif // ULTILITIES_H_
