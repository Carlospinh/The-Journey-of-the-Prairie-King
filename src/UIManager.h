#pragma once

#include "raylib.h"
#include "Player.h"
#include "ResourceManager.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

class UIManager {
public:
    void Init(ResourceManager& resources);
    void DrawHUD(const Player& player);
    void DrawMenu();
    void DrawGameOver();
    void DrawLevelCompleted();

private:
    Texture2D liveTexture;
    Texture2D coinTexture;
    Texture2D powerUpCartel;
    Texture2D clockTexture;
    Texture2D arrowTexture;

    int fontSize;
};
