#ifndef MENU_H
#define MENU_H

#include "raylib.h"

class Menu {
private:
    Texture2D logo;
    Rectangle startButton;
    bool buttonPressed;

public:
    Menu();
    ~Menu();

    void LoadResources();
    void Update();
    void Draw() const;
    bool ShouldStartGame() const;
};

#endif