#ifndef INTRO_MANAGER_H
#define INTRO_MANAGER_H

#include <raylib.h>

class IntroManager {
public:
    void Init();
    void Unload();
    bool Update(int& gameState);
    void Draw(int gameState);

private:
    Texture2D screens[3];
    float timer;
    float duration;
    float alpha;
    bool fadingIn;
    bool fadingOut;
};

#endif
