#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define INDEX 0
#define POT 1
#define SKIING 2
#define ABOUT 3

extern const int screenWidth;
extern const int screenHeight;
extern int cursite;
extern int presite;
extern bool gameOver;
extern bool pause;
extern Texture2D background;

void UpdateDrawFrame();
void UpdateGame(void);
void drawGame();
void UnloadIndex();
void initGame();