#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <raylib.h>

class MenuManager {
public:
    void Init();
    void Unload();
    bool Update();
    void Draw();
    void DrawGameOver();
    void DrawLevelCompleted();

private:
    Texture2D logo;
    Rectangle startButton;
};

#endif
