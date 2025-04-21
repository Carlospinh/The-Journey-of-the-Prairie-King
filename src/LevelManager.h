#pragma once

#include "raylib.h"
#include "Player.h"
#include "ResourceManager.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

class LevelManager {
public:
    void Init(ResourceManager& resources);
    void UpdateTransition(float deltaTime);
    bool ShouldTransition(const Player& player);
    void StartTransition();
    void DrawBackground();
    void DrawOverlay(const Player& player);
    void Reset();

private:
    Texture2D backgrounds[2];
    Texture2D completedBG[2];
    Texture2D level2[2]; 

    int current;
    float frameTimer;
    float frameTime;
    float scale;

    bool transitioning;
    float transitionProgress;

    ResourceManager* resourceManager;

    Texture2D GetArrowTexture() const;
};
