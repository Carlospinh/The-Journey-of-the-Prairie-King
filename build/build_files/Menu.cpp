#include "Menu.h"
#include "Constants.h"

Menu::Menu() : buttonPressed(false) {
    logo = { 0 };
    startButton = { Constants::SCREEN_WIDTH / 2 - 200, Constants::SCREEN_HEIGHT - 100, 200, 50 };
}

Menu::~Menu() {
    UnloadTexture(logo);
}

void Menu::LoadResources() {
    logo = LoadTexture("resources/JOPK_logo.png");
}

void Menu::Update() {
    if (IsKeyPressed(KEY_ENTER) ||
        (CheckCollisionPointRec(GetMousePosition(), startButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
        buttonPressed = true;
    }
}

void Menu::Draw() const {
    DrawTexture(logo, (Constants::SCREEN_WIDTH - logo.width) / 2 - 100,
        (Constants::SCREEN_HEIGHT - logo.height) / 3, WHITE);
    DrawRectangleRec(startButton, WHITE);
    DrawText("START", startButton.x + 60, startButton.y + 15, 20, BLACK);
}

bool Menu::ShouldStartGame() const {
    return buttonPressed;
}