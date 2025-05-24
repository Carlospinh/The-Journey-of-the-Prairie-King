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
    float defaultSpeed;                 // Default movement speed
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
    
    // Wheel power-up
    bool hasWheelPowerUp;               // Has wheel power-up
    bool wheelPowerUpActive;            // Wheel power-up active
    float wheelPowerUpTimer;            // Wheel power-up timer
    float wheelPowerUpDuration;         // Wheel power-up duration
    float wheelShootTimer;              // Wheel shooting timer
    float wheelShootCooldown;           // Wheel shooting cooldown
    
    // Shotgun power-up
    bool hasShotgunPowerUp;             // Has shotgun power-up
    bool shotgunPowerUpActive;          // Shotgun power-up active
    float shotgunPowerUpTimer;          // Shotgun power-up timer
    float shotgunPowerUpDuration;       // Shotgun power-up duration
    
    // Coffee power-up
    bool hasCoffeePowerUp;              // Has coffee power-up
    bool coffeePowerUpActive;           // Coffee power-up active
    float coffeePowerUpTimer;           // Coffee power-up timer
    float coffeePowerUpDuration;        // Coffee power-up duration
    float coffeeSpeedMultiplier;        // Coffee speed multiplier
    
    // Nuke power-up
    bool hasNukePowerUp;                // Has nuke power-up
    bool nukeActivated;                 // Whether nuke has been activated
    float nukeEffectTimer;              // Timer for nuke visual effect
    
    // Permanent shop upgrades
    int gunUpgradeLevel;                // Gun upgrade level (0-3)
    int bootUpgradeLevel;               // Boot upgrade level (0-3)
    int boxUpgradeLevel;                // Box upgrade level (0-3)
    float baseBulletCooldown;           // Base bullet cooldown before upgrades
    float baseSpeed;                    // Base speed before upgrades
    int baseMaxLives;                   // Base max lives before upgrades

    // Sounds
    Sound footstepSound;
    Sound gunshotSound;
    Sound hitSound;
    Sound pickupPowerUpSound;
    Sound nukeSound;

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
    void ShootShotgun(Bullet bullets[], int& bulletCount);
    
    // Power-up methods - Wheel
    void SetWheelPowerUp(bool has);
    bool HasWheelPowerUp() const;
    bool IsWheelPowerUpActive() const;
    float GetWheelPowerUpTimer() const;
    void ActivateWheelPowerUp();
    void UpdateWheelPowerUp(float deltaTime, Bullet bullets[], int& bulletCount);
    
    // Power-up methods - Shotgun
    void SetShotgunPowerUp(bool has);
    bool HasShotgunPowerUp() const;
    bool IsShotgunPowerUpActive() const;
    float GetShotgunPowerUpTimer() const;
    void ActivateShotgunPowerUp();
    void UpdateShotgunPowerUp(float deltaTime);
    
    // Power-up methods - Coffee
    void SetCoffeePowerUp(bool has);
    bool HasCoffeePowerUp() const;
    bool IsCoffeePowerUpActive() const;
    float GetCoffeePowerUpTimer() const;
    void ActivateCoffeePowerUp();
    void UpdateCoffeePowerUp(float deltaTime);
    
    // Power-up methods - Nuke
    void SetNukePowerUp(bool has);
    bool HasNukePowerUp() const;
    void ActivateNukePowerUp();
    bool IsNukeActivated() const;
    float GetNukeEffectTimer() const;
    void UpdateNukeEffect(float deltaTime);
    void ResetNukeEffect();

    // Shop upgrade methods
    void UpgradeGun();                  // Upgrade gun (reduces bullet cooldown)
    void UpgradeBoot();                 // Upgrade boot (increases speed)
    void UpgradeBox();                  // Upgrade box (increases max lives)
    int GetGunUpgradeLevel() const;
    int getBootUpgradeLevel() const;
    int GetBoxUpgradeLevel() const;
    void ApplyShopUpgrades();           // Apply all current upgrades to stats

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
