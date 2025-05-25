#include "../include/Coin.h"

// Initialize static members
Texture2D Coin::sharedTexture;
Texture2D Coin::trophyTexture = { 0 }; // Define trophyTexture
Sound Coin::pickupSound;

Coin::Coin() : Collectible(), isTrophy(false) {
}

Coin::~Coin() {
}

void Coin::LoadSharedResources() {
    sharedTexture = LoadTexture("resources/Coin/Coin.png");
    trophyTexture = LoadTexture("resources/Boss Battle/Trophy/Trophy.png");
    pickupSound = LoadSound("resources/Sounds/Coin.wav");
    SetSoundVolume(pickupSound, 0.5f);
}

void Coin::UnloadSharedResources() {
    UnloadTexture(sharedTexture);
    UnloadTexture(trophyTexture);
    UnloadSound(pickupSound);
}

void Coin::Init(Vector2 position) {
    this->position = position;
    this->scale = 0.4f;
    this->active = true;
    this->isTrophy = false;
    SetTexture(sharedTexture);
}

void Coin::PlayPickupSound() {
    PlaySound(pickupSound);
}
void Coin::SetAsTrophy() {
    isTrophy = true;
}

bool Coin::IsTrophy() const {
    return isTrophy;
}

void Coin::Draw() {
    if (!active) return;
    
    if (isTrophy) {
        // Draw trophy with smaller scale (70% reduction from 0.5f to 0.15f)
        Rectangle sourceRec = {0, 0, (float)trophyTexture.width, (float)trophyTexture.height};
        Rectangle destRec = {position.x, position.y, trophyTexture.width * 0.15f, trophyTexture.height * 0.15f};
        Vector2 origin = {0, 0};
        DrawTexturePro(trophyTexture, sourceRec, destRec, origin, 0.0f, WHITE);
    } else {
        // Draw regular coin using parent class method
        Collectible::Draw();
    }
}
