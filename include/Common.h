#ifndef COMMON_H
#define COMMON_H

#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

// Screen dimensions
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// Game object maximums
#define MAX_BULLETS 100
#define MAX_ENEMIES 20
#define MAX_COINS 50
#define MAX_POWERUPS 10
#define MAX_DEATH_ANIMATIONS 10

// Game states
typedef enum GameState {
    INTRO_1,
    INTRO_2,
    INTRO_3,
    MENU,
    PLAYING,
    GAME_OVER,
    LEVEL_COMPLETED,
    PLAYER_EXITING // Player exit animation after trophy
} GameState;

// Shooting directions
typedef enum ShootingDirection {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT
} ShootingDirection;

#endif // COMMON_H
