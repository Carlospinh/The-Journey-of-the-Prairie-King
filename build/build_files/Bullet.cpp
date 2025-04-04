#include "Bullet.h"

Bullet::Bullet(Vector2 pos, Vector2 dir, float spd) :
    Entity(pos, spd), direction(dir), active(true), speed(spd) {
    // Cargar textura aquí o asignarla desde fuera
}

void Bullet::Update(float deltaTime) {
    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;

    collisionBox.x = position.x;
    collisionBox.y = position.y;

    // Desactivar si sale de la pantalla
    if (position.x < 0 || position.x > GetScreenWidth() ||
        position.y < 0 || position.y > GetScreenHeight()) {
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