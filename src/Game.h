#pragma once
#include "raylib.h"

class Game {
public:
    Game();
    ~Game();

    void Run();

private:
    void Init();
    void Update();
    void Draw();
    void LoadResources();
    void UnloadResources();

    void UpdateIntro();
    void DrawIntro();

    enum class GameState {
        INTRO_1,
        INTRO_2,
        INTRO_3,
        MENU,
        PLAYING,
        GAME_OVER,
        LEVEL_COMPLETED
    };

    GameState currentState;
    float introTimer;
    float fadeAlpha;
    bool fadingIn;
    bool fadingOut;

    int screenWidth;
    int screenHeight;

    // Recursos
    Texture2D introScreens[3];
    Texture2D menuLogo;
    Music bgMusic;
    Sound startSound;
};
