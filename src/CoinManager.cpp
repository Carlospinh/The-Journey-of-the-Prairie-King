#include "CoinManager.h"

void CoinManager::Init(ResourceManager& resources) {
    texture = resources.GetTexture("coin");
    scale = 0.4f;
    for (int i = 0; i < MAX_COINS; i++) {
        coins[i].active = false;
    }
    collected = 0;
}

void CoinManager::Update(const Player& player) {
    for (int i = 0; i < MAX_COINS; i++) {
        if (!coins[i].active) continue;

        float pw = texture.width * scale;
        float ph = texture.height * scale;
        Rectangle coinRect = { coins[i].position.x, coins[i].position.y, pw, ph };
        Rectangle playerRect = { player.GetPosition().x, player.GetPosition().y, pw, ph };

        if (CheckCollisionRecs(coinRect, playerRect)) {
            coins[i].active = false;
            collected++;
        }
    }
}

void CoinManager::Draw() {
    for (int i = 0; i < MAX_COINS; i++) {
        if (coins[i].active) {
            DrawTextureEx(texture, coins[i].position, 0.0f, scale, WHITE);
        }
    }
}

void CoinManager::Spawn(Vector2 position) {
    for (int i = 0; i < MAX_COINS; i++) {
        if (!coins[i].active) {
            coins[i].position = position;
            coins[i].scale = scale;
            coins[i].active = true;
            break;
        }
    }
}

void CoinManager::Reset() {
    for (int i = 0; i < MAX_COINS; i++) {
        coins[i].active = false;
    }
    collected = 0;
}

int CoinManager::GetCollected() const {
    return collected;
}
