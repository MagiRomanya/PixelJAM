#include "raylib.h"

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
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "PixelJAM 24");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    
    GameScreen currentScreen = OPENING;
    int frameCounter = 0;	    

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
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

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            switch(currentScreen)
            {
                case OPENING:
                    DrawText("LA JAM OPENING! :)", 190, 200, 20, MAROON);
 	            break;

	        case TITLE:
	            DrawText("LA JAM TITLE! :)", 190, 200, 20, MAROON);
	   	    //TODO condition to jump to GAMEPLAY	
                    break;
       	        default:	       
					DrawText("LA JAM! :)", 190, 200, 20, MAROON);
            }

            //DrawText("LA JAM! :)", 190, 200, 20, MAROON);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
