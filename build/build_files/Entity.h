#ifndef ENTITY_H
#define ENTITY_H

#include "raylib.h"
#include "raymath.h"

class Entity {
protected:
    Vector2 position;
    Texture2D texture;
    float speed;
    Rectangle collisionBox;
    bool active;
    float health;
    float initialHealth;

public:
    Entity(Vector2 pos, float spd, float hp);
    virtual ~Entity();

    virtual void Update(float deltaTime) = 0;
    virtual void Draw() const;

    Vector2 GetPosition() const;
    void SetPosition(Vector2 newPos);
    Rectangle GetCollisionBox() const;
    bool CheckCollision(const Rectangle& other) const;
    bool IsActive() const;
    void SetActive(bool isActive);
    float GetHealth() const;
    float GetInitialHealth() const;
    void TakeDamage(float damage);
    void SetTexture(Texture2D newTexture);
};

#endif