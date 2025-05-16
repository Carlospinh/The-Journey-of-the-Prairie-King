#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include "Entity.h"

class Collectible : public Entity {
protected:
    Texture2D texture;  // Texture for this collectible

public:
    Collectible();
    virtual ~Collectible();
    
    // Set texture for collectible
    void SetTexture(Texture2D texture);
    
    // Get collectible collision rectangle
    Rectangle GetCollisionRect() const;
    
    // Draw collectible
    virtual void Draw() override;
    
    // Update collectible (overridden by derived classes if needed)
    virtual void Update(float deltaTime) override;
    
    // Check if collectible is outside level boundaries
    bool IsOutOfBounds(float bgX, float bgY, float bgWidth, float bgHeight) const;
};

#endif // COLLECTIBLE_H
