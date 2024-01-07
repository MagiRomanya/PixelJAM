#include <stdio.h>
#include "game.h"
#include "raylib.h"
#include "sprite_manager.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int initialScreenWidth = 800;
    const int initialScreenHeight = 450; // 16:9
    InitWindow(initialScreenWidth, initialScreenHeight, "PixelJAM 24");
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
    InitAudioDevice();
    initializeAssets();

    Image windowIcon = LoadImageFromTexture(getSpriteFromID(SPRITE_SAFETY_HATL_ID));
    SetWindowIcon(windowIcon);

    // Game screens
    //--------------------------------------------------------------------------------------
    SCREEN currentScreen = TITLE_SCREEN;

    while (currentScreen != QUIT_GAME) {
        switch (currentScreen) {
            case TITLE_SCREEN:
                currentScreen = showTitleScreen();
                break;
            case MENU_SCREEN:
                currentScreen = showMenuScreen();
                break;
            case LEVEL_1:
                SetPlayerHat(IMPORTANT_HAT);
                currentScreen = runLevel("assets/maps/level1.png", 1000.0f, 2, LEVEL_1, LEVEL_2);
                break;
            case LEVEL_2:
                SetPlayerHat(POINTY_HAT);
                currentScreen = runLevel("assets/maps/level2.png", 900.0f, 1, LEVEL_2, LEVEL_3);
                break;
            case LEVEL_3:
                SetPlayerHat(KING_HAT);
                currentScreen = runLevel("assets/maps/level3.png", 1000.0f, 0, LEVEL_3, LEVEL_4);
                break;
            case LEVEL_4:
                SetPlayerHat(SAFETY_HAT);
                currentScreen = runLevel("assets/maps/level4.png", 850.0f, 3, LEVEL_4, LEVEL_5);
                break;
            case LEVEL_5:
                SetPlayerHat(CHEF_HAT);
                currentScreen = runLevel("assets/maps/level5.png", 1300.0f, 2, LEVEL_5, LEVEL_6);
                break;
            case LEVEL_6:
                SetPlayerHat(IMPORTANT_HAT);
                currentScreen = runLevel("assets/maps/level6.png", 400.0f, 2, LEVEL_6, LEVEL_7);
                break;
            case LEVEL_7:
                SetPlayerHat(POINTY_HAT);
                currentScreen = runLevel("assets/maps/level7.png", 850.0f, 6, LEVEL_7, LEVEL_8);
                break;
            case LEVEL_8:
                SetPlayerHat(SAFETY_HAT);
                currentScreen = runLevel("assets/maps/level8.png", 1500.0f, 1, LEVEL_8, CREDITS_SCREEN);
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
