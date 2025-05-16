#include "../include/Entity.h"

Entity::Entity() : position({0, 0}), scale(1.0f), active(false) {
}

Entity::Entity(Vector2 position, float scale) : position(position), scale(scale), active(true) {
}

Entity::~Entity() {
}

Vector2 Entity::GetPosition() const {
    return position;
}

void Entity::SetPosition(Vector2 position) {
    this->position = position;
}

float Entity::GetScale() const {
    return scale;
}

void Entity::SetScale(float scale) {
    this->scale = scale;
}

bool Entity::IsActive() const {
    return active;
}

void Entity::SetActive(bool active) {
    this->active = active;
}

bool Entity::CheckCollision(const Entity& other, float thisWidth, float thisHeight, float otherWidth, float otherHeight) const {
    Rectangle thisRect = { position.x, position.y, thisWidth, thisHeight };
    Rectangle otherRect = { other.position.x, other.position.y, otherWidth, otherHeight };
    
    return CheckCollisionRecs(thisRect, otherRect);
}
