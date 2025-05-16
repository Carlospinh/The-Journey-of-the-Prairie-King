#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Enemy : public Entity {
private:
    Texture2D frames[2];      // Animation frames
    int currentFrame;         // Current animation frame
    float frameTimer;         // Animation frame timer
    float frameDuration;      // Duration of each frame
    float speed;              // Movement speed
    
    static Texture2D sharedFrames[2]; // Shared textures for all enemies

public:
    Enemy();
    ~Enemy();
    
    // Static methods for shared resources
    static void LoadSharedTextures();
    static void UnloadSharedTextures();
    
    // Initialize enemy
    void Init(Vector2 position, float scale, float speed);
    
    // Update enemy (move toward player)
    void Update(float deltaTime) override;
    
    // Draw the enemy
    void Draw() override;
    
    // Get enemy collision rectangle
    Rectangle GetCollisionRect() const;
    
    // Move enemy toward target position
    void MoveToward(Vector2 targetPos, float deltaTime);
    
    // Check if enemy is outside level boundaries
    bool IsOutOfBounds(float bgX, float bgY, float bgWidth, float bgHeight) const;
};

#endif // ENEMY_H
