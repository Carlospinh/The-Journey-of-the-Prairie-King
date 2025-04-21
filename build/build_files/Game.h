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
