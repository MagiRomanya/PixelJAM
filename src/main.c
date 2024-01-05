#include <stdio.h>
#include "game.h"
#include "raylib.h"
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

    // Game screens
    //--------------------------------------------------------------------------------------
    bool should_quit = false;
    showTitleScreen();
    should_quit = showMenuScreen();
    runLevel("assets/maps/map-test2.png", 1000, 10);

    // De-Initialization
    //--------------------------------------------------------------------------------------
    destroyAssets();
    CloseAudioDevice();
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
