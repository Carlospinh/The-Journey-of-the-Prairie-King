#include "PowerUpManager.h"

void PowerUpManager::Init() {
    texture = LoadTexture("resources/PowerUps/Wheel.png");
    scale = 0.2f;
    for (int i = 0; i < MAX_POWERUPS; ++i) {
        powerUps[i].active = false;
    }
    hasPowerUp = false;
    activePowerUp = false;
    timer = 0.0f;
    duration = 12.0f;
    shootCooldown = 0.5f;
    shootTimer = 0.0f;
}

void PowerUpManager::Update(float deltaTime, Vector2 playerPos) {
    float bgScale = 3.8f;
    float bgWidth = 860 * bgScale;
    float bgHeight = 860 * bgScale;
    float bgX = (1920 - bgWidth) / 2;
    float bgY = (1080 - bgHeight) / 2;

    for (int i = 0; i < MAX_POWERUPS; ++i) {
        if (powerUps[i].active) {
            Rectangle playerRect = { playerPos.x, playerPos.y, 32 * 3.0f, 32 * 3.0f };
            Rectangle powerRect = { powerUps[i].position.x, powerUps[i].position.y,
                                    texture.width * scale, texture.height * scale };

            if (CheckCollisionRecs(playerRect, powerRect)) {
                powerUps[i].active = false;
                hasPowerUp = true;
            }
            else if (powerUps[i].position.x < bgX || powerUps[i].position.x > bgX + bgWidth ||
                powerUps[i].position.y < bgY || powerUps[i].position.y > bgY + bgHeight) {
                powerUps[i].active = false;
            }
        }
    }

    if (activePowerUp) {
        timer -= deltaTime;
        shootTimer -= deltaTime;
        if (timer <= 0.0f) {
            activePowerUp = false;
        }
    }
}

void PowerUpManager::Draw() {
    for (int i = 0; i < MAX_POWERUPS; ++i) {
        if (powerUps[i].active) {
            DrawTextureEx(texture, powerUps[i].position, 0.0f, scale, WHITE);
        }
    }
}

void PowerUpManager::TrySpawn(Vector2 pos) {
    if (hasPowerUp || activePowerUp) return;

    for (int i = 0; i < MAX_POWERUPS; ++i) {
        if (!powerUps[i].active) {
            powerUps[i].position = pos;
            powerUps[i].scale = scale;
            powerUps[i].active = true;
            break;
        }
    }
}

void PowerUpManager::Activate() {
    if (hasPowerUp && !activePowerUp) {
        hasPowerUp = false;
        activePowerUp = true;
        timer = duration;
        shootTimer = 0.0f;
    }
}

bool PowerUpManager::HasPowerUp() const {
    return hasPowerUp;
}

bool PowerUpManager::IsActive() const {
    return activePowerUp;
}

float PowerUpManager::GetTimer() const {
    return timer;
}

void PowerUpManager::Reset() {
    for (int i = 0; i < MAX_POWERUPS; ++i) {
        powerUps[i].active = false;
    }
    hasPowerUp = false;
    activePowerUp = false;
    timer = 0.0f;
    shootTimer = 0.0f;
}

void PowerUpManager::Unload() {
    UnloadTexture(texture);
}
