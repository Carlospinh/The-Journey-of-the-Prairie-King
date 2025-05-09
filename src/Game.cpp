#include "Game.h"
#include <raylib.h>

Game::Game() : currentState(GameState::Intro1), isMusicPlaying(false), coinsCollected(0) {}

Game::~Game() {}

void Game::Init() {
    InitWindow(1920, 1080, "Journey of the Prairie King");
    ToggleFullscreen();
    SetTargetFPS(60);

    InitAudioDevice();
    SetMasterVolume(1.0f);

    introManager.Init();
    menuManager.Init();
    player.Init();
    enemyManager.Init();
    bulletManager.Init();
    powerUpManager.Init();
    coinManager.Init();
    levelManager.Init();

    backgroundMusic = LoadMusicStream("resources/BKMusic/TheOutlaw.mp3");
    startSound = LoadSound("resources/Sounds/start.wav");
    SetSoundVolume(startSound, 1.0f);
    backgroundMusic.looping = true;
}

void Game::Run() {
    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
    Shutdown();
}

void Game::Update() {
    float deltaTime = GetFrameTime();

    switch (currentState) {
    case GameState::Intro1:
    case GameState::Intro2:
    case GameState::Intro3:
        UpdateIntro();
        break;
    case GameState::Menu:
        UpdateMenu();
        break;
    case GameState::Playing:
        UpdatePlaying();
        break;
    case GameState::GameOver:
        UpdateGameOver();
        break;
    case GameState::LevelCompleted:
        UpdateLevelCompleted();
        break;
    }
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    switch (currentState) {
    case GameState::Intro1:
    case GameState::Intro2:
    case GameState::Intro3:
        DrawIntro();
        break;
    case GameState::Menu:
        DrawMenu();
        break;
    case GameState::Playing:
        DrawPlaying();
        break;
    case GameState::GameOver:
        DrawGameOver();
        break;
    case GameState::LevelCompleted:
        DrawLevelCompleted();
        break;
    }

    EndDrawing();
}

void Game::Shutdown() {
    StopMusicStream(backgroundMusic);
    UnloadMusicStream(backgroundMusic);
    UnloadSound(startSound);

    introManager.Unload();
    menuManager.Unload();
    player.Unload();
    enemyManager.Unload();
    bulletManager.Unload();
    powerUpManager.Unload();
    coinManager.Unload();
    levelManager.Unload();

    CloseAudioDevice();
    CloseWindow();
}

void Game::UpdateIntro() {
    if (introManager.Update((int&)currentState)) {
        if (currentState == GameState::Intro3) {
            currentState = GameState::Menu;
            PlaySound(startSound);
        }
        else {
            currentState = static_cast<GameState>((int)currentState + 1);
        }
    }
}

void Game::UpdateMenu() {
    if (menuManager.Update()) {
        currentState = GameState::Playing;
        PlayMusicStream(backgroundMusic);
        SetMusicVolume(backgroundMusic, 0.5f);
        isMusicPlaying = true;
        coinsCollected = 0;
    }
}

void Game::UpdatePlaying() {
    float dt = GetFrameTime();
    if (isMusicPlaying) UpdateMusicStream(backgroundMusic);

    levelManager.Update();
    player.Update(dt);
    enemyManager.Update(dt, player.GetPosition());
    bulletManager.Update(dt);
    powerUpManager.Update(dt, player.GetPosition());
    coinManager.Update(player.GetPosition());

    // Disparar
    if (IsKeyPressed(KEY_UP)) bulletManager.Shoot(player.GetPosition(), { 0, -1 });
    if (IsKeyPressed(KEY_DOWN)) bulletManager.Shoot(player.GetPosition(), { 0, 1 });
    if (IsKeyPressed(KEY_LEFT)) bulletManager.Shoot(player.GetPosition(), { -1, 0 });
    if (IsKeyPressed(KEY_RIGHT)) bulletManager.Shoot(player.GetPosition(), { 1, 0 });

    // Power-up
    if (IsKeyPressed(KEY_SPACE) && powerUpManager.HasPowerUp()) {
        powerUpManager.Activate();
        bulletManager.ShootCircle(player.GetPosition());
    }

    // Colisiones
    for (int i = 0; i < EnemyManager::MAX_ENEMIES; i++) {
        Enemy& e = enemyManager.Get(i);
        if (e.IsActive()) {
            if (bulletManager.CheckCollision(e.GetBounds())) {
                e.Deactivate();
                coinsCollected++;
                coinManager.Spawn(e.GetPosition());

                if (GetRandomValue(0, 3) == 0) {
                    powerUpManager.TrySpawn(e.GetPosition());
                }
            }
            else if (CheckCollisionRecs(e.GetBounds(), player.GetBounds())) {
                e.Deactivate();
                player.DecreaseLife();
                if (!player.IsAlive()) currentState = GameState::GameOver;
            }
        }
    }

    if (levelManager.TimeRemaining() <= 0.0f)
        currentState = GameState::LevelCompleted;
}

void Game::UpdateGameOver() {
    if (IsKeyPressed(KEY_SPACE)) {
        levelManager.Reset();
        player.Reset();
        enemyManager.Reset();
        bulletManager.Reset();
        powerUpManager.Reset();
        coinManager.Reset();
        currentState = GameState::Menu;
    }
}

void Game::UpdateLevelCompleted() {
    if (IsKeyPressed(KEY_SPACE)) {
        levelManager.Reset();
        player.Reset();
        enemyManager.Reset();
        bulletManager.Reset();
        powerUpManager.Reset();
        coinManager.Reset();
        currentState = GameState::Menu;
    }
}

void Game::DrawIntro() {
    introManager.Draw((int)currentState);
}

void Game::DrawMenu() {
    menuManager.Draw();
}

void Game::DrawPlaying() {
    levelManager.DrawBackground();
    coinManager.Draw();
    powerUpManager.Draw();
    enemyManager.Draw();
    bulletManager.Draw();
    player.Draw();
    levelManager.DrawUI(player, coinsCollected);
}

void Game::DrawGameOver() {
    menuManager.DrawGameOver();
}

void Game::DrawLevelCompleted() {
    menuManager.DrawLevelCompleted();
}
