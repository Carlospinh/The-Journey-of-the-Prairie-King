#ifndef POWERUP_H
#define POWERUP_H

#include "Collectible.h"

class PowerUp : public Collectible {
private:
    static Texture2D wheelTexture;     // Shared wheel texture
    static Sound pickupSound;          // Shared pickup sound
    
    bool isWheel;                      // PowerUp type flag

public:
    PowerUp();
    ~PowerUp();
    
    // Static methods for shared resources
    static void LoadSharedResources();
    static void UnloadSharedResources();
    static Texture2D GetWheelTexture();  // Added getter for wheel texture
    
    // Initialize a power-up at given position
    void InitWheel(Vector2 position);
    
    // Play pickup sound
    static void PlayPickupSound();
    
    // Check if this is a wheel power-up
    bool IsWheel() const;
};

#endif // POWERUP_H
