#ifndef LEVEL_H
#define LEVEL_H

#include "Common.h"

class Level {
private:
    Texture2D backgrounds[2];           // Background textures (alternating)
    Texture2D completedBackgrounds[2];   // Completed level backgrounds
    Texture2D arrowTexture;             // Arrow texture for level exit
    int currentBackground;              // Current background index (0 or 1)
    float backgroundFrameTime;          // Time between background frames
    float backgroundFrameCounter;       // Frame counter for background
    float scale;                        // Scale for drawing
    
    int currentLevel;                   // Current level number
    
    // Level transition
    bool levelTransition;               // Is level transitioning
    float transitionY;                  // Transition Y position
    float transitionProgress;           // Transition progress [0-1]
    float transitionDuration;           // Transition duration in seconds
    bool showBackgroundDuringTransition; // Whether to show background during transition
    float transitionPause;              // Timer for pause between levels
    float pauseDuration;                // Duration of pause between levels
    bool pauseComplete;                 // Whether the pause is complete
    
    // Arrow blinking
    float arrowBlinkTimer;              // Timer for arrow blinking
    float arrowBlinkInterval;           // Interval for arrow blinking
    bool arrowVisible;                  // Is arrow currently visible
    
    // Level boundaries
    Rectangle levelBounds;              // Boundaries of the level playfield
    Rectangle exitZone;                 // Exit zone for level transition
    Rectangle obstacles[9];             // Obstacle rectangles

public:
    Level();
    ~Level();
    
    // Load level resources
    void LoadResources(int levelNumber);
    void UnloadResources();
    
    // Set level to completed state
    void SetCompleted();
    
    // Check if level is completed
    bool IsCompleted() const;
    
    // Update level
    void Update(float deltaTime);
    
    // Draw level background and objects
    void Draw();
    
    // Check if player is in exit zone
    bool IsPlayerInExitZone(Rectangle playerRect) const;
    
    // Start level transition
    void StartTransition();
    
    // Check if level is transitioning
    bool IsTransitioning() const;
    
    // Check if transition is complete
    bool IsTransitionComplete() const;
    
    // Get level boundaries
    Rectangle GetLevelBounds() const;
    
    // Get obstacle at index
    Rectangle GetObstacle(int index) const;
    
    // Get number of obstacles
    int GetObstacleCount() const;
    
    // Check if arrow should be visible
    bool ShouldShowArrow() const;
    
    // Draw arrow
    void DrawArrow();
    
    // Get current level number
    int GetCurrentLevel() const;
};

#endif // LEVEL_H
