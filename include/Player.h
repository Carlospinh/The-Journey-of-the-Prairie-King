#pragma once

#include "raylib.h"
#include "ResourceManager.h"

class Player {
public:
    Player(); // Constructor añadido

    void Init(ResourceManager& resources);
    void Update(float deltaTime);
    void Draw();
    Texture2D GetCurrentTexture() const;
    Vector2 GetPosition() const;
    int Lives() const;
    void Reset();

private:
    Texture2D sprites[4];
    Texture2D shootSprites[12];
    Texture2D hiTexture;

    Vector2 position;
    float speed;
    float scale;
    int currentFrame;
    float frameTime;
    float frameCounter;
    bool movingLeft;
    bool movingRight;

    int lives;
    float hiTimer;
    const float hiDuration = 1.0f;
};
