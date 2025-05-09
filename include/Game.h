#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "EnemyManager.h"
#include "BulletManager.h"
#include "PowerUpManager.h"
#include "CoinManager.h"
#include "IntroManager.h"
#include "MenuManager.h"
#include "LevelManager.h"
#include <raylib.h>

class Game {
public:
    Game();
    ~Game();

    void Init();
    void Run();
    void Update();
    void Draw();
    void Shutdown();

private:
    enum class GameState {
        Intro1,
        Intro2,
        Intro3,
        Menu,
        Playing,
        GameOver,
        LevelCompleted
    } currentState;

    Player player;
    EnemyManager enemyManager;
    BulletManager bulletManager;
    PowerUpManager powerUpManager;
    CoinManager coinManager;
    IntroManager introManager;
    MenuManager menuManager;
    LevelManager levelManager;

    Music backgroundMusic;
    Sound startSound;
    bool isMusicPlaying;

    int coinsCollected;

    void UpdateIntro();
    void UpdateMenu();
    void UpdatePlaying();
    void UpdateGameOver();
    void UpdateLevelCompleted();

    void DrawIntro();
    void DrawMenu();
    void DrawPlaying();
    void DrawGameOver();
    void DrawLevelCompleted();
};

#endif