#include "raylib.h"
#include "pixel_perfect.h"
#include <math.h>

#define VIRTUAL_SCREEN_WIDTH 160
#define VIRTUAL_SCREEN_HEIGHT 90

//-----------------------------------------------------------------------------------
// Enum  for game screens 
//-----------------------------------------------------------------------------------
typedef enum GameSreen { OPENING = 0, TITLE, GAMEPLAY, ENDING } GameScreen;
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int initialScreenWidth = 800;
    const int initialScreenHeight = 450; // 16:9

    InitWindow(initialScreenWidth, initialScreenHeight, "PixelJAM 24");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    
    GameScreen currentScreen = OPENING;
    int frameCounter = 0;	    

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    Texture2D tree = LoadTexture("assets/sprites/Tree001.png");

    RenderTexture2D WorldRenderTexture = LoadRenderTexture(VIRTUAL_SCREEN_WIDTH, VIRTUAL_SCREEN_HEIGHT);

    // Camera target position
    float cameraX = 0;
    float cameraY = 0;

    //--------------------------------------------------------------------------------------
    // Main game loop
    SetTargetFPS(120);
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch(currentScreen)
        {
            case OPENING:
                frameCounter++; //count frames
                if(frameCounter > 300 || IsKeyPressed(KEY_ENTER)){
                    currentScreen = TITLE;
                }
                break;

            case TITLE:
                if(IsKeyPressed(KEY_ENTER)){
                    currentScreen = OPENING;
                }
                //TODO condition to jump to GAMEPLAY
                break;

        }
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        cameraX = sinf(GetTime())*20;
        cameraY = cosf(GetTime()+3)*0;

        PixelPerfectData pp_data = computePixelPerfectData((Vector2){cameraX, cameraY});

        // Draw
        //----------------------------------------------------------------------------------
        BeginTextureMode(WorldRenderTexture);
        {

            ClearBackground(RAYWHITE);
            BeginMode2D(pp_data.worldCamera);
            {
                DrawCircle(0.5*GetVirtualScreenWidth(), GetVirtualScreenHeight()/2, 10, RED);
                DrawTexture(tree, 0.5*GetVirtualScreenWidth(), GetVirtualScreenHeight()/2, WHITE);
            }
            EndMode2D();
        }
        EndTextureMode();


            //DrawText("LA JAM! :)", 190, 200, 20, MAROON);

        BeginDrawing();
        {
            ClearBackground(RED);
            BeginMode2D(pp_data.screenSpaceCamera);
            {
                DrawTexturePro(WorldRenderTexture.texture, pp_data.worldRec, pp_data.screenSpaceRec, (Vector2){0}, 0.0f, WHITE);
            }
            EndMode2D();
            switch (currentScreen) {
                case OPENING:
                    DrawText("LA JAM OPENING! :)", 190, 200, 20, MAROON);
                    break;

                case TITLE:
                    DrawText("LA JAM TITLE! :)", 190, 200, 20, MAROON);
                    // TODO condition to jump to GAMEPLAY
                    break;
                default:
                    DrawText("LA JAM! :)", 190, 200, 20, MAROON);
        }

        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------

    UnloadRenderTexture(WorldRenderTexture);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
