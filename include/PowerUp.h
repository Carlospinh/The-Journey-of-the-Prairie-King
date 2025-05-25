#ifndef POWERUP_H
#define POWERUP_H

#include "Collectible.h"

// Define power-up types for easier identification
enum PowerUpType {
    POWERUP_WHEEL,
    POWERUP_SHOTGUN,
    POWERUP_COFFEE,
    POWERUP_NUKE,
    POWERUP_LIFE
};

class PowerUp : public Collectible {
private:
    // Static textures for all power-ups
    static Texture2D wheelTexture;     // Shared wheel texture
    static Texture2D shotgunTexture;   // Shared shotgun texture
    static Texture2D coffeeTexture;    // Shared coffee texture
    static Texture2D nukeTexture;      // Shared nuke texture
    static Texture2D lifeTexture;      // Shared life texture
    static Sound pickupSound;          // Shared pickup sound
    
    PowerUpType type;                  // Type of power-up

public:
    PowerUp();
    ~PowerUp();
    
    // Static methods for shared resources
    static void LoadSharedResources();
    static void UnloadSharedResources();
    
    // Getters for power-up textures
    static Texture2D GetWheelTexture();
    static Texture2D GetShotgunTexture();
    static Texture2D GetCoffeeTexture();
    static Texture2D GetNukeTexture();
    static Texture2D GetLifeTexture();
    
    // Initialize power-ups at given position
    void InitWheel(Vector2 position);
    void InitShotgun(Vector2 position);
    void InitCoffee(Vector2 position);
    void InitNuke(Vector2 position);
    void InitLife(Vector2 position);
    
    // Play pickup sound
    static void PlayPickupSound();
    
    // Get power-up type
    PowerUpType GetType() const;
    bool IsWheel() const;
    bool IsShotgun() const;
    bool IsCoffee() const;
    bool IsNuke() const;
    bool IsLife() const;
};

#endif // POWERUP_H
