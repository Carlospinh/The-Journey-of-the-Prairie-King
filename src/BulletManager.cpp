#include "BulletManager.h"
#include <math.h>

void BulletManager::Init() {
    texture = LoadTexture("resources/bullet/bullet.png");
    speed = 300.0f;
    scale = 3.5f;
    count = 0;
    for (int i = 0; i < MAX_BULLETS; ++i) {
        active[i] = false;
    }
}

void BulletManager::Update(float deltaTime) {
    float bgScale = 3.8f;
    float bgWidth = 860 * bgScale;
    float bgHeight = 860 * bgScale;
    float bgX = (1920 - bgWidth) / 2;
    float bgY = (1080 - bgHeight) / 2;

    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (active[i]) {
            bullets[i].x += directions[i].x * speed * deltaTime;
            bullets[i].y += directions[i].y * speed * deltaTime;

            if (bullets[i].x < bgX || bullets[i].x > bgX + bgWidth ||
                bullets[i].y < bgY || bullets[i].y > bgY + bgHeight) {
                active[i] = false;
                count--;
            }
        }
    }
}

void BulletManager::Draw() {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (active[i]) {
            DrawTextureEx(texture, bullets[i], 0.0f, scale, WHITE);
        }
    }
}

void BulletManager::Shoot(Vector2 origin, Vector2 direction) {
    if (count >= MAX_BULLETS) return;

    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (!active[i]) {
            bullets[i] = {
                origin.x - texture.width * scale / 2 + direction.x * 20,
                origin.y - texture.height * scale / 2 + direction.y * 20
            };
            directions[i] = direction;
            active[i] = true;
            count++;
            break;
        }
    }
}

void BulletManager::ShootCircle(Vector2 origin) {
    if (count >= MAX_BULLETS - 8) return;

    for (int i = 0; i < 8; ++i) {
        float angle = i * 45.0f * DEG2RAD;
        Vector2 dir = { cosf(angle), sinf(angle) };

        for (int j = 0; j < MAX_BULLETS; ++j) {
            if (!active[j]) {
                bullets[j] = {
                    origin.x - texture.width * scale / 2,
                    origin.y - texture.height * scale / 2
                };
                directions[j] = dir;
                active[j] = true;
                count++;
                break;
            }
        }
    }
}

bool BulletManager::CheckCollision(Rectangle target) {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (active[i]) {
            Rectangle b = { bullets[i].x, bullets[i].y,
                texture.width * scale, texture.height * scale };
            if (CheckCollisionRecs(b, target)) {
                active[i] = false;
                count--;
                return true;
            }
        }
    }
    return false;
}

void BulletManager::Reset() {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        active[i] = false;
    }
    count = 0;
}

void BulletManager::Unload() {
    UnloadTexture(texture);
}
