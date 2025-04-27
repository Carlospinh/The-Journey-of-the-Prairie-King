#pragma once
#include "raylib.h"
#include "GameState.h"

class IntroManager {
public:
    void Init();
    void Update(GameState& state);
    void Draw(GameState state);
    void Unload();

private:
    Texture2D screen1;
    Texture2D screen2;
    Texture2D screen3;
    float alpha;
    float timer;
    float duration;
    bool fadingIn;
    bool fadingOut;
    Sound startSound; // NUEVO
};
