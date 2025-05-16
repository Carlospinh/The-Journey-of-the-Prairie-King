#ifndef DEATH_ANIMATION_H
#define DEATH_ANIMATION_H

#include "Entity.h"

class DeathAnimation : public Entity {
private:
    Texture2D frames[6];      // Animation frames
    int currentFrame;         // Current animation frame
    float frameTimer;         // Animation frame timer
    float frameDuration;      // Duration of each frame
    float displayTimer;       // How long to display the animation
    float displayDuration;    // Total display time
    
public:
    DeathAnimation();
    ~DeathAnimation();
    
    // Initialize animation at position
    void Init(Vector2 position);
    
    // Load textures for this animation
    void LoadTextures();
    
    // Unload textures for this animation
    void UnloadTextures();
    
    // Update animation
    void Update(float deltaTime) override;
    
    // Draw animation
    void Draw() override;
    
    // Check if animation is complete
    bool IsComplete() const;
};

#endif // DEATH_ANIMATION_H
