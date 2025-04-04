#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "raylib.h"

class Background {
private:
    Texture2D textures[2];
    int currentTexture;
    float frameTime;
    float frameCounter;
    float scale;

public:
    Background();
    ~Background();

    void LoadTextures(const char* path1, const char* path2);
    void Update(float deltaTime);
    void Draw() const;
    Rectangle GetBounds() const;
};

#endif