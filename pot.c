#include "pot.h"

int anglePattern = 2;
int anglestep = 0;
bool choosing = false;
int toshake = 0;

PlayerPot playerPot[MAX_PLAYERS] = { 0 };
Ball balls[MAX_ROUNDS];

int playerTurn = 0;
int roundTime = 0;
bool ballOnAir = false;
Boundary boundary;
bool next = false;
bool calcu = false;
int score[2]={0,0};
int winId=-1;
Texture2D  ball_red;
Texture2D  ball_yellow;
Texture2D  score_png;
float absf(float a){
	if(a>0) return a;
	else return -a;
}
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int mainPot(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Pots");

    InitGamePot();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFramePot, 60, 1);
#else

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
	ball_red = LoadTexture("res/ball_red.png");
	ball_yellow = LoadTexture("res/ball_yellow.png");
	score_png = LoadTexture("res/score.png");
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawFramePot();
        //----------------------------------------------------------------------------------
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadGamePot();         // Unload loaded data (textures, sounds, models...)
    cursite=INDEX;
    ClearBackground(RAYWHITE);
	//background = LoadTexture("resource/bg.png");
    return 0;
    //CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    //return 0;
}

// Initialize game variables
void InitGamePot(void)
{
    for(int i=0;i<MAX_ROUNDS;i++){
		balls[i].radius = 10;
		balls[i].position.x=20;
		balls[i].position.y=225;
		balls[i].active=false;
		balls[i].speed=(Vector2){0,0};
		balls[i].firstChange=false;
    }
    
	for(int i=0;i<2;i++){
		playerPot[i].position=(Vector2){10,225};
		playerPot[i].size=(Vector2){40,40};
		playerPot[i].isAlive=true;	
		playerPot[i].aimingPoint = playerPot[i].position;
    	playerPot[i].previousAngle = 0;
    	playerPot[i].previousPower = 0;
    	playerPot[i].previousPoint = playerPot[i].position;
    	playerPot[i].aimingAngle = 0;
    	playerPot[i].aimingPower = 0;
		playerPot[i].isUping = true;
	}
	playerTurn = 0;
	roundTime = 0;
	ballOnAir = false;
	boundary.heart=(Vector2){700,225};
	next=false;
	calcu = false;
	score[0]=score[1]=0;
	winId = -1;
	choosing = true;
}


// Update and Draw (one frame)
void UpdateDrawFramePot(void)
{
    UpdateGamePot();
    DrawGamePot();
}

// Unload game variables
void UnloadGamePot(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update game (one frame)
void UpdateGamePot(void)
{
	if(choosing){
		if (IsKeyPressed('1')){
			anglePattern = 1;
			choosing = false;
		}
		if (IsKeyPressed('2')){
			anglePattern = 2;
			choosing = false;
		}
	}
    else if (!gameOver)
    {
        if (IsKeyPressed('P')) pause = !pause;

        if (!pause)
        {
            if (!ballOnAir) ballOnAir = UpdatePlayerPot(playerTurn); // If we are aiming
            else
            {
				UpdateBrushPot();
                if (UpdateBallPot(playerTurn))                       // If 8 balls keep unmoved
                {
					playerTurn++;
                    if (playerTurn == MAX_PLAYERS) playerTurn = 0;
					roundTime++;
					if(roundTime==MAX_ROUNDS)gameOver=true;
		    		ballOnAir=false;
                }
		
            }
        }
    } else if(!next) {
    	if (IsKeyPressed(KEY_ENTER))
        {
            next=!next;
        }
	} else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGamePot();
            gameOver = false;
        }
    }
}

bool UpdateBallPot(int playerTurn)
{
	
	if(balls[roundTime].firstChange==false){
		balls[roundTime].speed.x = cos(playerPot[playerTurn].previousAngle)*playerPot[playerTurn].previousPower*3/DELTA_FPS/5;
        balls[roundTime].speed.y = sin(playerPot[playerTurn].previousAngle)*playerPot[playerTurn].previousPower*3/DELTA_FPS/5;
		printf("angle:%lf,power:%lf\n",playerPot[playerTurn].previousAngle,playerPot[playerTurn].previousPower);
        balls[roundTime].active = true;
        //printf("%d:activate\n",roundTime);
		balls[roundTime].firstChange = true;
	} 
	bool flag = true;
	for(int i=0;i<=roundTime;i++){
		for(int j=0;j<=roundTime;j++){
			if(i==j)continue;
			if(judgeCollisionPot(balls[i],balls[j])){
				updateVelocityPot(i,j);	
			}
			if((balls[j].speed.x>EPS||balls[j].speed.x<-EPS)||(balls[j].speed.y>EPS||balls[j].speed.y<-EPS))flag=false;	
		}
		balls[i].position.x += balls[i].speed.x;
    	balls[i].position.y += balls[i].speed.y;
		float angle = atan(balls[i].speed.y/balls[i].speed.x);
		float x = balls[i].speed.x;
		float y = balls[i].speed.y;
		float afterx=0;
		float aftery=0;
		if(x>EPS&&y>EPS){
			aftery = balls[i].speed.y-sin(angle)*AC;
			afterx = balls[i].speed.x-cos(angle)*AC;
			if(afterx<=EPS || aftery<=EPS)afterx=aftery=0;
		} else if(x>EPS&&y>=-EPS&&y<=EPS) {
			afterx = balls[i].speed.x-AC;
			if(afterx<=EPS)afterx=0;
			aftery=0;
		} else if(x>EPS&&y<-EPS) {
			afterx = balls[i].speed.x-cos(angle)*AC;
			aftery = balls[i].speed.y-sin(angle)*AC;
			if(afterx<=EPS || aftery>=-EPS)afterx=aftery=0;
		} else if(x>=-EPS&&x<=EPS&&y>EPS){
			aftery = balls[i].speed.y-AC;
			if(aftery<=EPS)aftery=0;
			afterx =0;
		} else if(x>=-EPS&&x<=EPS&&y>=-EPS&&y<=EPS) {
			aftery=afterx=0;
		} else if(x>=-EPS&&x<=EPS&&y<-EPS){
			aftery = balls[i].speed.y+AC;
			if(aftery>=-EPS)aftery=0;
			afterx =0;
		} else if(x<-EPS&&y>EPS){
			aftery = balls[i].speed.y+sin(angle)*AC;
			afterx = balls[i].speed.x+cos(angle)*AC;
			if(aftery<=EPS || afterx>=-EPS)afterx=aftery=0;
		} else if(x<-EPS&&y>=-EPS&&y<=EPS) {
			afterx = balls[i].speed.x+AC;
			if(afterx>=-EPS)afterx=0;
		} else if(x<-EPS&&y<-EPS) {
			aftery = balls[i].speed.y+sin(angle)*AC;
			afterx = balls[i].speed.x+cos(angle)*AC;
			if(aftery>=-EPS || afterx>=-EPS)afterx=aftery=0;
		}
		balls[i].speed.x=afterx;
		balls[i].speed.y=aftery;
		if((balls[i].speed.x>EPS||balls[i].speed.x<-EPS)||(balls[i].speed.y>EPS||balls[i].speed.y<-EPS))flag=false;
	}
	return flag;
}

bool UpdatePlayerPot(int playerTurn)
{
    // If we are aiming at the firing quadrant, we calculate the angle
    if(anglePattern == 1){
		if (GetMousePosition().x >= playerPot[playerTurn].position.x)
    	{
        	// Distance (calculating the fire power)
    	   	playerPot[playerTurn].aimingPower = sqrt(pow(playerPot[playerTurn].position.x - GetMousePosition().x, 2) + pow(playerPot[playerTurn].position.y - GetMousePosition().y, 2));
        	// Calculates the angle via arcsin
        	playerPot[playerTurn].aimingAngle = asin((GetMousePosition().y-playerPot[playerTurn].position.y)/playerPot[playerTurn].aimingPower);
        	// Point of the screen we are aiming at
        	playerPot[playerTurn].aimingPoint = GetMousePosition();

        	// Ball fired
        	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        	{
            	playerPot[playerTurn].previousPoint = playerPot[playerTurn].aimingPoint;
            	playerPot[playerTurn].previousPower = playerPot[playerTurn].aimingPower;
            	playerPot[playerTurn].previousAngle = playerPot[playerTurn].aimingAngle;
            	balls[roundTime].position = playerPot[playerTurn].position;
            	return true;
        	}
    	}
    	else
    	{
        	playerPot[playerTurn].aimingPoint = playerPot[playerTurn].position;
        	playerPot[playerTurn].aimingPower = 0;
        	playerPot[playerTurn].aimingAngle = 0;
    	}
	} else if(anglePattern == 2){
		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
			anglestep++;
		}
		if(anglestep == 0){
			playerPot[playerTurn].aimingPower = 200;
			playerPot[playerTurn].aimingAngle -= playerPot[playerTurn].isUping ? -1.0/DELTA_FPS : 1.0/DELTA_FPS;
			//printf("anglestep:%d  angle:%lf\n",anglestep,playerPot[playerTurn].aimingAngle);
			playerPot[playerTurn].aimingPoint.x = playerPot[playerTurn].position.x + playerPot[playerTurn].aimingPower*cos(playerPot[playerTurn].aimingAngle)/2;
			playerPot[playerTurn].aimingPoint.y = playerPot[playerTurn].position.y + playerPot[playerTurn].aimingPower*sin(playerPot[playerTurn].aimingAngle)/2;
			if(playerPot[playerTurn].aimingAngle > PI/6) playerPot[playerTurn].isUping = false;
			if(playerPot[playerTurn].aimingAngle < -PI/6) playerPot[playerTurn].isUping = true;
		}
		if(anglestep == 1){
			playerPot[playerTurn].aimingPower -= playerPot[playerTurn].isUping ? -400.0/DELTA_FPS : 400.0/DELTA_FPS;
			//printf("anglestep:%d  power:%lf\n",anglestep,playerPot[playerTurn].aimingPower);
			playerPot[playerTurn].aimingPoint.x = playerPot[playerTurn].position.x + playerPot[playerTurn].aimingPower*cos(playerPot[playerTurn].aimingAngle)/2;
			playerPot[playerTurn].aimingPoint.y = playerPot[playerTurn].position.y + playerPot[playerTurn].aimingPower*sin(playerPot[playerTurn].aimingAngle)/2;
			if(playerPot[playerTurn].aimingPower > 600) playerPot[playerTurn].isUping = false;
			if(playerPot[playerTurn].aimingPower < 30) playerPot[playerTurn].isUping = true;
		}
		if(anglestep == 2){
			playerPot[playerTurn].previousPoint = playerPot[playerTurn].aimingPoint;
            playerPot[playerTurn].previousPower = playerPot[playerTurn].aimingPower;
            playerPot[playerTurn].previousAngle = playerPot[playerTurn].aimingAngle;
			printf("anglestep:%d  angle:%lf\n",anglestep,playerPot[playerTurn].aimingAngle);
            balls[roundTime].position = playerPot[playerTurn].position;
			playerPot[playerTurn].aimingPoint = playerPot[playerTurn].position;
        	playerPot[playerTurn].aimingPower = 0;
        	playerPot[playerTurn].aimingAngle = 0;
			anglestep = 0;
			return true;
		}
	}

    return false;
}

void UpdateBrushPot(void){
	if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){
		toshake = 10;
	}
}

void DrawBrushPot(void){
	double shakerange = 0;
	if(toshake!=0){
		shakerange = 5*((toshake%2==1) ? -1:1);
		toshake --;
	}
	DrawRectangle(GetMousePosition().x, GetMousePosition().y+shakerange,10, 20, RED);
}

// Draw game (one frame)
void DrawGamePot(void)
{
    BeginDrawing();
    	
        ClearBackground(RAYWHITE);
        //DrawTexture(score_png,boundary.heart.x, boundary.heart.y, WHITE);
	DrawTexture(score_png,0, 0, WHITE);	
		if(choosing){
			DrawText("1.EASY 2.HARD", GetScreenWidth()/2 - MeasureText("1.EASY 2.HARD", 20)/2, GetScreenHeight()/2 - 50, 20, GRAY);
		}
        else if (!gameOver||!next)
        {
			if(playerTurn%2==0)
				DrawRectangle(playerPot[playerTurn].position.x - playerPot[playerTurn].size.x/2, playerPot[playerTurn].position.y - playerPot[playerTurn].size.y/2,
		                                                             playerPot[playerTurn].size.x, playerPot[playerTurn].size.y, RED);
			else
				DrawRectangle(playerPot[playerTurn].position.x - playerPot[playerTurn].size.x/2, playerPot[playerTurn].position.y - playerPot[playerTurn].size.y/2,
		                                                             playerPot[playerTurn].size.x, playerPot[playerTurn].size.y, BLUE);
            // Draw ball
			for(int i=0;i<=roundTime;i++){
				if(i%2==0){
				//	if (balls[i].active) DrawCircle(balls[i].position.x, balls[i].position.y, balls[i].radius, MAROON);
					if (balls[i].active) DrawTexture(ball_red,balls[i].position.x-10, balls[i].position.y-10, WHITE);	
				}
				else {
					//if (balls[i].active) DrawCircle(balls[i].position.x, balls[i].position.y, balls[i].radius, BLUE);
					if (balls[i].active) DrawTexture(ball_yellow,balls[i].position.x-10, balls[i].position.y-10, WHITE);	
				}
			}
			
            // Draw the angle and the power of the aim, and the previous ones
            //if (!ballOnAir)
            if(!gameOver)
            {
                
                // Draw aim
				if(ballOnAir){
					DrawBrushPot();
				}
                if(!ballOnAir){
					if((playerPot[playerTurn].aimingAngle <= RIGHT_ANGLE && playerPot[playerTurn].aimingAngle >= -RIGHT_ANGLE
						&& playerPot[playerTurn].aimingPower<= RIGHT_HIG && playerPot[playerTurn].aimingPower >= RIGHT_LOW) 
					    ||(playerPot[playerTurn].aimingAngle <= RIGHT_ANGLE && playerPot[playerTurn].aimingAngle >= -RIGHT_ANGLE && anglePattern == 2
						   && anglestep==0) ){
						DrawTriangle((Vector2){ playerPot[playerTurn].position.x + playerPot[playerTurn].size.x/4, playerPot[playerTurn].position.y - playerPot[playerTurn].size.y/4 },
									(Vector2){ playerPot[playerTurn].position.x + playerPot[playerTurn].size.x/4, playerPot[playerTurn].position.y + playerPot[playerTurn].size.y/4 },
									playerPot[playerTurn].aimingPoint, DARKGREEN);
					}
					else if (playerTurn==0)
					{
						DrawTriangle((Vector2){ playerPot[playerTurn].position.x + playerPot[playerTurn].size.x/4, playerPot[playerTurn].position.y - playerPot[playerTurn].size.y/4 },
									(Vector2){ playerPot[playerTurn].position.x + playerPot[playerTurn].size.x/4, playerPot[playerTurn].position.y + playerPot[playerTurn].size.y/4 },
									playerPot[playerTurn].aimingPoint, MAROON);
					}
					else
					{
						DrawTriangle((Vector2){ playerPot[playerTurn].position.x + playerPot[playerTurn].size.x/4, playerPot[playerTurn].position.y - playerPot[playerTurn].size.y/4 },
									(Vector2){ playerPot[playerTurn].position.x + playerPot[playerTurn].size.x/4, playerPot[playerTurn].position.y + playerPot[playerTurn].size.y/4 },
									playerPot[playerTurn].aimingPoint, DARKBLUE);
					}
				}
               if(roundTime%2==0){
               		DrawText(TextFormat("1 playerPot-%i pot",(roundTime+2)/2), 360, 60, 20, DARKBLUE);
			   } else {
			   		DrawText(TextFormat("2 playerPot-%i pot",(roundTime+1)/2), 360, 60, 20, DARKBLUE);
			   }
            }
			judgeTempScorePot();
            DrawText(TextFormat("%i playerPot-%i points %i playerPot-%i points", 1,score[0],2,score[1]),360, 20, 20, DARKBLUE);
			if(gameOver){
				if(!calcu) {
					judgeScorePot();
					calcu=true;
				}
			}
			if(calcu){
				if(winId>=0){
					DrawText(TextFormat("%d playerPot win in this round",winId+1), 360, 100, 20, DARKBLUE);
				} else {
					DrawText(TextFormat("No one win in this round"), screenWidth*2/4, 40, 20, DARKBLUE);
				}	
			}
            if (pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, GRAY);
        }
        else DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 - 50, 20, GRAY);

    EndDrawing();
}

bool judgeCollisionPot(Ball ball1,Ball ball2){
    float p1x = ball1.position.x ;
    float p1y = ball1.position.y ;
    float r1 = ball1.radius;
    float p2x = ball2.position.x ;
    float p2y = ball2.position.y ;
    float r2 = ball2.radius;
    return sqrt(pow(p1x-p2x,2)+pow(p1y-p2y,2))-(r2+r1)<=EPS; 
}

void judgeTempScorePot(){
	int shunxu[roundTime];
	score[0]=score[1]=0;
	for(int i=0;i<roundTime;i++){
		shunxu[i]=i;
	}
	for(int i=0;i<roundTime-1;i++){
		for(int j=0;j<roundTime-1-i;j++){
			if((sqrt(pow(balls[shunxu[j]].position.x-boundary.heart.x,2)+pow(balls[shunxu[j]].position.y-boundary.heart.y,2)))
			>(sqrt(pow(balls[shunxu[j+1]].position.x-boundary.heart.x,2)+pow(balls[shunxu[j+1]].position.y-boundary.heart.y,2)))){
				int t=shunxu[j];
				shunxu[j]=shunxu[j+1];
				shunxu[j+1]=t;
			}
		}
	}
	int id=-1;
	for(int i=0;i<roundTime;i++){
		if(sqrt(pow(balls[shunxu[i]].position.x-boundary.heart.x,2)+pow(balls[shunxu[i]].position.y-boundary.heart.y,2))<boundary.r1){
			if(i==0){
				id=shunxu[i]%2;
			} else if(shunxu[i]%2!=id){
				break;
			}	
			score[id] +=3;
		} 
		else if(sqrt(pow(balls[shunxu[i]].position.x-boundary.heart.x,2)+pow(balls[shunxu[i]].position.y-boundary.heart.y,2))<boundary.r2){
			if(i==0){
				id=shunxu[i]%2;
			} else if(shunxu[i]%2!=id){
				break;
			}	
			score[id] +=2;
		} 
		else if(sqrt(pow(balls[shunxu[i]].position.x-boundary.heart.x,2)+pow(balls[shunxu[i]].position.y-boundary.heart.y,2))<boundary.r3){
			if(i==0){
				id=shunxu[i]%2;
			} else if(shunxu[i]%2!=id){
				break;
			}	
			score[id]++;
		} else {
			break;
		}
	}
}

void judgeScorePot(){
	int shunxu[MAX_ROUNDS];
	score[1] = score[0] =0;
	for(int i=0;i<MAX_ROUNDS;i++){
		shunxu[i]=i;
	}
	for(int i=0;i<MAX_ROUNDS-1;i++){
		for(int j=0;j<MAX_ROUNDS-1-i;j++){
			if((sqrt(pow(balls[shunxu[j]].position.x-boundary.heart.x,2)+pow(balls[shunxu[j]].position.y-boundary.heart.y,2)))
			>(sqrt(pow(balls[shunxu[j+1]].position.x-boundary.heart.x,2)+pow(balls[shunxu[j+1]].position.y-boundary.heart.y,2)))){
				int t=shunxu[j];
				shunxu[j]=shunxu[j+1];
				shunxu[j+1]=t;
			}
		}
	}
	int id=-1;
	for(int i=0;i<MAX_ROUNDS;i++){
		if(sqrt(pow(balls[shunxu[i]].position.x-boundary.heart.x,2)+pow(balls[shunxu[i]].position.y-boundary.heart.y,2))<boundary.r1){
			if(i==0){
				id=shunxu[i]%2;
			} else if(shunxu[i]%2!=id){
				break;
			}	
			score[id] +=3;
		} 
		else if(sqrt(pow(balls[shunxu[i]].position.x-boundary.heart.x,2)+pow(balls[shunxu[i]].position.y-boundary.heart.y,2))<boundary.r2){
			if(i==0){
				id=shunxu[i]%2;
			} else if(shunxu[i]%2!=id){
				break;
			}	
			score[id] +=2;
		} 
		else if(sqrt(pow(balls[shunxu[i]].position.x-boundary.heart.x,2)+pow(balls[shunxu[i]].position.y-boundary.heart.y,2))<boundary.r3){
			if(i==0){
				id=shunxu[i]%2;
			} else if(shunxu[i]%2!=id){
				break;
			}	
			score[id]++;
		} else {
			break;
		}
	}
	winId=id;
}

void updateVelocityPot(int i,int j){
    float v1x = balls[i].speed.x ;
	float v1y = balls[i].speed.y ;
	float p1x = balls[i].position.x ;
	float p1y = balls[i].position.y ;
	float v2x = balls[j].speed.x ;
	float v2y = balls[j].speed.y ;
	float p2x = balls[j].position.x ;
	float p2y = balls[j].position.y ;
	
	if(p1y == p2y){
		if((p2y - p1y)*(v2y - v1y) > 0) return;
		balls[i].speed.x = v2x;
		balls[j].speed.x = v1x;
		//printf("here\n");
		return;
	}
	
	if(p1x == p2x){
		if((p2x - p1x)*(v2x - v1x) > 0) return;
		balls[i].speed.y = v2y;
		balls[j].speed.y = v1y;
		//printf("here\n");
		return;
	}
	
	double anl = atan2(absf(p1y - p2y), absf(p1x - p2x));
	if((p1y - p2y)*(p1x - p2x) > 0){
		float v1zhou = v2x * cos(anl) + v2y * sin(anl);
		float v1chui = v1x * sin(anl) - v1y * cos(anl);
		float v2zhou = v1x * cos(anl) + v1y * sin(anl);
		float v2chui = v2x * sin(anl) - v2y * cos(anl);
		if((v1zhou-v2zhou)*(p2x - p1x) > 0 && (v1zhou-v2zhou)*(p2y - p1y) > 0){
			return;
		}
		balls[i].speed.x = v1zhou * cos(anl) + v1chui * sin(anl);
		balls[i].speed.y = v1zhou * sin(anl) - v1chui * cos(anl);
		balls[j].speed.x = v2zhou * cos(anl) + v2chui * sin(anl);
		balls[j].speed.y = v2zhou * sin(anl) - v2chui * cos(anl);
		return;
	}
	else {
		float v1zhou = v2x * cos(anl) - v2y * sin(anl);
		float v1chui = v1x * sin(anl) + v1y * cos(anl);
		float v2zhou = v1x * cos(anl) - v1y * sin(anl);
		float v2chui = v2x * sin(anl) + v2y * cos(anl);
		if((v1zhou-v2zhou)*(p2x - p1x) > 0 && (v1zhou-v2zhou)*(p1y - p2y) > 0){
			return;
		}
		balls[i].speed.x = v1zhou * cos(anl) + v1chui * sin(anl);
		balls[i].speed.y = -v1zhou * sin(anl) + v1chui * cos(anl);
		balls[j].speed.x = v2zhou * cos(anl) + v2chui * sin(anl);
		balls[j].speed.y = -v2zhou * sin(anl) + v2chui * cos(anl);
		return;
	}
    return;
}

