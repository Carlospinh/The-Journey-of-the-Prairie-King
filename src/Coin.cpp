#include "../include/Coin.h"

// Initialize static members
Texture2D Coin::sharedTexture;
Sound Coin::pickupSound;

Coin::Coin() : Collectible() {
}

Coin::~Coin() {
}

void Coin::LoadSharedResources() {
    sharedTexture = LoadTexture("resources/Coin/Coin.png");
    pickupSound = LoadSound("resources/Sounds/Coin.wav");
    SetSoundVolume(pickupSound, 0.5f);
}

void Coin::UnloadSharedResources() {
    UnloadTexture(sharedTexture);
    UnloadSound(pickupSound);
}

void Coin::Init(Vector2 position) {
    this->position = position;
    this->scale = 0.4f;
    this->active = true;
    SetTexture(sharedTexture);
}

void Coin::PlayPickupSound() {
    PlaySound(pickupSound);
}
