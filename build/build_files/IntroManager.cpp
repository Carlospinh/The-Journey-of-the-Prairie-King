#include "IntroManager.h"
#include "GameState.h"

void IntroManager::Init() {
    screen1 = LoadTexture("resources/StartScreens/screen1.png");
    screen2 = LoadTexture("resources/StartScreens/screen2.png");
    screen3 = LoadTexture("resources/StartScreens/screen3.png");

    alpha = 0.0f;
    timer = 0.0f;
    fadingIn = true;
    fadingOut = false;
    duration = 5.0f;
}

void IntroManager::Update(GameState& state) {
    float dt = GetFrameTime();
    timer += dt;

    if (fadingIn) {
        alpha += dt * 2.0f; 
        if (alpha >= 1.0f) {
            alpha = 1.0f;
            fadingIn = false;
        }
    }

    if (timer >= duration - 1.0f && !fadingOut) {
        fadingOut = true;
    }

    if (fadingOut) {
        alpha -= dt * 2.0f;
        if (alpha <= 0.0f) {
            alpha = 0.0f;
            fadingOut = false;
            fadingIn = true;
            timer = 0.0f;

            if (state == INTRO_1) state = INTRO_2;
            else if (state == INTRO_2) state = INTRO_3;
            else if (state == INTRO_3) state = MENU;
        }
    }
}

void IntroManager::Draw(GameState state) {
    const char* text = "";
    Texture2D* screen = nullptr;

    if (state == INTRO_1) {
        text = "Welcome to the remake of Journey of the Prairie King!";
        screen = &screen1;
    }
    else if (state == INTRO_2) {
        text = "Made by our student team and mentor.";
        screen = &screen2;
    }
    else if (state == INTRO_3) {
        text = "Follow us at github.com/Carlospinh/The-Journey-of-the-Prairie-King";
        screen = &screen3;
    }

    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 30, 1);
    DrawText(text, (SCREEN_WIDTH - textSize.x) / 2, SCREEN_HEIGHT / 4, 30, Fade(WHITE, alpha));

    float scale = 0.7f;
    float w = screen->width * scale;
    float h = screen->height * scale;
    DrawTextureEx(*screen, { (SCREEN_WIDTH - w) / 2, SCREEN_HEIGHT / 2 - h / 2 }, 0.0f, scale, Fade(WHITE, alpha));
}

void IntroManager::Unload() {
    UnloadTexture(screen1);
    UnloadTexture(screen2);
    UnloadTexture(screen3);
}
