#include "../include/PowerUp.h"

// Initialize static members
Texture2D PowerUp::wheelTexture;
Sound PowerUp::pickupSound;

PowerUp::PowerUp() : Collectible(), isWheel(false) {
}

PowerUp::~PowerUp() {
}

void PowerUp::LoadSharedResources() {
    wheelTexture = LoadTexture("resources/PowerUps/Wheel.png");
    pickupSound = LoadSound("resources/Sounds/PickUpPowerUp.wav");
    SetSoundVolume(pickupSound, 0.6f);
}

void PowerUp::UnloadSharedResources() {
    UnloadTexture(wheelTexture);
    UnloadSound(pickupSound);
}

void PowerUp::InitWheel(Vector2 position) {
    this->position = position;
    this->scale = 0.2f;
    this->active = true;
    this->isWheel = true;
    SetTexture(wheelTexture);
}

void PowerUp::PlayPickupSound() {
    PlaySound(pickupSound);
}

bool PowerUp::IsWheel() const {
    return isWheel;
}

Texture2D PowerUp::GetWheelTexture() {
    return wheelTexture;
}
