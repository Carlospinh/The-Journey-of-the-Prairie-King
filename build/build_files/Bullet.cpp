#include "Bullet.h"
#include "Constants.h"

Bullet::Bullet(Vector2 pos, Vector2 dir, float spd) :
    Entity(pos, spd, 1.0f), direction(dir), active(true), speed(spd) {
    texture = LoadTexture("resources/bullet/bullet.png");
    collisionBox.width = texture.width;
    collisionBox.height = texture.height;
}

Bullet::~Bullet() {
    UnloadTexture(texture);
}

void Bullet::Update(float deltaTime) {
    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;

    collisionBox.x = position.x;
    collisionBox.y = position.y;

    if (position.x < 0 || position.x > Constants::SCREEN_WIDTH ||
        position.y < 0 || position.y > Constants::SCREEN_HEIGHT) {
        active = false;
    }
}

void Bullet::Draw() const {
    if (active) {
        DrawTextureEx(texture, position, 0.0f, 3.0f, WHITE);
    }
}

bool Bullet::IsActive() const {
    return active;
}

void Bullet::SetActive(bool isActive) {
    active = isActive;
}

float Bullet::GetDamage() const {
    return 10.0f;
}