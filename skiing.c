#include "skiing.h"

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
Texture2D hinders[4];
bool victory = false;
bool winnerIsPlayer = false;
bool showwin = false;

Player player = { 0 };
Player player1 = { 0 };
Enemy enemy[NUM_MAX_ENEMIES] = { 0 };
Shoot shoot[2 * NUM_SHOOTS] = { 0 };
EnemyWave wave;

int shootRate = 0;
int shootRate1 = 0;
float alpha = 0.0f;

int activeEnemies = 0;
int enemiesKill = 0;
bool smooth = false;
int collision(struct Rect *a, struct Rect *b) {
	int result = 1;
	if (a->x + a->w < b->x) result = 0;
	if (a->x > b->x + b->w) result = 0;
	if (a->y + a->h < b->y) result = 0;
	if (a->y > b->y + b->h) result = 0;

	return result;
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int mainSkiing(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "sample game: space invaders");

    InitGameSkiing();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrameSkiing, 60, 1);
#else
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawFrameSkiing();
        //----------------------------------------------------------------------------------
    }
#endif
    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadGameSkiing();         // Unload loaded data (textures, sounds, models...)
    
    cursite=INDEX;
    ClearBackground(RAYWHITE);
    //background = LoadTexture("resource/bg.png");
    return 0;
    //CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    //return 0;
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
void InitGameSkiing(void)
{
    
	Texture2D tree = LoadTexture("res/tree.png");
	Texture2D stone1 = LoadTexture("res/stone1.png");
	Texture2D stone2 = LoadTexture("res/stone2.png");
	Texture2D stone3 = LoadTexture("res/stone3.png");
	Texture2D snow = LoadTexture("res/snow.png");
	hinders[0] = tree;
    hinders[1] = stone1;
    hinders[2] = stone2;
    hinders[3] = stone3;

    // Initialize player
    player.rec.x =  20;
    player.rec.y = 50;
    player.human = LoadTexture("res/human.png");
    player.width = player.human.width;
    player.height = player.human.height;
    player.speed.x = 5;
    player.speed.y = 10;
    
    // Initialize player1
    player1.rec.x =  20;
    player1.rec.y = 350;
    player1.human = LoadTexture("res/human1.png");
    player1.width = player1.human.width;
    player1.height = player1.human.height;
    player1.speed.x = 5;
    player1.speed.y = 10;

    // Initialize enemies
    for (int i = 0; i < NUM_MAX_ENEMIES; i++)
    {
    	enemy[i].hinder = hinders[GetRandomValue(0,3)];
        enemy[i].height = enemy[i].hinder.height;
        enemy[i].width = enemy[i].hinder.width;
        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
        enemy[i].rec.y = GetRandomValue(enemy[i].height, screenHeight - enemy[i].height);
        enemy[i].speed.x = 5;
        enemy[i].speed.y = 5;
        enemy[i].active = true;
    }

    
    // Initialize shoots
    for (int i = 0; i < 2 * NUM_SHOOTS; i++)
    {
        shoot[i].rec.x = player.rec.x;
        shoot[i].rec.y = player.rec.y;
        shoot[i].width = snow.width;
        shoot[i].height = snow.height;
        shoot[i].speed.x = 7;
        shoot[i].speed.y = 0;
        shoot[i].active = false;
        shoot[i].snow = snow;
    }
    
    // Initialize game variables
    shootRate = 0;
    shootRate1 = 0;
    pause = false;
    gameOver = true;
    victory = false;
    smooth = false;
    winnerIsPlayer = false;
    wave = FIRST;
    activeEnemies = FIRST_WAVE;
    enemiesKill = 0;
    alpha = 0;
    showwin = false;
}

// Update game (one frame)
void UpdateGameSkiing(void)
{
    if (!gameOver)
    {
        if (IsKeyPressed('P')) pause = !pause;
        if(!showwin)showwin=true;

        if (!pause)
        {
            switch (wave)
            {
                case FIRST:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;
                        
                        if (alpha >= 1.0f) smooth = true;
                    }
                    
                    if (smooth) alpha -= 0.02f;
                    
                    if (enemiesKill == 5 * activeEnemies)
                    {
                        enemiesKill = 0;
                        
                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }
                        
                        activeEnemies = SECOND_WAVE;
                        wave = SECOND;
                        smooth = false;
                        alpha = 0.0f;
                    }
                } break;
                case SECOND:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;
                        
                        if (alpha >= 1.0f) smooth = true;
                    }
                    
                    if (smooth) alpha -= 0.02f;
                    
                    if (enemiesKill == 5 * activeEnemies)
                    {
                        enemiesKill = 0;
                        
                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }
                        
                        activeEnemies = THIRD_WAVE;
                        wave = THIRD;
                        smooth = false;
                        alpha = 0.0f;
                    }
                } break;
                case THIRD:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;
                        
                        if (alpha >= 1.0f) smooth = true;
                    }
                    
                    if (smooth) alpha -= 0.02f;
                    
                    if (enemiesKill == 5 * activeEnemies) {
                    	enemiesKill = 0;
                        
                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }
                        
                        activeEnemies = FOURTH_WAVE;
                        wave = FOURTH;
                        smooth = false;
                        alpha = 0.0f;
					}
					//victory = true;
                        
                } break;
                case FOURTH:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;
                        
                        if (alpha >= 1.0f) smooth = true;
                    }
                    
                    if (smooth) alpha -= 0.02f;
                    
                    if (enemiesKill == 10 * activeEnemies) victory = true;
                        
                } break;
                default: break;
            }

            // Player movement
            if (IsKeyDown(KEY_D)) player.rec.x += player.speed.x;
            if (IsKeyDown(KEY_A)) player.rec.x -= player.speed.x;
            if (IsKeyDown(KEY_W)) player.rec.y -= player.speed.y;
            if (IsKeyDown(KEY_S)) player.rec.y += player.speed.y;

            if (IsKeyDown(KEY_RIGHT)) player1.rec.x += player1.speed.x;
            if (IsKeyDown(KEY_LEFT)) player1.rec.x -= player1.speed.x;
            if (IsKeyDown(KEY_UP)) player1.rec.y -= player1.speed.y;
            if (IsKeyDown(KEY_DOWN)) player1.rec.y += player1.speed.y;
            
            
            // Player collision with enemy
            for (int i = 0; i < activeEnemies; i++)
            {
                struct Rect playerRect = { player.rec.x, player.rec.y-5, player.width, player.height };
                struct Rect player1Rect = { player1.rec.x, player1.rec.y, player1.width, player1.height }; 
				struct Rect enemyRect = { enemy[i].rec.x, enemy[i].rec.y, enemy[i].width, enemy[i].height };
				if (collision(&playerRect, &enemyRect)) {
					gameOver = true;
					winnerIsPlayer = false;	
				}
				if (collision(&player1Rect, &enemyRect)) {
					gameOver = true;	
					winnerIsPlayer = true;				
				}
            }

             // Enemy behaviour
            for (int i = 0; i < activeEnemies; i++)
            {
                if (enemy[i].active)
                {
                    enemy[i].rec.x -= enemy[i].speed.x;

                    if (enemy[i].rec.x < 0)
                    {
                    	enemiesKill++;
                        enemy[i].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                        enemy[i].rec.y = GetRandomValue(enemy[i].height, screenHeight-enemy[i].height);
                    }
                }
            }

            // Wall behaviour
            if (player.rec.x <= 0) player.rec.x = 0;
            if (player.rec.x + player.width >= screenWidth) player.rec.x = screenWidth - player.width;
            if (player.rec.y <= 0) player.rec.y = 0;
            if (player.rec.y + player.height >= screenHeight) player.rec.y = screenHeight - player.height;
            
            if (player1.rec.x <= 0) player1.rec.x = 0;
            if (player1.rec.x + player1.width >= screenWidth) player1.rec.x = screenWidth - player1.width;
            if (player1.rec.y <= 0) player1.rec.y = 0;
            if (player1.rec.y + player1.height >= screenHeight) player1.rec.y = screenHeight - player1.height;

            
            // Shoot initialization
            if (IsKeyDown(KEY_J))
            {
                shootRate += 5;
                for (int i = 0; i < NUM_SHOOTS; i++)
                {
                    if (!shoot[i].active && shootRate%50 == 0)
                    {
                        shoot[i].rec.x = player.rec.x;
                        shoot[i].rec.y = player.rec.y;
                        shoot[i].active = true;
                        break;
                    }
                }
            }
            
            if (IsKeyDown(KEY_ENTER))
            {
            	shootRate1 += 5;
            	for (int i = NUM_SHOOTS; i < 2 * NUM_SHOOTS; i++)
                {
                    if (!shoot[i].active && shootRate1%50 == 0)
                    {
                        shoot[i].rec.x = player1.rec.x;
                        shoot[i].rec.y = player1.rec.y;
                        shoot[i].active = true;
                        break;
                    }
                }
			}

            // Shoot logic
            for (int i = 0; i < 2 * NUM_SHOOTS; i++)
            {
                if (shoot[i].active)
                {
                    // Movement
                    shoot[i].rec.x += shoot[i].speed.x;

                    // Collision with enemy
                    for (int j = 0; j < activeEnemies; j++)
                    {
                        if (enemy[j].active)
                        {
                        	struct Rect shootRect = { shoot[i].rec.x, shoot[i].rec.y, shoot[i].width, shoot[i].height };
							struct Rect enemyRect = { enemy[j].rec.x, enemy[j].rec.y, enemy[i].width, enemy[i].height };
							if (collision(&shootRect, &enemyRect)) {
								shoot[i].active = false;
                                enemy[j].rec.x = GetRandomValue(screenWidth, screenWidth + 1000);
                                enemy[j].rec.y = GetRandomValue(enemy[j].height, screenHeight - enemy[j].height);
                                shootRate = i >= NUM_SHOOTS? shootRate : 0;
                                shootRate1 = i >= NUM_SHOOTS? 0 : shootRate1;
                                enemiesKill++;
							}
                            
                            if (shoot[i].rec.x + shoot[i].width >= screenWidth)
                            {
                                shoot[i].active = false;
                                shootRate = 0;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGameSkiing();
            gameOver = false;
        }
    }
}

// Draw game (one frame)
void DrawGameSkiing(void)
{
    BeginDrawing();

        ClearBackground(RAYWHITE);
        //DrawText(TextFormat("%04i", enemiesKill), 20, 20, 40, GRAY);
        if (!gameOver)
        {
            if (wave == FIRST) DrawText("FIRST WAVE", screenWidth/2 - MeasureText("FIRST WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
            else if (wave == SECOND) DrawText("SECOND WAVE", screenWidth/2 - MeasureText("SECOND WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
            else if (wave == THIRD) DrawText("THIRD WAVE", screenWidth/2 - MeasureText("THIRD WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
            else if (wave == FOURTH) DrawText("ENDLESS MODE", screenWidth/2 - MeasureText("ENDLESS MODE", 40)/2, screenHeight/2 - 40, 40, Fade(BLACK, alpha));
            
            for (int i = 0; i < activeEnemies; i++)
            {
                if (enemy[i].active) DrawTextureV(enemy[i].hinder, enemy[i].rec, WHITE);
                
            }
            
            for (int i = 0; i < 2 * NUM_SHOOTS; i++)
            {
                if (shoot[i].active) DrawTextureV(shoot[i].snow, shoot[i].rec, WHITE);
            }
            
            DrawTextureV(player.human, player.rec, WHITE);
            DrawTextureV(player1.human, player1.rec, WHITE);
            
        
            if (victory) DrawText("YOU WIN", screenWidth/2 - MeasureText("YOU WIN", 40)/2, screenHeight/2 - 40, 40, BLACK);
        
            if (pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, GRAY);
        }
        else {
        	DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2-80, 20, GRAY);
            DrawText("PLAYER0:PRESS [ENTER] to shoot [up/right/down/left arrow] move", GetScreenWidth()/2 - MeasureText("PLAYER0:PRESS [ENTER] to shoot [up/right/down/left arrow] move", 20)/2, GetScreenHeight()/2, 20, BLUE);
            DrawText("PLAYER1:PRESS [J] to shoot [WASD] move", GetScreenWidth()/2 - MeasureText("PLAYER1:PRESS [J] to shoot [WASD] move", 20)/2, GetScreenHeight()/2+80, 20, RED);
        	if(showwin){
                if(winnerIsPlayer) DrawText("PLAYER0 WIN !", GetScreenWidth()/2 - MeasureText("PLAYER0 WIN !", 40)/2, GetScreenHeight()/2 - 150, 40, BLUE);
        	    else DrawText("PLAYER1 WIN !", GetScreenWidth()/2 - MeasureText("PLAYER1 WIN !", 40)/2, GetScreenHeight()/2 - 150, 40, BLUE);
            }  
		}

    EndDrawing();
}

// Unload game variables
void UnloadGameSkiing(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void UpdateDrawFrameSkiing(void)
{
    UpdateGameSkiing();
    DrawGameSkiing();
}
