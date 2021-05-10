#include "mainGame.h"
//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define MAX_PLAYERS                       2
#define MAX_ROUNDS                        8

#define GRAVITY                       9.81f
#define AC			      0.012f        //jiansudu                            
#define DELTA_FPS                        60
#define EPS 			       1e-5
#define RIGHT_ANGLE  0.14f
#define RIGHT_LOW  370
#define RIGHT_HIG  430
//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef struct PlayerPot {
    Vector2 position;
    Vector2 size;

    Vector2 aimingPoint;
    double aimingAngle;
    double aimingPower;

    Vector2 previousPoint;
    double previousAngle;
    double previousPower;

    Vector2 impactPoint;

    bool isLeftTeam;                // This playerPot belongs to the left or to the right team
    bool isPlayer;                  // If is a playerPot or an AI
    bool isAlive;
	bool isUping;
} PlayerPot;

typedef struct Building {
    Rectangle rectangle;
    Color color;
} Building;

typedef struct Explosion {
    Vector2 position;
    int radius;
    bool active;
} Explosion;

typedef struct Ball {
    Vector2 position;
    Vector2 speed;
    int radius;
    bool active;
	bool firstChange;
} Ball;

typedef struct Boundary {
	//home
	float r1=20;
	float r2=40;
	float r3=60;
	Vector2 heart=(Vector2){700,225};
	//up and down boundary
	float uplen;
	float downlen;
	Vector2 upLeft;
	Vector2 upRight;
	Vector2 downLeft;
	Vector2 downRight;	
}Boundary;
//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
void InitGamePot(void);         // Initialize game
void UpdateGamePot(void);       // Update game (one frame)
void DrawGamePot(void);         // Draw game (one frame)
void UnloadGamePot(void);       // Unload game
void UpdateDrawFramePot(void);  // Update and Draw (one frame)
void DrawBrushPot(void);
void UpdateBrushPot(void);
// Additional module functions
bool UpdatePlayerPot(int playerTurn);
bool UpdateBallPot(int playerTurn);
bool judgeCollisionPot(Ball ball1,Ball ball2);
void updateVelocityPot(int i,int j);
void judgeScorePot();
void judgeTempScorePot();
float absf(float a);
int mainPot(void);