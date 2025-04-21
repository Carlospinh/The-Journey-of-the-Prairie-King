#include "UIManager.h"

void UIManager::Init(ResourceManager& resources) {
    fontSize = 30;
    liveTexture = resources.GetTexture("live");
    coinTexture = resources.GetTexture("coin");
    powerUpCartel = resources.GetTexture("powerUpCartel");
    clockTexture = resources.GetTexture("clock");
    arrowTexture = resources.GetTexture("arrow");
}

void UIManager::DrawHUD(const Player& player) {
    // Lives
    float scale = 0.15f;
    Vector2 pos = { 340, 150 };
    DrawTextureEx(liveTexture, pos, 0.0f, scale, WHITE);
    DrawText(TextFormat("x %d", player.Lives()), pos.x + liveTexture.width * scale + 10, pos.y, fontSize, WHITE);

    // Coins (placeholder - ideally use CoinManager reference)
    Vector2 coinPos = { 350, 200 };
    float coinScale = 0.4f;
    DrawTextureEx(coinTexture, coinPos, 0.0f, coinScale, WHITE);
    DrawText("x ??", coinPos.x + coinTexture.width * coinScale + 10, coinPos.y, fontSize, WHITE);
}

void UIManager::DrawMenu() { 
    DrawText("PRESS ENTER TO START", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2, fontSize, WHITE);
}

void UIManager::DrawGameOver() {
    const char* text = "GAME OVER";
    int size = 100;
    Vector2 sizeVec = MeasureTextEx(GetFontDefault(), text, size, 2);
    DrawText(text, (SCREEN_WIDTH - sizeVec.x) / 2, SCREEN_HEIGHT / 2 - 50, size, RED);

    const char* prompt = "Press SPACE To Go Back To Menu";
    if ((int)(GetTime() * 2) % 2 == 0) {
        DrawText(prompt, SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 80, fontSize, WHITE);
    }
}

void UIManager::DrawLevelCompleted() {
    const char* text = "YOU WIN";
    int size = 100;
    Vector2 sizeVec = MeasureTextEx(GetFontDefault(), text, size, 2);
    DrawText(text, (SCREEN_WIDTH - sizeVec.x) / 2, SCREEN_HEIGHT / 2 - 50, size, YELLOW);

    const char* prompt = "Press SPACE To Go Back To Menu";
    if ((int)(GetTime() * 2) % 2 == 0) {
        DrawText(prompt, SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 80, fontSize, WHITE);
    }
}
