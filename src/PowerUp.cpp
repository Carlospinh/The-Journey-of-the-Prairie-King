#include "../include/PowerUp.h"

// Initialize static members
Texture2D PowerUp::wheelTexture;
Texture2D PowerUp::shotgunTexture;
Texture2D PowerUp::coffeeTexture;
Texture2D PowerUp::nukeTexture;
Sound PowerUp::pickupSound;

PowerUp::PowerUp() : Collectible(), type(POWERUP_WHEEL) {
}

PowerUp::~PowerUp() {
}

void PowerUp::LoadSharedResources() {
    wheelTexture = LoadTexture("resources/PowerUps/Wheel.png");
    shotgunTexture = LoadTexture("resources/PowerUps/Shotgun.png");
    coffeeTexture = LoadTexture("resources/PowerUps/Coffee.png");
    nukeTexture = LoadTexture("resources/PowerUps/Nuke.png");
    
    pickupSound = LoadSound("resources/Sounds/PickUpPowerUp.wav");
    SetSoundVolume(pickupSound, 0.6f);
}

void PowerUp::UnloadSharedResources() {
    UnloadTexture(wheelTexture);
    UnloadTexture(shotgunTexture);
    UnloadTexture(coffeeTexture);
    UnloadTexture(nukeTexture);
    UnloadSound(pickupSound);
}

void PowerUp::InitWheel(Vector2 position) {
    this->position = position;
    this->scale = 0.2f;
    this->active = true;
    this->type = POWERUP_WHEEL;
    SetTexture(wheelTexture);
}

void PowerUp::InitShotgun(Vector2 position) {
    this->position = position;
    this->scale = 0.2f;
    this->active = true;
    this->type = POWERUP_SHOTGUN;
    SetTexture(shotgunTexture);
}

void PowerUp::InitCoffee(Vector2 position) {
    this->position = position;
    this->scale = 0.7f;  // Increased from 0.2f to 0.5f (2.5x larger)
    this->active = true;
    this->type = POWERUP_COFFEE;
    SetTexture(coffeeTexture);
}

void PowerUp::InitNuke(Vector2 position) {
    this->position = position;
    this->scale = 0.2f;
    this->active = true;
    this->type = POWERUP_NUKE;
    SetTexture(nukeTexture);
}

void PowerUp::PlayPickupSound() {
    PlaySound(pickupSound);
}

PowerUpType PowerUp::GetType() const {
    return type;
}

bool PowerUp::IsWheel() const {
    return type == POWERUP_WHEEL;
}

bool PowerUp::IsShotgun() const {
    return type == POWERUP_SHOTGUN;
}

bool PowerUp::IsCoffee() const {
    return type == POWERUP_COFFEE;
}

bool PowerUp::IsNuke() const {
    return type == POWERUP_NUKE;
}

Texture2D PowerUp::GetWheelTexture() {
    return wheelTexture;
}

Texture2D PowerUp::GetShotgunTexture() {
    return shotgunTexture;
}

Texture2D PowerUp::GetCoffeeTexture() {
    return coffeeTexture;
}

Texture2D PowerUp::GetNukeTexture() {
    return nukeTexture;
}
