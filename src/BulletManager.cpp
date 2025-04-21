#include "BulletManager.h"
#include "raylib.h"
#include <cmath>

void BulletManager::Init(ResourceManager& resources) {
    texture = resources.GetTexture("bullet");
    scale = 3.5f;
    speed = 300.0f;
    cooldown = 0.2f;
    lastShotTime = 0.0f;

    for (int i = 0; i < MAX_BULLETS; i++) {
        active[i] = false;
    }

    bulletCount = 0;
}

void BulletManager::Update(float deltaTime, const Player& player) {
    float currentTime = GetTime();

    if (currentTime - lastShotTime >= cooldown && bulletCount < MAX_BULLETS) {
        Vector2 dir = { 0, 0 };
        bool shoot = false;

        if (IsKeyPressed(KEY_UP)) {
            dir = { 0, -1 };
            shoot = true;
        }
        else if (IsKeyPressed(KEY_DOWN)) {
            dir = { 0, 1 };
            shoot = true;
        }
        else if (IsKeyPressed(KEY_LEFT)) {
            dir = { -1, 0 };
            shoot = true;
        }
        else if (IsKeyPressed(KEY_RIGHT)) {
            dir = { 1, 0 };
            shoot = true;
        }

        if (shoot) {
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!active[i]) {
                    Vector2 center = player.GetPosition();
                    bullets[i] = {
                        center.x + dir.x * 20,
                        center.y + dir.y * 20
                    };
                    directions[i] = dir;
                    active[i] = true;
                    bulletCount++;
                    lastShotTime = currentTime;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (active[i]) {
            bullets[i].x += directions[i].x * speed * deltaTime;
            bullets[i].y += directions[i].y * speed * deltaTime;

            if (bullets[i].x < 0 || bullets[i].x > SCREEN_WIDTH ||
                bullets[i].y < 0 || bullets[i].y > SCREEN_HEIGHT) {
                active[i] = false;
                bulletCount--;
            }
        }
    }
}

void BulletManager::Draw() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (active[i]) {
            DrawTextureEx(texture, bullets[i], 0.0f, scale, WHITE);
        }
    }
}

void BulletManager::Reset() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        active[i] = false;
    }
    bulletCount = 0;
    lastShotTime = 0.0f;
}