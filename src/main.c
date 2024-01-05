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
    SCREEN_SELECTION selection = SELECTED_TITLE_SCREEN;

    while (selection != SELECTED_QUIT) {
        switch (selection) {
            case SELECTED_TITLE_SCREEN:
                selection = showTitleScreen();
                break;
            case SELECTED_PLAY_LEVEL1:
                selection = runLevel("assets/maps/map-test2.png", 1000.0f, 10, SELECTED_CREDITS);
                break;
            case SELECTED_MENU_SCREEN:
                selection = showMenuScreen();
                break;
            case SELECTED_CREDITS:
                selection = showCreditsScreen();
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
