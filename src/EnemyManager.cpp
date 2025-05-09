#include "EnemyManager.h"
#include <stdlib.h>
#include <math.h>

void Enemy::Init(Texture2D tex1, Texture2D tex2) {
    frames[0] = tex1;
    frames[1] = tex2;
    scale = 0.2f;
    speed = 100.0f;
    currentFrame = 0;
    frameTimer = 0.0f;
    active = false;
    position = { 0, 0 };
}

void Enemy::Update(float deltaTime, Vector2 playerPos) {
    if (!active) return;

    Vector2 dir = { playerPos.x - position.x, playerPos.y - position.y };
    float len = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (len > 0) {
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

    float bgScale = 3.8f;
    float bgWidth = 860 * bgScale;
    float bgHeight = 860 * bgScale;
    float bgX = (1920 - bgWidth) / 2;
    float bgY = (1080 - bgHeight) / 2;

    if (position.x < bgX - 150 || position.x > bgX + bgWidth + 150 ||
        position.y < bgY - 150 || position.y > bgY + bgHeight + 150) {
        active = false;
    }
}

void Enemy::Draw() {
    if (active) {
        DrawTextureEx(frames[currentFrame], position, 0.0f, scale, WHITE);
    }
}

void Enemy::Spawn(Vector2 pos, float spd) {
    position = pos;
    speed = spd;
    active = true;
    currentFrame = 0;
    frameTimer = 0.0f;
    scale = 0.2f;
}

void Enemy::Deactivate() {
    active = false;
}

bool Enemy::IsActive() const {
    return active;
}

Rectangle Enemy::GetBounds() const {
    float w = frames[0].width * scale;
    float h = frames[0].height * scale;
    return { position.x, position.y, w, h };
}

Vector2 Enemy::GetPosition() const {
    return position;
}

void EnemyManager::Init() {
    frame1 = LoadTexture("resources/Enemies/Orc/Orc1.png");
    frame2 = LoadTexture("resources/Enemies/Orc/Orc2.png");

    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].Init(frame1, frame2);
    }

    spawnTimer = 0.0f;
    spawnInterval = 2.0f;
}

void EnemyManager::Update(float deltaTime, Vector2 playerPos) {
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.0f;
        Spawn();
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].Update(deltaTime, playerPos);
    }
}

void EnemyManager::Draw() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].Draw();
    }
}

void EnemyManager::Spawn() {
    float bgScale = 3.8f;
    float bgWidth = 860 * bgScale;
    float bgHeight = 860 * bgScale;
    float bgX = (1920 - bgWidth) / 2;
    float bgY = (1080 - bgHeight) / 2;

    Vector2 spawnPositions[] = {
        { bgX + 15, bgY + bgHeight * 0.4f + (float)(rand() % (int)(bgHeight * 0.2f)) },
        { bgX + bgWidth - 15, bgY + bgHeight * 0.4f + (float)(rand() % (int)(bgHeight * 0.2f)) },
        { bgX + bgWidth * 0.4f + (float)(rand() % (int)(bgWidth * 0.2f)), bgY + 15 },
        { bgX + bgWidth * 0.4f + (float)(rand() % (int)(bgWidth * 0.2f)), bgY + bgHeight - 15 }
    };

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].IsActive()) {
            int index = rand() % 4;
            float spd = 100.0f + (rand() % 50);
            enemies[i].Spawn(spawnPositions[index], spd);
            break;
        }
    }
}

void EnemyManager::Reset() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].Deactivate();
    }
    spawnTimer = 0.0f;
}

void EnemyManager::Unload() {
    UnloadTexture(frame1);
    UnloadTexture(frame2);
}
