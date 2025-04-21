#pragma once

#include "raylib.h"

enum GameState;

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

class IntroManager {
public:
    void Init();
    void Update(GameState& state);
    void Draw(GameState state);
    void Unload();

private:
    Texture2D screen1, screen2, screen3; 
    float alpha;
    float timer;
    float duration;
    bool fadingIn;
    bool fadingOut;
};
