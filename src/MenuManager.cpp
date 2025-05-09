#include "MenuManager.h"

void MenuManager::Init() {
    logo = LoadTexture("resources/JOPK_logo.png");
    startButton = { 1920 / 2.0f - 100, 1080 - 100, 200, 50 };
}

void MenuManager::Unload() {
    UnloadTexture(logo);
}

bool MenuManager::Update() {
    return IsKeyPressed(KEY_ENTER);
}

void MenuManager::Draw() {
    DrawTexture(logo, (1920 - logo.width) / 2, (1080 - logo.height) / 3, WHITE);
    DrawRectangleRec(startButton, WHITE);
    DrawText("START", startButton.x + 60, startButton.y + 15, 20, BLACK);
}

void MenuManager::DrawGameOver() {
    ClearBackground(BLACK);
    DrawText("GAME OVER", 700, 400, 100, RED);
    if ((int)(GetTime() * 2) % 2 == 0)
        DrawText("Press SPACE To Go Back To Menu", 660, 520, 40, WHITE);
}

void MenuManager::DrawLevelCompleted() {
    ClearBackground(BLACK);
    DrawText("YOU WIN", 780, 400, 100, YELLOW);
    if ((int)(GetTime() * 2) % 2 == 0)
        DrawText("Press SPACE To Go Back To Menu", 660, 520, 40, WHITE);
}
