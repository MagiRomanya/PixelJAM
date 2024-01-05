#include <stdio.h>
#include "game.h"
#include "raylib.h"
#include "sprite_manager.h"
#include "ultilities.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int initialScreenWidth = 800;
    const int initialScreenHeight = 450; // 16:9
    InitWindow(initialScreenWidth, initialScreenHeight, "PixelJAM 24");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    InitAudioDevice();
    initializeAssets();

    Image windowIcon = LoadImageFromTexture(getSpriteFromID(SPRITE_SAFETY_HATL_ID));
    SetWindowIcon(windowIcon);

    // Game screens
    //--------------------------------------------------------------------------------------
    SCREEN currentScreen = TITLE_SCREEN;
    currentScreen = LEVEL_1;

    while (currentScreen != QUIT_GAME) {
        switch (currentScreen) {
            case TITLE_SCREEN:
                currentScreen = showTitleScreen();
                break;
            case MENU_SCREEN:
                currentScreen = showMenuScreen();
                break;
            case CONTROLS_SCREEN:
                currentScreen = showControlsScreen();
                break;
            case LEVEL_1:
                currentScreen = runLevel("assets/maps/map-test2.png", 1000.0f, 0, CREDITS_SCREEN);
                break;
            case CREDITS_SCREEN:
                currentScreen = showCreditsScreen();
                break;
            default:
                break;
        }
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadImage(windowIcon);
    destroyAssets();
    CloseAudioDevice();
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
