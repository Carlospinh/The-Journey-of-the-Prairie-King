#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

// Define enemy types
enum EnemyType {
    ENEMY_ORC,    // Levels 1-9
    ENEMY_OGRE,   // Levels 2-9
    ENEMY_MUMMY,  // Levels 4-9
    ENEMY_BOSS    // Level 10 only
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
    
    // Beaver chasing functionality
    bool chasingBeaver;       // Whether this enemy is chasing a beaver
    Vector2 beaverTarget;     // Position of the beaver being chased
    float beaverChaseRange;   // Range within which enemy will chase beaver
    
    // Boss movement variables
    Vector2 bossSpawnPos;     // Original spawn position for boss
    float bossMovementRange;  // How far boss can move left/right from spawn
    int bossDirection;        // 1 for right, -1 for left
    
    // Boss shooting variables
    float bossRainTimer;      // Timer for boss rain gun intervals
    float bossRainInterval;   // How often boss shoots rain bullets
    
    // Static textures for all enemy types
    static Texture2D orcFrames[2];     // Shared textures for orc enemies
    static Texture2D ogreFrames[2];    // Shared textures for ogre enemies
    static Texture2D mummyFrames[2];   // Shared textures for mummy enemies
    static Texture2D bossFrames[2];    // Shared textures for boss enemy
    // Boss shooting variables
    float bossShootTimer;     // Timer for boss shooting intervals
    float bossShootInterval;  // How often boss shoots (in seconds)
    float bossRailTimer;      // Timer for boss rail gun intervals
    float bossRailInterval;   // How often boss shoots rail bullets
    int bossRailBurstCount;   // How many bullets left in current burst
    float bossRailBurstDelay; // Delay between bullets in burst
    float bossRailBurstTimer; // Timer for burst bullets
    bool bossRailBurstActive;
    bool frozen;
public:
    Enemy();
    ~Enemy();
    void SetFrozen(bool isFrozen);
    bool IsFrozen() const;
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
    
    // New beaver-related methods
    void SetBeaverTarget(Vector2 beaverPos);
    void ClearBeaverTarget();
    bool IsChasingBeaver() const;
    float GetDistanceToBeaver() const;
    
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
    
    // Boss shooting method - returns true if boss should shoot this frame
    bool ShouldBossShoot(float deltaTime, Vector2 playerPos);
    
    // Get boss shoot direction toward player
    Vector2 GetBossShootDirection(Vector2 playerPos) const;
    
    // Boss rain gun methods - returns true if boss should shoot rain bullets
    bool ShouldBossShootRain(float deltaTime, Vector2 playerPos);
    
    // Get multiple rain bullet directions with spread
    void GetRainBulletDirections(Vector2 playerPos, Vector2* directions, int bulletCount) const;
};

#endif // ENEMY_H
