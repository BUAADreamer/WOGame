#include "mainGame.h"
#include "skiing.h"
#include "pot.h"
#include "about.h"
Texture2D button0={0}; // Load button texture
Rectangle sourceRec0;
Rectangle sourceRec1;
Rectangle btnBounds0;
Rectangle btnBounds1;
Texture2D button1={0}; // Load button texture
//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Winter Olympics Games");
    initGame();
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    cursite=0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        switch (cursite)
    	{
		    case INDEX: UpdateDrawFrame(); break;
		    case POT: mainPot(); break;
		    case SKIING: mainSkiing(); break;
            case ABOUT: mainAbout();break;
		    default: break;
    	} 
        if(presite!=cursite) {
            initGame();
        }
        presite = cursite;
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    
    // Unload current screen data before closing
    // switch (cursite)
    // {
    //     case INDEX: UnloadIndex(); break;
    //     case POT: UnloadGamePot(); break;
    //     case SKIING: UnloadGameSkiing(); break;
    //     default: break;
    // }
    
    // Unload all global loaded data (i.e. fonts) here!
    
    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
void UnloadIndex(){

}
void initGame(){
    background = LoadTexture("resource/bg.png");
    button0 = LoadTexture("resource/1.png"); // Load button texture
    button1 = LoadTexture("resource/2.png"); // Load button texture
    float frameHeight = (float)button0.height;
    sourceRec0 = { 0, 0, (float)button0.width, frameHeight };
    btnBounds0 = { screenWidth/2.0f , screenHeight/2.0f - button0.height/2.0f, (float)button0.width, frameHeight };
    sourceRec1 = { 0, 0, (float)button1.width, frameHeight };
    btnBounds1 = { screenWidth/2.0f - 2*button1.width/2.0f, screenHeight/2.0f - button1.height/2.0f, (float)button1.width, frameHeight };
}

void UpdateDrawFrame(){
    UpdateGame();
    drawGame();
}

void UpdateGame(){
	//if (IsKeyPressed('Z')) cursite=POT;
	//else if(IsKeyPressed('A')) cursite = ABOUT;
    Vector2 mousePoint = GetMousePosition();
    if (IsKeyPressed('A')) cursite=ABOUT;
    else if (CheckCollisionPointRec(mousePoint, btnBounds1)){
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) cursite=POT;
    }
    else if (CheckCollisionPointRec(mousePoint, btnBounds0)){
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) cursite=SKIING;
    }
}

void drawGame(){
    BeginDrawing();
    DrawTexture(background,0, 0, WHITE);
    DrawText("WOGame", 20, 20, 40, BLUE);
	DrawText("Ice Pot", 200, 50, 30, YELLOW);
	DrawText("Skiing", 500, 50, 30, WHITE);
    DrawTextureRec(button0, sourceRec0, (Vector2){ btnBounds0.x, btnBounds0.y }, WHITE); // Draw button frame
    DrawTextureRec(button1, sourceRec1, (Vector2){ btnBounds1.x, btnBounds1.y }, WHITE); // Draw button frame
    DrawText("Click one Icon to begin your game", 220, 380, 20, BLACK);
    DrawText("Press [A] to learn about WOGame", 220, 410, 20, BLACK);
    DrawText("Powered By BUAA-Dreamers", 500, 10, 20, RED);
    EndDrawing();
}

