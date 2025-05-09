#include "LevelManager.h"
#include <stdio.h>

void LevelManager::Init() {
    backgrounds[0] = LoadTexture("levels/Level_1.png");
    backgrounds[1] = LoadTexture("levels/Level_1_2.png");
    completed[0] = LoadTexture("levels/Level1_Completed1.png");
    completed[1] = LoadTexture("levels/Level1_Completed2.png");
    level2[0] = LoadTexture("levels/Level_2.png");
    level2[1] = LoadTexture("levels/Level_2_2.png");
    clock = LoadTexture("resources/UI/Clock.png");
    arrow = LoadTexture("resources/UI/Arrow.png");
    powerUpCartel = LoadTexture("resources/UI/PowerUpCartel.png");
    liveIcon = LoadTexture("resources/UI/Live.png");

    frameCounter = 0.0f;
    frameTime = 0.5f;
    currentBackground = 0;
    timeRemaining = 60.0f;
    timeElapsed = 0.0f;
    arrowInterval = 0.5f;
    arrowTimer = 0.0f;
    arrowVisible = false;
}

void LevelManager::Unload() {
    for (int i = 0; i < 2; ++i) {
        UnloadTexture(backgrounds[i]);
        UnloadTexture(completed[i]);
        UnloadTexture(level2[i]);
    }
    UnloadTexture(clock);
    UnloadTexture(arrow);
    UnloadTexture(powerUpCartel);
    UnloadTexture(liveIcon);
}

void LevelManager::Update() {
    float dt = GetFrameTime();
    timeElapsed += dt;
    if (timeRemaining > 0) timeRemaining -= dt;
    if (timeRemaining < 0) timeRemaining = 0;

    frameCounter += dt;
    if (frameCounter >= frameTime) {
        frameCounter = 0.0f;
        currentBackground = (currentBackground + 1) % 2;
    }

    if (timeRemaining <= 0.0f) {
        arrowTimer += dt;
        if (arrowTimer >= arrowInterval) {
            arrowTimer = 0.0f;
            arrowVisible = !arrowVisible;
        }
    }
}

void LevelManager::DrawBackground() {
    float scale = 3.8f;
    Rectangle src = { 0, 0, (float)backgrounds[currentBackground].width,
                             (float)backgrounds[currentBackground].height };
    Rectangle dst = { (1920 - src.width * scale) / 2,
                      (1080 - src.height * scale) / 2,
                      src.width * scale, src.height * scale };
    DrawTexturePro(backgrounds[currentBackground], src, dst, { 0, 0 }, 0.0f, WHITE);
}

void LevelManager::DrawUI(const Player& player, int coins) {
    float clockScale = 0.18f;
    float barWidth = 900.0f;
    float barHeight = 20.0f;
    float clockX = (1920 - (clock.width * clockScale + barWidth)) / 2;
    float clockY = 20.0f;
    DrawTextureEx(clock, { clockX, clockY }, 0.0f, clockScale, WHITE);

    float remaining = (timeRemaining / 60.0f) * barWidth;
    DrawRectangle(clockX + clock.width * clockScale + 10, clockY, barWidth, barHeight, BLACK);
    Color c = timeRemaining <= 3.0f && ((int)(timeRemaining * 2) % 2 == 0) ? ORANGE :
        (timeRemaining <= 10.0f ? RED : GREEN);
    DrawRectangle(clockX + clock.width * clockScale + 10, clockY, remaining, barHeight, c);

    if (liveIcon.id != 0) {
        DrawTextureEx(liveIcon, { 340, 150 }, 0.0f, 0.15f, WHITE);
        DrawText(TextFormat("x %d", player.GetLives()), 340 + liveIcon.width * 0.15f + 10, 150, 30, WHITE);
    }

    DrawTextureEx(powerUpCartel, { 350, 30 }, 0.0f, 0.2f, WHITE);
    DrawText(TextFormat("Coins: %d", coins), 360, 80, 30, YELLOW);

    if (arrowVisible) {
        DrawTextureEx(arrow, { 960 - arrow.width * 0.18f / 2, 1000 }, 0.0f, 0.18f, WHITE);
    }
}

void LevelManager::Reset() {
    timeRemaining = 60.0f;
    timeElapsed = 0.0f;
    currentBackground = 0;
    arrowVisible = false;
    frameCounter = 0.0f;
    arrowTimer = 0.0f;
}
