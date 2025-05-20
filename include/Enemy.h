#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

// Define enemy types
enum EnemyType {
    ENEMY_ORC,    // Levels 1-9
    ENEMY_OGRE,   // Levels 2-9
    ENEMY_MUMMY   // Levels 4-9
};

class Enemy : public Entity {
private:
    Texture2D frames[2];      // Animation frames
    int currentFrame;         // Current animation frame
    float frameTimer;         // Animation frame timer
    float frameDuration;      // Duration of each frame
    float speed;              // Movement speed
    EnemyType type;           // Type of enemy
    int health;               // Health points: Orc = 1, Ogre = 2, Mummy = 4
    float hitFlashTime;       // Time to show hit flash effect
    
    // Static textures for all enemy types
    static Texture2D orcFrames[2];     // Shared textures for orc enemies
    static Texture2D ogreFrames[2];    // Shared textures for ogre enemies
    static Texture2D mummyFrames[2];   // Shared textures for mummy enemies

public:
    Enemy();
    ~Enemy();
    
    // Static methods for shared resources
    static void LoadSharedTextures();
    static void UnloadSharedTextures();
    
    // Initialize enemy with specific type
    void Init(Vector2 position, float scale, float speed, EnemyType enemyType);
    
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
    
    // Handle obstacle collision for enemies
    void HandleObstacleCollision(Rectangle obstacle);
    
    // Get enemy type
    EnemyType GetType() const;
    
    // Check if enemy can appear in given level
    static bool CanAppearInLevel(EnemyType type, int level);
    
    // Take damage and return true if enemy dies
    bool TakeDamage(int damage = 1);
    
    // Get health
    int GetHealth() const;
};

#endif // ENEMY_H
