#include "PowerUpManager.h"

void PowerUpManager::Init(ResourceManager& resources) {
    texture = resources.GetTexture("wheel");
    scale = 0.2f;
    hasPowerUp = false;
    active = false;
    duration = 12.0f;
    timer = 0.0f;
    for (int i = 0; i < MAX_POWERUPS; i++) {
        powerUps[i].active = false;
    }
}

void PowerUpManager::Update(Player& player) {
    if (active) {
        timer -= GetFrameTime();
        if (timer <= 0.0f) {
            active = false;
        }
    }

    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!powerUps[i].active) continue;

        float pw = texture.width * scale;
        float ph = texture.height * scale;
        Rectangle pRect = { powerUps[i].position.x, powerUps[i].position.y, pw, ph };
        Rectangle playerRect = { player.GetPosition().x, player.GetPosition().y, pw, ph };

        if (CheckCollisionRecs(pRect, playerRect)) {
            powerUps[i].active = false;
            hasPowerUp = true;
        }
    }

    if (hasPowerUp && IsKeyPressed(KEY_SPACE) && !active) {
        active = true;
        hasPowerUp = false;
        timer = duration;
    }
}

void PowerUpManager::Draw() {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (powerUps[i].active) {
            DrawTextureEx(texture, powerUps[i].position, 0.0f, scale, WHITE);
        }
    }
}

void PowerUpManager::Spawn(Vector2 position) {
    if (hasPowerUp || active) return;

    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!powerUps[i].active) {
            powerUps[i].position = position;
            powerUps[i].scale = scale;
            powerUps[i].active = true;
            break;
        }
    }
}

void PowerUpManager::Reset() {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        powerUps[i].active = false;
    }
    hasPowerUp = false;
    active = false;
    timer = 0.0f;
}

bool PowerUpManager::IsActive() const {
    return active;
}

float PowerUpManager::GetRemainingTime() const {
    return timer;
}