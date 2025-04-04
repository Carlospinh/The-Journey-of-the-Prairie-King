#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "raylib.h"

class Player : public Entity {
private:
    Texture2D sprites[4];
    int currentFrame;
    float frameTime;
    float frameCounter;
    bool movingLeft;
    bool movingRight;
    float lastBulletTime;
    float bulletCooldown;

public:
    Player(Vector2 pos, float spd);
    ~Player();

    void Update(float deltaTime) override;
    void Draw() const override;

    void HandleInput(float deltaTime);
    bool CanShoot() const;
    void ResetShootTimer();
    int GetCurrentFrame() const;
    Texture2D GetCurrentTexture() const;
};

#endif