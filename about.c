#include "about.h"
int mainAbout(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "About WOGame");
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    // Main game loop
    background = LoadTexture("resource/bg.png");
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
       BeginDrawing();
       DrawTexture(background,0, 0, WHITE);
       DrawText("WOGame introduction", 200, 20, 40, YELLOW);
       DrawText("This is a game to promote the Beijing Winter Olympics", 100, 80, 20, BLUE);
       DrawText("Now we have developed Ice Pot and Skiing game.", 100, 110, 20, BLUE);
       DrawText("In the future we will develop more games \nabout winter Olympics.", 100, 140, 20, BLUE);
       DrawText("Powered by BUAA-Dreamers", 150, 350, 40, RED);
       EndDrawing();
    }
    
    cursite=INDEX;
    ClearBackground(RAYWHITE);
    return 0;
}