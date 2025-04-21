#pragma once

#include "Player.h"
#include "EnemyManager.h"
#include "CoinManager.h"
#include "PowerUpManager.h"
#include "BulletManager.h"
#include "UIManager.h"
#include "IntroManager.h"
#include "LevelManager.h"
#include "SoundManager.h"
#include "ResourceManager.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

enum GameState {
    INTRO_1,
    INTRO_2, 
    INTRO_3,
    MENU,
    PLAYING,
    GAME_OVER,
    LEVEL_COMPLETED
};

class Game {
public:
    void Init();
    void Update();
    void Draw();
    void Close();
    void Reset();

private:
    GameState currentGameState;

    Player player;
    EnemyManager enemyManager;
    CoinManager coinManager;
    PowerUpManager powerUpManager;
    BulletManager bulletManager;
    UIManager uiManager;
    IntroManager introManager;
    LevelManager levelManager;
    SoundManager soundManager;
    ResourceManager resourceManager;
};
