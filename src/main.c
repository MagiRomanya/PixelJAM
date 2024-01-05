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
    SCREEN current_screen = TITLE_SCREEN;

    while (current_screen != QUIT_GAME) {
        switch (current_screen) {
            case TITLE_SCREEN:
                current_screen = showTitleScreen();
                break;
            case LEVEL_1:
                current_screen = runLevel("assets/maps/map-test2.png", 1000.0f, 10, CREDITS_SCREEN);
                break;
            case MENU_SCREEN:
                current_screen = showMenuScreen();
                break;
            case CREDITS_SCREEN:
                current_screen = showCreditsScreen();
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
