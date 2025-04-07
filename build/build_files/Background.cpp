#include "Background.h"
#include "Constants.h"

Background::Background() : currentTexture(0), frameTime(0.5f), frameCounter(0.0f), scale(3.8f) {
    textures[0] = { 0 };
    textures[1] = { 0 };
}

Background::~Background() {
    UnloadTexture(textures[0]);
    UnloadTexture(textures[1]);
}

void Background::LoadTextures(const char* path1, const char* path2) {
    textures[0] = LoadTexture(path1);
    textures[1] = LoadTexture(path2);
}

void Background::Update(float deltaTime) {
    frameCounter += deltaTime;
    if (frameCounter >= frameTime) {
        frameCounter = 0.0f;
        currentTexture = (currentTexture + 1) % 2;
    }
}

void Background::Draw() const {
    Rectangle destRec = {
        (Constants::SCREEN_WIDTH - textures[currentTexture].width * scale) / 2 - 100,
        (Constants::SCREEN_HEIGHT - textures[currentTexture].height * scale) / 2,
        textures[currentTexture].width * scale,
        textures[currentTexture].height * scale
    };
    DrawTexturePro(textures[currentTexture],
        { 0, 0, (float)textures[currentTexture].width, (float)textures[currentTexture].height },
        destRec, { 0, 0 }, 0.0f, WHITE);
}

Rectangle Background::GetBounds() const {
    return {
        (Constants::SCREEN_WIDTH - textures[currentTexture].width * scale) / 2 - 100,
        (Constants::SCREEN_HEIGHT - textures[currentTexture].height * scale) / 2,
        textures[currentTexture].width * scale,
        textures[currentTexture].height * scale
    };
}