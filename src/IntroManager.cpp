#include "IntroManager.h"

void IntroManager::Init() {
    screens[0] = LoadTexture("resources/StartScreens/screen1.png");
    screens[1] = LoadTexture("resources/StartScreens/screen2.png");
    screens[2] = LoadTexture("resources/StartScreens/screen3.png");
    timer = 0.0f;
    duration = 5.0f;
    alpha = 0.0f;
    fadingIn = true;
    fadingOut = false;
}

void IntroManager::Unload() {
    for (int i = 0; i < 3; ++i) UnloadTexture(screens[i]);
}

bool IntroManager::Update(int& gameState) {
    float dt = GetFrameTime();
    if (fadingIn) {
        alpha += dt * 2.0f;
        if (alpha >= 1.0f) {
            alpha = 1.0f;
            fadingIn = false;
        }
    }

    timer += dt;

    if (timer >= duration - 1.0f && !fadingOut) {
        fadingOut = true;
    }

    if (fadingOut) {
        alpha -= dt * 2.0f;
        if (alpha <= 0.0f) {
            alpha = 0.0f;
            fadingOut = false;
            timer = 0.0f;
            fadingIn = true;
            gameState++;
            return true;
        }
    }

    return false;
}

void IntroManager::Draw(int gameState) {
    const char* text = nullptr;
    switch (gameState) {
    case 0:
        text = "Hi! This is our remake of Journey of the Prairie King...";
        break;
    case 1:
        text = "Our Team and Mentor";
        break;
    case 2:
        text = "Follow Our Journey On...";
        break;
    }

    if (text) {
        Vector2 size = MeasureTextEx(GetFontDefault(), text, 30, 1);
        DrawText(text, (1920 - size.x) / 2, 150, 30, Fade(WHITE, alpha));
    }

    DrawTextureEx(screens[gameState], { (1920 - screens[gameState].width * 0.7f) / 2.0f, 300.0f }, 0.0f, 0.7f, Fade(WHITE, alpha));
}
