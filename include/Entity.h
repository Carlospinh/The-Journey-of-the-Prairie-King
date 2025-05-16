#ifndef ENTITY_H
#define ENTITY_H

#include "Common.h"

// Base class for game entities (player, enemies, collectibles, etc.)
class Entity {
protected:
    Vector2 position;    // Position in world space
    float scale;         // Scale factor for drawing
    bool active;         // Whether the entity is active in the game

public:
    Entity();
    Entity(Vector2 position, float scale);
    virtual ~Entity();

    // Basic getters and setters
    Vector2 GetPosition() const;
    void SetPosition(Vector2 position);
    float GetScale() const;
    void SetScale(float scale);
    bool IsActive() const;
    void SetActive(bool active);

    // Virtual methods to be implemented by derived classes
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;
    
    // Base collision detection method
    bool CheckCollision(const Entity& other, float thisWidth, float thisHeight, float otherWidth, float otherHeight) const;
};

#endif // ENTITY_H
