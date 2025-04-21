#pragma once

#include "raylib.h"

class Entity {
public:
    virtual ~Entity() = default;

    virtual void Init() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;

    Vector2 GetPosition() const { return position; }
    void SetPosition(Vector2 pos) { position = pos; }
    float GetScale() const { return scale; }
    void SetScale(float s) { scale = s; }

protected:
    Vector2 position = { 0.0f, 0.0f };
    float scale = 1.0f;
};
