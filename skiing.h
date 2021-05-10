#include "mainGame.h"
//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define NUM_SHOOTS 50
#define NUM_MAX_ENEMIES 30
#define FIRST_WAVE 10
#define SECOND_WAVE 20
#define THIRD_WAVE 25
#define FOURTH_WAVE 30

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum { FIRST = 0, SECOND, THIRD, FOURTH } EnemyWave;

typedef struct Player{
    Texture2D human;
    Vector2 rec;
    Vector2 speed;
    float height;
	float width;
} Player;

typedef struct Enemy{
    Vector2 rec;
    Vector2 speed;
    bool active;
    Texture2D hinder;
    float height;
    float width;
} Enemy;

typedef struct Shoot{
    Texture2D snow;
    Vector2 rec;
    Vector2 speed;
    bool active;
    float height;
    float width;
} Shoot;

struct Rect {
	float x;
	float y;
	float w;
	float h;
};

int collision(struct Rect *a, struct Rect *b);





//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
void InitGameSkiing(void);         // Initialize game
void UpdateGameSkiing(void);       // Update game (one frame)
void DrawGameSkiing(void);         // Draw game (one frame)
void UnloadGameSkiing(void);       // Unload game
void UpdateDrawFrameSkiing(void);  // Update and Draw (one frame)
int mainSkiing(void);