#ifndef COIN_H
#define COIN_H

#include "Collectible.h"

class Coin : public Collectible {
private:
    static Texture2D sharedTexture;  // Shared texture for all coins
    static Sound pickupSound;        // Shared pickup sound

public:
    Coin();
    ~Coin();
    
    // Static methods for shared resources
    static void LoadSharedResources();
    static void UnloadSharedResources();
    
    // Initialize a coin at given position
    void Init(Vector2 position);
    
    // Play pickup sound
    static void PlayPickupSound();
};

#endif // COIN_H
