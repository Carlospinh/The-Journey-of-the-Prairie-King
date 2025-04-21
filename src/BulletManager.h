#pragma once

#include "raylib.h"
#include "Player.h"
#include "ResourceManager.h"

#define MAX_BULLETS 100

class BulletManager {
public:
    void Init(ResourceManager& resources);
    void Update(float deltaTime, const Player& player);
    void Draw();
    void Reset();

private:
    Texture2D texture;
    Vector2 bullets[MAX_BULLETS];
    Vector2 directions[MAX_BULLETS];
    bool active[MAX_BULLETS];

    float scale;
    float speed;
    float cooldown;
    float lastShotTime;
    int bulletCount;
};
