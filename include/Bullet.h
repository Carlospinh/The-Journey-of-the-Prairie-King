#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"
#include "Common.h"

enum BulletType {
    BULLET_PLAYER,
    BULLET_BOSS,
    BULLET_BOSS_RAIN
};

class Bullet : public Entity {
private:
    Vector2 direction;   // Direction of bullet movement
    float speed;         // Bullet speed
    int damage;          // Bullet damage
    bool hasPenetration; // Whether bullet can penetrate enemies
    BulletType bulletType; // Type of bullet (player or boss)
    Texture2D texture;   // Bullet texture
    static Texture2D sharedTexture;  // Shared texture for player bullets
    static Texture2D bossTexture;    // Shared texture for boss bullets

public:
    Bullet();
    ~Bullet();
    
    // Static methods for shared resources
    static void LoadSharedTexture();
    static void UnloadSharedTexture();
    
    // Initialize a bullet
    void Init(Vector2 position, Vector2 direction);
    void Init(Vector2 position, Vector2 direction, int damage, bool penetration);
    void Init(Vector2 position, Vector2 direction, BulletType type);
    
    // Update bullet position
    void Update(float deltaTime) override;
    
    // Draw the bullet
    void Draw() override;
    
    // Get bullet direction
    Vector2 GetDirection() const;
    
    // Get bullet collision rectangle
    Rectangle GetCollisionRect() const;
    
    // Get bullet damage
    int GetDamage() const;
    
    // Check if bullet has penetration
    bool HasPenetration() const;
    
    // Get bullet type
    BulletType GetBulletType() const;
    
    // Reduce damage after hitting enemy (for penetration)
    void ReduceDamage(int amount);
};

#endif // BULLET_H
