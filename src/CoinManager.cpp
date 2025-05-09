#include "CoinManager.h"

void CoinManager::Init() {
    texture = LoadTexture("resources/Coin/Coin.png");
    scale = 0.4f;
    for (int i = 0; i < MAX_COINS; ++i) {
        coins[i].active = false;
    }
}

void CoinManager::Update(Vector2 playerPos) {
    float bgScale = 3.8f;
    float bgWidth = 860 * bgScale;
    float bgHeight = 860 * bgScale;
    float bgX = (1920 - bgWidth) / 2;
    float bgY = (1080 - bgHeight) / 2;

    for (int i = 0; i < MAX_COINS; ++i) {
        if (coins[i].active) {
            Rectangle playerRect = { playerPos.x, playerPos.y, 32 * 3.0f, 32 * 3.0f };
            Rectangle coinRect = { coins[i].position.x, coins[i].position.y,
                texture.width * scale, texture.height * scale };

            if (CheckCollisionRecs(playerRect, coinRect)) {
                coins[i].active = false;
                // El aumento de monedas se hace desde Game.cpp
            }

            if (coins[i].position.x < bgX || coins[i].position.x > bgX + bgWidth ||
                coins[i].position.y < bgY || coins[i].position.y > bgY + bgHeight) {
                coins[i].active = false;
            }
        }
    }
}

void CoinManager::Draw() {
    for (int i = 0; i < MAX_COINS; ++i) {
        if (coins[i].active) {
            DrawTextureEx(texture, coins[i].position, 0.0f, scale, WHITE);
        }
    }
}

void CoinManager::Spawn(Vector2 position) {
    for (int i = 0; i < MAX_COINS; ++i) {
        if (!coins[i].active) {
            coins[i].position = position;
            coins[i].scale = scale;
            coins[i].active = true;
            break;
        }
    }
}

void CoinManager::Reset() {
    for (int i = 0; i < MAX_COINS; ++i) {
        coins[i].active = false;
    }
}

void CoinManager::Unload() {
    UnloadTexture(texture);
}
