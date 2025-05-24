#include "../include/Collectible.h"

Collectible::Collectible() : Entity() {
}

Collectible::~Collectible() {
}

void Collectible::SetTexture(Texture2D texture) {
    this->texture = texture;
}

Rectangle Collectible::GetCollisionRect() const {
    float width = texture.width * scale;
    float height = texture.height * scale;
    return { position.x, position.y, width, height };
}

void Collectible::Draw() {
    if (!active) return;
    
    DrawTextureEx(texture, position, 0.0f, scale, WHITE);
}

void Collectible::Update(float deltaTime) {
    // Base implementation does nothing
    // Derived classes can override this if needed
}

bool Collectible::IsOutOfBounds(float bgX, float bgY, float bgWidth, float bgHeight) const {
    return (position.x < bgX || 
            position.x > bgX + bgWidth ||
            position.y < bgY || 
            position.y > bgY + bgHeight);
}
