#include "../include/Bullet.h"

// Initialize static members
Texture2D Bullet::sharedTexture;
Texture2D Bullet::bossTexture;

Bullet::Bullet() : Entity(), direction({ 0, 0 }), speed(300.0f), damage(1), hasPenetration(false), bulletType(BULLET_PLAYER) {
}

Bullet::~Bullet() {
}

void Bullet::LoadSharedTexture() {
    sharedTexture = LoadTexture("resources/bullet/bullet.png");
    bossTexture = LoadTexture("resources/Boss Battle/Boss/Boss Bullet.png");
}

void Bullet::UnloadSharedTexture() {
    UnloadTexture(sharedTexture);
    UnloadTexture(bossTexture);
}

void Bullet::Init(Vector2 position, Vector2 direction) {
    this->position = position;
    this->direction = direction;
    this->active = true;
    this->scale = 3.5f;
    this->damage = 1;
    this->hasPenetration = false;
    this->bulletType = BULLET_PLAYER;
}

void Bullet::Init(Vector2 position, Vector2 direction, int damage, bool penetration) {
    this->position = position;
    this->direction = direction;
    this->active = true;
    this->scale = 3.5f;
    this->damage = damage;
    this->hasPenetration = penetration;
    this->bulletType = BULLET_PLAYER;
}

void Bullet::Init(Vector2 position, Vector2 direction, BulletType type) {
    this->position = position;
    this->direction = direction;
    this->active = true;
    this->bulletType = type;
    
    if (type == BULLET_BOSS || type == BULLET_BOSS_RAIN) {
        this->scale = 0.1f;  // Boss bullets slightly smaller
        this->damage = 1;    // Boss bullets deal 1 damage to player
        this->hasPenetration = false;
        
        if (type == BULLET_BOSS_RAIN) {
            this->speed = 600.0f; // Rain bullets slower than regular boss bullets
        } else {
            this->speed = 400.0f; // Regular boss bullets
        }
    } else {
        this->scale = 3.5f;
        this->damage = 1;
        this->hasPenetration = false;
        this->speed = 300.0f;
    }
}

void Bullet::Update(float deltaTime) {
    if (!active) return;
    
    // Move bullet according to direction and speed
    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;
}

void Bullet::Draw() {
    if (!active) return;
    
    if (bulletType == BULLET_BOSS || bulletType == BULLET_BOSS_RAIN) {
        DrawTextureEx(bossTexture, position, 0.0f, scale, WHITE);
    } else {
        DrawTextureEx(sharedTexture, position, 0.0f, scale, WHITE);
    }
}

Vector2 Bullet::GetDirection() const {
    return direction;
}

Rectangle Bullet::GetCollisionRect() const {
    Texture2D currentTexture = (bulletType == BULLET_BOSS || bulletType == BULLET_BOSS_RAIN) ? bossTexture : sharedTexture;
    float width = currentTexture.width * scale;
    float height = currentTexture.height * scale;
    return { position.x, position.y, width, height };
}

int Bullet::GetDamage() const {
    return damage;
}

bool Bullet::HasPenetration() const {
    return hasPenetration;
}

BulletType Bullet::GetBulletType() const {
    return bulletType;
}

void Bullet::ReduceDamage(int amount) {
    damage -= amount;
    if (damage <= 0) {
        active = false;
    }
}
