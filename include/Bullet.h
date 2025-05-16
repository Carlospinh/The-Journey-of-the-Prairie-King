#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"

class Bullet : public Entity {
private:
    Vector2 direction;   // Direction of bullet movement
    float speed;         // Bullet speed
    Texture2D texture;   // Bullet texture
    static Texture2D sharedTexture;  // Shared texture for all bullets

public:
    Bullet();
    ~Bullet();
    
    // Static methods for shared resources
    static void LoadSharedTexture();
    static void UnloadSharedTexture();
    
    // Initialize a bullet
    void Init(Vector2 position, Vector2 direction);
    
    // Update bullet position
    void Update(float deltaTime) override;
    
    // Draw the bullet
    void Draw() override;
    
    // Get bullet direction
    Vector2 GetDirection() const;
    
    // Get bullet collision rectangle
    Rectangle GetCollisionRect() const;
};

#endif // BULLET_H
