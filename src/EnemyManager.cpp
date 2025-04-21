#include "EnemyManager.h"
#include <cmath>
#include <cstdlib>
#include "GameState.h"

void Enemy::Init(Texture2D frame1, Texture2D frame2) {
    frames[0] = frame1;
    frames[1] = frame2;
    currentFrame = 0;
    frameTimer = 0.0f;
    speed = 100.0f + (rand() % 50);
    scale = 0.2f;
    active = true;
}

void Enemy::Update(float deltaTime, const Vector2& playerPos) {
    if (!active) return;

    Vector2 dir = { playerPos.x - position.x, playerPos.y - position.y };
    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (len > 0.0f) {
        dir.x /= len;
        dir.y /= len;
    }

    position.x += dir.x * speed * deltaTime;
    position.y += dir.y * speed * deltaTime;

    frameTimer += deltaTime;
    if (frameTimer >= 0.2f) {
        frameTimer = 0.0f;
        currentFrame = (currentFrame + 1) % 2;
    }
}

void Enemy::Draw() {
    if (!active) return;
    DrawTextureEx(frames[currentFrame], position, 0.0f, scale, WHITE);
}

void EnemyManager::Init(ResourceManager& resources) {
    frames[0] = resources.GetTexture("orc1");
    frames[1] = resources.GetTexture("orc2");
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].Deactivate();
    }
    spawnTimer = 0.0f;
    spawnInterval = 2.0f;
}

void EnemyManager::Update(float deltaTime, const Player& player, BulletManager& /*bulletManager*/) {
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.0f;
        Spawn(player);
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].IsActive()) {
            enemies[i].Update(deltaTime, player.GetPosition());
        }
    }
}

void EnemyManager::Draw() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].IsActive()) {
            enemies[i].Draw();
        }
    }
}

void EnemyManager::Reset() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].Deactivate();
    }
    spawnTimer = 0.0f;
}

void EnemyManager::Spawn(const Player& player) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].IsActive()) {
            enemies[i].Init(frames[0], frames[1]);
            enemies[i].SetPosition({ static_cast<float>(rand() % SCREEN_WIDTH), static_cast<float>(rand() % SCREEN_HEIGHT) });
            break;
        }
    }
}
