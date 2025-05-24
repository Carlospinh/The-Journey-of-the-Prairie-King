#include "../include/Bullet.h"

// Initialize static member
Texture2D Bullet::sharedTexture;

Bullet::Bullet() : Entity(), direction({ 0, 0 }), speed(300.0f) {
}

Bullet::~Bullet() {
}

void Bullet::LoadSharedTexture() {
    sharedTexture = LoadTexture("resources/bullet/bullet.png");
}

void Bullet::UnloadSharedTexture() {
    UnloadTexture(sharedTexture);
}

void Bullet::Init(Vector2 position, Vector2 direction) {
    this->position = position;
    this->direction = direction;
    this->active = true;
    this->scale = 3.5f; // bulletScale from original code
}

void Bullet::Update(float deltaTime) {
    if (!active) return;
    
    // Move bullet according to direction and speed
    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;
    
    // Check if bullet is out of game area (handled by GameManager)
}

void Bullet::Draw() {
    if (!active) return;
    
    DrawTextureEx(sharedTexture, position, 0.0f, scale, WHITE);
}

Vector2 Bullet::GetDirection() const {
    return direction;
}

Rectangle Bullet::GetCollisionRect() const {
    float width = sharedTexture.width * scale;
    float height = sharedTexture.height * scale;
    return { position.x, position.y, width, height };
}
