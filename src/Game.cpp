#include "Game.h"
#include "raylib.h"
#include "GameState.h"

void Game::Init() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Journey of the Prairie King");
    ToggleFullscreen();
    SetTargetFPS(60);

    InitAudioDevice();
    SetMasterVolume(1.0f);

    // Inicializar subsistemas
    resourceManager.LoadAll();
    soundManager.LoadSounds();
    introManager.Init(); 
    levelManager.Init(resourceManager);
    player.Init(resourceManager);
    enemyManager.Init(resourceManager);
    coinManager.Init(resourceManager);
    bulletManager.Init(resourceManager);
    powerUpManager.Init(resourceManager);
    uiManager.Init(resourceManager);

    currentGameState = INTRO_1;
}

void Game::Update() {
    float deltaTime = GetFrameTime();

    switch (currentGameState) {
    case INTRO_1:
    case INTRO_2:
    case INTRO_3:
        introManager.Update(currentGameState);
        break;
    case MENU:
        if (IsKeyPressed(KEY_ENTER)) {
            currentGameState = PLAYING;
            soundManager.PlayMusic();
        }
        break;
    case PLAYING:
        soundManager.UpdateMusic();

        if (player.Lives() <= 0) {
            currentGameState = GAME_OVER;
            break;
        }

        player.Update(deltaTime);
        bulletManager.Update(deltaTime, player);
        enemyManager.Update(deltaTime, player, bulletManager);
        coinManager.Update(player);
        powerUpManager.Update(player);

        if (levelManager.ShouldTransition(player)) {
            levelManager.StartTransition();
        }

        levelManager.UpdateTransition(deltaTime);

        break;
    case GAME_OVER:
    case LEVEL_COMPLETED:
        if (IsKeyPressed(KEY_SPACE)) {
            Reset();
            currentGameState = MENU;
        }
        break;
    }
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    switch (currentGameState) {
    case INTRO_1:
    case INTRO_2:
    case INTRO_3:
        introManager.Draw(currentGameState);
        break;
    case MENU:
        uiManager.DrawMenu();
        break;
    case PLAYING:
        levelManager.DrawBackground();
        player.Draw();
        enemyManager.Draw();
        bulletManager.Draw();
        coinManager.Draw();
        powerUpManager.Draw();
        levelManager.DrawOverlay(player);
        uiManager.DrawHUD(player);
        break;
    case GAME_OVER:
        uiManager.DrawGameOver();
        break;
    case LEVEL_COMPLETED:
        uiManager.DrawLevelCompleted();
        break;
    }

    EndDrawing();
} 

void Game::Close() {
    soundManager.Cleanup();
    resourceManager.UnloadAll();
    CloseAudioDevice();
    CloseWindow();
}

void Game::Reset() {
    player.Reset();
    bulletManager.Reset();
    enemyManager.Reset();
    coinManager.Reset();
    powerUpManager.Reset();
    levelManager.Reset();
    soundManager.RestartMusic();
}
