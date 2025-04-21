#include "Game.h"
#include <ctime>
#include <cmath>

Game::Game() {
    screenWidth = 1920;
    screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "Journey of the Prairie King (C++)");
    InitAudioDevice();
    SetTargetFPS(60);

    Init();
}

Game::~Game() {
    UnloadResources();
    CloseAudioDevice();
    CloseWindow();
}

void Game::Init() {
    currentState = GameState::INTRO_1;
    introTimer = 0.0f;
    fadeAlpha = 0.0f;
    fadingIn = true;
    fadingOut = false;

    srand((unsigned int)time(nullptr));
    LoadResources();
}

void Game::LoadResources() {
    introScreens[0] = LoadTexture("resources/StartScreens/screen1.png");
    introScreens[1] = LoadTexture("resources/StartScreens/screen2.png");
    introScreens[2] = LoadTexture("resources/StartScreens/screen3.png");

    menuLogo = LoadTexture("resources/JOPK_logo.png");
    startSound = LoadSound("resources/Sounds/start.wav");

    bgMusic = LoadMusicStream("resources/BKMusic/TheOutlaw.mp3");
    bgMusic.looping = true;
}

void Game::UnloadResources() {
    for (int i = 0; i < 3; i++) {
        UnloadTexture(introScreens[i]);
    }

    UnloadTexture(menuLogo);
    UnloadSound(startSound);
    UnloadMusicStream(bgMusic);
}

void Game::UpdateIntro() {
    float dt = GetFrameTime();

    if (fadingIn) {
        fadeAlpha += dt * 2.0f;
        if (fadeAlpha >= 1.0f) {
            fadeAlpha = 1.0f;
            fadingIn = false;
        }
    }

    introTimer += dt;

    if (introTimer >= 4.0f && !fadingOut) {
        fadingOut = true;
    }

    if (fadingOut) {
        fadeAlpha -= dt * 2.0f;
        if (fadeAlpha <= 0.0f) {
            fadeAlpha = 0.0f;
            fadingOut = false;
            introTimer = 0.0f;
            fadingIn = true;

            switch (currentState) {
            case GameState::INTRO_1: currentState = GameState::INTRO_2; break;
            case GameState::INTRO_2: currentState = GameState::INTRO_3; break;
            case GameState::INTRO_3:
                currentState = GameState::MENU;
                PlaySound(startSound);
                break;
            default: break;
            }
        }
    }
}

void Game::DrawIntro() {
    int index = static_cast<int>(currentState);
    DrawTextureEx(introScreens[index], { 300, 100 }, 0.0f, 0.7f, Fade(WHITE, fadeAlpha));
}

void Game::Update() {
    switch (currentState) {
    case GameState::INTRO_1:
    case GameState::INTRO_2:
    case GameState::INTRO_3:
        UpdateIntro();
        break;
    case GameState::MENU:
        if (IsKeyPressed(KEY_ENTER)) {
            currentState = GameState::PLAYING;
            PlayMusicStream(bgMusic);
        }
        break;
    case GameState::PLAYING:
        UpdateMusicStream(bgMusic);
        break;
    case GameState::GAME_OVER:
    case GameState::LEVEL_COMPLETED:
        if (IsKeyPressed(KEY_SPACE)) {
            currentState = GameState::MENU;
        }
        break;
    }
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    switch (currentState) {
    case GameState::INTRO_1:
    case GameState::INTRO_2:
    case GameState::INTRO_3:
        DrawIntro();
        break;
    case GameState::MENU:
        DrawTexture(menuLogo, (screenWidth - menuLogo.width) / 2, screenHeight / 3, WHITE);
        DrawText("Press ENTER to Start", screenWidth / 2 - 150, screenHeight - 100, 30, WHITE);
        break;
    case GameState::PLAYING:
        DrawText("Playing...", 50, 50, 40, GREEN);
        break;
    case GameState::GAME_OVER:
        DrawText("Game Over - Press SPACE", 50, 50, 40, RED);
        break;
    case GameState::LEVEL_COMPLETED:
        DrawText("Level Complete - Press SPACE", 50, 50, 40, YELLOW);
        break;
    }

    EndDrawing();
}

void Game::Run() {
    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
}
