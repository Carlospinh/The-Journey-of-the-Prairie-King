#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Bullet.h"

class Player : public Entity {
private:
    Texture2D sprites[4];              // Normal movement sprites
    Texture2D shootSprites[12];         // Shooting animation sprites
    Texture2D hiTexture;                // "Hi" texture
    int currentFrame;                   // Current animation frame
    float frameTime;                    // Time between frames
    float frameCounter;                 // Frame counter
    float speed;                        // Movement speed
    bool movingLeft;                    // Moving left flag
    bool movingRight;                   // Moving right flag
    ShootingDirection currentShootDirection;  // Current shooting direction
    float shootAnimationTimer;          // Shoot animation timer
    float shootAnimationDuration;       // Shoot animation duration
    float lastBulletTime;               // Time since last bullet fired
    float bulletCooldown;               // Bullet fire cooldown
    float footstepCooldown;             // Footstep sound cooldown
    float footstepDelay;                // Time between footsteps
    int lives;                          // Player lives
    float hiTimer;                      // "Hi" animation timer
    const float hiDuration;             // "Hi" animation duration
    bool hasWheelPowerUp;               // Has wheel power-up
    bool wheelPowerUpActive;            // Wheel power-up active
    float wheelPowerUpTimer;            // Wheel power-up timer
    float wheelPowerUpDuration;         // Wheel power-up duration
    float wheelShootTimer;              // Wheel shooting timer
    float wheelShootCooldown;           // Wheel shooting cooldown

    // Sounds
    Sound footstepSound;
    Sound gunshotSound;
    Sound hitSound;
    Sound pickupPowerUpSound;

public:
    Player();
    ~Player();

    void LoadTextures();
    void UnloadTextures();

    void Update(float deltaTime) override;
    void Draw() override;

    // Movement methods
    void MoveUp(float deltaTime, float bgY);
    void MoveDown(float deltaTime, float bgY, float bgHeight);
    void MoveLeft(float deltaTime, float bgX);
    void MoveRight(float deltaTime, float bgX, float bgWidth);
    void UpdateAnimation(float deltaTime, bool moving);

    // Shooting methods
    void Shoot(Bullet bullets[], int& bulletCount, float deltaTime);
    void ShootCircle(Bullet bullets[], int& bulletCount);
    
    // Power-up methods
    void SetWheelPowerUp(bool has);
    bool HasWheelPowerUp() const;
    bool IsWheelPowerUpActive() const;
    float GetWheelPowerUpTimer() const;
    void ActivateWheelPowerUp();
    void UpdateWheelPowerUp(float deltaTime, Bullet bullets[], int& bulletCount);

    // Collision methods
    void HandleObstacleCollision(Rectangle obstacle);

    // Getters/Setters
    Texture2D GetCurrentTexture() const;
    int GetLives() const;
    void SetLives(int lives);
    void DecreaseLives();
    Rectangle GetCollisionRect() const;
    void PlayHiAnimation();
    bool IsHiAnimationPlaying() const;
};

#endif // PLAYER_H
