#include "../include/DeathAnimation.h"
#include <cstdio>  // Added for sprintf

DeathAnimation::DeathAnimation() 
    : Entity(),
    currentFrame(0),
    frameTimer(0.0f),
    frameDuration(0.1f),
    displayTimer(0.0f),
    displayDuration(3.0f + (rand() % 8)) { // Between 3 and 10 seconds
}

DeathAnimation::~DeathAnimation() {
    if (active) {
        UnloadTextures();
    }
}

void DeathAnimation::Init(Vector2 position) {
    this->position = position;
    this->active = true;
    this->currentFrame = 0;
    this->frameTimer = 0.0f;
    this->displayTimer = 0.0f;
    this->displayDuration = 3.0f + (rand() % 8); // Between 3 and 10 seconds
    
    LoadTextures();
}

void DeathAnimation::LoadTextures() {
    for (int i = 0; i < 6; i++) {
        char filename[50];
        sprintf(filename, "resources/Enemies/Die/Die%d.png", i + 1);
        frames[i] = LoadTexture(filename);
    }
}

void DeathAnimation::UnloadTextures() {
    for (int i = 0; i < 6; i++) {
        UnloadTexture(frames[i]);
    }
}

void DeathAnimation::Update(float deltaTime) {
    if (!active) return;
    
    if (currentFrame < 5) {
        // Animation in progress (frames 0-4)
        frameTimer += deltaTime;
        if (frameTimer >= frameDuration) {
            frameTimer = 0.0f;
            currentFrame++;
        }
    }
    else {
        // Last frame (5) - keep it visible for a while
        displayTimer += deltaTime;
        if (displayTimer >= displayDuration) {
            active = false;
            UnloadTextures();
        }
    }
}

void DeathAnimation::Draw() {
    if (!active) return;
    
    DrawTextureEx(frames[currentFrame], position, 0.0f, 0.11f, WHITE);
}

bool DeathAnimation::IsComplete() const {
    return !active;
}
