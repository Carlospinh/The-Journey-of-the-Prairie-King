#include "Entity.h"

Entity::Entity(Vector2 pos, float spd, float hp) :
    position(pos), speed(spd), active(true), health(hp), initialHealth(hp) {
    collisionBox = { position.x, position.y, 0, 0 };
    texture = { 0 };
}

Entity::~Entity() {
    // La textura debe ser manejada por las clases derivadas
}

void Entity::Draw() const {
    if (active) {
        DrawTextureV(texture, position, WHITE);
    }
}

Vector2 Entity::GetPosition() const {
    return position;
}

void Entity::SetPosition(Vector2 newPos) {
    position = newPos;
    collisionBox.x = newPos.x;
    collisionBox.y = newPos.y;
}

Rectangle Entity::GetCollisionBox() const {
    return collisionBox;
}

bool Entity::CheckCollision(const Rectangle& other) const {
    return CheckCollisionRecs(collisionBox, other);
}

bool Entity::IsActive() const {
    return active;
}

void Entity::SetActive(bool isActive) {
    active = isActive;
}

float Entity::GetHealth() const {
    return health;
}

float Entity::GetInitialHealth() const {
    return initialHealth;
}

void Entity::TakeDamage(float damage) {
    health -= damage;
    if (health <= 0) {
        active = false;
    }
}

void Entity::SetTexture(Texture2D newTexture) {
    texture = newTexture;
    collisionBox.width = texture.width;
    collisionBox.height = texture.height;
}