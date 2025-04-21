#pragma once

#include "raylib.h"
#include "Player.h"
#include "ResourceManager.h"
#include "BulletManager.h"

#define MAX_ENEMIES 20

class EnemyManager {
public:
    void Init(ResourceManager& resources);
    void Update(float deltaTime, const Player& player, BulletManager& bulletManager);
    void Draw();
    void Reset();

private:
    struct Enemy {
        Vector2 position; 
        float speed;
        bool active;
        int currentFrame;
        float frameTimer;
        Texture2D frames[2];
        float scale;
    } enemies[MAX_ENEMIES];

    Texture2D frames[2];
    float spawnTimer;
    float spawnInterval;

    void Spawn(const Player& player);
};
