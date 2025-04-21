#pragma once

#include "raylib.h"
#include "Player.h"
#include "ResourceManager.h"

#define MAX_POWERUPS 10

class PowerUpManager {
public:
    void Init(ResourceManager& resources);
    void Update(Player& player);
    void Draw();
    void Spawn(Vector2 position);
    void Reset();
    bool IsActive() const;
    float GetRemainingTime() const;

private:
    struct PowerUp {
        Vector2 position;
        bool active;
        float scale;
    } powerUps[MAX_POWERUPS];

    Texture2D texture;
    float scale;
    bool hasPowerUp;
    bool active;
    float duration;
    float timer;
};
