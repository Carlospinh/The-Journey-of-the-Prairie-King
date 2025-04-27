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
#include "GameState.h"

class Game {
public:
    Game() :
        currentGameState(INTRO_1),
        player(),
        enemyManager(),
        coinManager(),
        powerUpManager(),
        bulletManager(),
        uiManager(),
        introManager(),
        levelManager(),
        soundManager(),
        resourceManager() {
    }

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
