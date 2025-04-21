#include "EnemyManager.h"
#include <cmath>
#include <cstdlib>

void EnemyManager::Init(ResourceManager& resources) {
    frames[0] = resources.GetTexture("orc1");
    frames[1] = resources.GetTexture("orc2");

    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = false;
        enemies[i].frames[0] = frames[0];
        enemies[i].frames[1] = frames[1];
        enemies[i].scale = 0.2f;
        enemies[i].speed = 100.0f + (rand() % 50);
    }

    spawnTimer = 0.0f;
    spawnInterval = 2.0f;
}

void EnemyManager::Update(float deltaTime, const Player& player, BulletManager& bulletManager) {
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.0f;
        Spawn(player);
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) continue;

        Vector2 dir = {
            player.GetPosition().x - enemies[i].position.x,
            player.GetPosition().y - enemies[i].position.y
        };
        float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
        if (len > 0) {
            dir.x /= len;
            dir.y /= len;
        }

        enemies[i].position.x += dir.x * enemies[i].speed * deltaTime;
        enemies[i].position.y += dir.y * enemies[i].speed * deltaTime;

        enemies[i].frameTimer += deltaTime;
        if (enemies[i].frameTimer >= 0.2f) {
            enemies[i].frameTimer = 0.0f;
            enemies[i].currentFrame = (enemies[i].currentFrame + 1) % 2;
        }
    }
}

void EnemyManager::Draw() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) continue;
        DrawTextureEx(enemies[i].frames[enemies[i].currentFrame], enemies[i].position, 0.0f, enemies[i].scale, WHITE);
    }
}

void EnemyManager::Reset() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = false;
    }
    spawnTimer = 0.0f;
}

void EnemyManager::Spawn(const Player& player) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            enemies[i].active = true;
            enemies[i].position = {
                static_cast<float>(rand() % SCREEN_WIDTH),
                static_cast<float>(rand() % SCREEN_HEIGHT)
            };
            enemies[i].speed = 100.0f + (rand() % 50);
            enemies[i].currentFrame = 0;
            enemies[i].frameTimer = 0.0f;
            break;
        }
    }
}