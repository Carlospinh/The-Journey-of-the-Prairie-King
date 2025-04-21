#pragma once

#include "raylib.h"
#include "Player.h"
#include "ResourceManager.h"

#define MAX_COINS 50

class CoinManager {
public:
    void Init(ResourceManager& resources);
    void Update(const Player& player);
    void Draw();
    void Spawn(Vector2 position);
    void Reset();
    int GetCollected() const;

private:
    struct Coin {
        Vector2 position;
        bool active; 
        float scale;
    } coins[MAX_COINS];

    Texture2D texture;
    float scale;
    int collected;
};
