#ifndef COIN_H
#define COIN_H

#include "Collectible.h"

class Coin : public Collectible {
private:
    static Texture2D sharedTexture;  // Shared texture for all coins
    static Texture2D trophyTexture;  // Shared texture for trophy
    static Sound pickupSound;        // Shared pickup sound
    bool isTrophy;

public:
    Coin();
    ~Coin();
    
    // Static methods for shared resources
    static void LoadSharedResources();
    static void UnloadSharedResources();
    
    // Initialize a coin at given position
    void Init(Vector2 position);
    
    // Override the Draw method from Collectible
    void Draw() override;
    
    // Play pickup sound
    static void PlayPickupSound();
    
    void SetAsTrophy();
    bool IsTrophy() const;
};

#endif // COIN_H
