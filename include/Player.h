#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

class Player {
public:
    void Init();
    void Update(float deltaTime);
    void Draw();
    void Reset();
    void Unload();

    Vector2 GetPosition() const;
    void SetPosition(Vector2 pos);
    Rectangle GetBounds() const;

    int GetLives() const;
    void DecreaseLife();
    bool IsAlive() const;

private:
    Vector2 position;
    float speed;
    int currentFrame;
    float frameCounter;
    float frameTime;
    bool movingLeft;
    bool movingRight;
    float scale;

    Texture2D sprites[4];
    Texture2D shootSprites[12];

    float shootAnimationTimer;
    float shootAnimationDuration;
    enum ShootingDirection { NONE, UP, DOWN, LEFT, RIGHT } shootDir;

    int lives;

    Texture2D GetCurrentTexture() const;
};

#endif
