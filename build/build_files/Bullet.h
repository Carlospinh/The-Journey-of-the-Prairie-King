#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"
#include "raylib.h"


class Bullet : public Entity {
private:
    Vector2 direction;
    bool active;
    float speed;
    // Añadir este método a la clase Bullet
    float GetDamage() const;

public:
    Bullet(Vector2 pos, Vector2 dir, float spd);

    void Update(float deltaTime) override;
    void Draw() const override;

    bool IsActive() const;
    void SetActive(bool isActive);
};

#endif