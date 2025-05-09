#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include <raylib.h>
#include "Player.h"

class LevelManager {
public:
    void Init();
    void Update();
    void DrawBackground();
    void DrawUI(const Player& player, int coins);
    void Reset();
    void Unload();

    float TimeRemaining() const { return timeRemaining; }

private:
    Texture2D backgrounds[2];
    Texture2D completed[2];
    Texture2D level2[2];
    Texture2D clock;
    Texture2D arrow;
    Texture2D liveIcon;
    Texture2D powerUpCartel;

    int currentBackground;
    float frameCounter;
    float frameTime;

    float timeRemaining;
    float timeElapsed;

    float arrowTimer;
    float arrowInterval;
    bool arrowVisible;
};

#endif
