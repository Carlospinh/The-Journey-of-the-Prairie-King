#include "../include/Level.h"
#include <cstdio>  // Added for sprintf

Level::Level()
    : currentBackground(0),
    backgroundFrameTime(0.5f),
    backgroundFrameCounter(0.0f),
    scale(3.8f),
    currentLevel(1),
    levelTransition(false),
    transitionY(0.0f),
    transitionProgress(0.0f),
    transitionDuration(1.5f),
    showBackgroundDuringTransition(true),
    transitionPause(0.0f),
    pauseDuration(1.5f),
    pauseComplete(false),
    arrowBlinkTimer(0.0f),
    arrowBlinkInterval(0.5f),
    arrowVisible(false) {
}

Level::~Level() {
    UnloadResources();
}

void Level::LoadResources(int levelNumber) {
    // Store the current level number
    currentLevel = levelNumber;
    
    char path1[100], path2[100], compPath1[100], compPath2[100];
    
    // Use correct filename patterns that match the actual files
    if (levelNumber == 1) {
        // Level 1 has different naming pattern
        sprintf(path1, "levels/Level_1.png");
        sprintf(path2, "levels/Level_1_2.png");
        sprintf(compPath1, "levels/Level1_Completed1.png");
        sprintf(compPath2, "levels/Level1_Completed2.png");
    } else if (levelNumber >= 3 && levelNumber <= 7) {
        // Level 3-7 follow this pattern
        sprintf(path1, "levels/Level_%d.png", levelNumber);
        sprintf(path2, "levels/Level_%d_2.png", levelNumber);
        sprintf(compPath1, "levels/Level_%d_Completed_1.png", levelNumber);
        sprintf(compPath2, "levels/Level_%d_Completed_2.png", levelNumber);
    } else if (levelNumber >= 8 && levelNumber <= 9) {
        // Level 8-9 only have one frame for each state
        sprintf(path1, "levels/Level_%d_1.png", levelNumber);
        sprintf(path2, "levels/Level_%d_1.png", levelNumber); // Use same frame twice
        sprintf(compPath1, "levels/Level_%d_Completed_1.png", levelNumber);
        sprintf(compPath2, "levels/Level_%d_Completed_1.png", levelNumber); // Use same frame twice
    } else {
        // Level 2 follows this pattern
        sprintf(path1, "levels/Level_%d.png", levelNumber);
        sprintf(path2, "levels/Level_%d_2.png", levelNumber);
        sprintf(compPath1, "levels/Level_%d_Completed_1.png", levelNumber);
        sprintf(compPath2, "levels/Level_%d_Completed_2.png", levelNumber);
    }
    
    // Try to load backgrounds
    backgrounds[0] = LoadTexture(path1);
    
    // Check if the texture was loaded successfully
    if (backgrounds[0].id == 0) {
        TraceLog(LOG_WARNING, "Failed to load texture: %s", path1);
    }
    
    backgrounds[1] = LoadTexture(path2);
    if (backgrounds[1].id == 0) {
        TraceLog(LOG_WARNING, "Failed to load texture: %s", path2);
        // If second frame failed to load, use the first frame as fallback
        backgrounds[1] = backgrounds[0];
    }
    
    // Try to load completed backgrounds, if they exist
    // If they don't exist, use copies of the regular backgrounds
    FILE *file;
    if ((file = fopen(compPath1, "r"))) {
        fclose(file);
        completedBackgrounds[0] = LoadTexture(compPath1);
        if (completedBackgrounds[0].id == 0) {
            TraceLog(LOG_WARNING, "Failed to load texture: %s", compPath1);
            completedBackgrounds[0] = backgrounds[0];
        }
    } else {
        completedBackgrounds[0] = backgrounds[0];
    }
    
    if ((file = fopen(compPath2, "r"))) {
        fclose(file);
        completedBackgrounds[1] = LoadTexture(compPath2);
        if (completedBackgrounds[1].id == 0) {
            TraceLog(LOG_WARNING, "Failed to load texture: %s", compPath2);
            completedBackgrounds[1] = backgrounds[1];
        }
    } else {
        completedBackgrounds[1] = backgrounds[1];
    }
    
    arrowTexture = LoadTexture("resources/UI/Arrow.png");
    
    // Calculate level bounds
    float bgWidth = backgrounds[0].width * scale;
    float bgHeight = backgrounds[0].height * scale;
    float bgX = (SCREEN_WIDTH - bgWidth) / 2;
    float bgY = (SCREEN_HEIGHT - bgHeight) / 2;
    
    levelBounds = Rectangle{ bgX, bgY, bgWidth, bgHeight };
    
    // Set up exit zone at bottom of level
    exitZone = Rectangle{
        bgX + bgWidth * 0.4f,
        bgY + bgHeight - 50,
        bgWidth * 0.2f,
        50
    };
    
    // Set up obstacles - moved inward to be positioned directly over the bushes
    float thickness = 50;
    float gapWidth = 50;
    float offset = 0; // Previously this was effectively -thickness
    
    // These obstacles are for the outer boundaries and the bushes
    // Moved inward to appear directly on the bushes
    obstacles[1] = Rectangle{ bgX + offset, bgY + bgHeight - thickness, 320, thickness };
    obstacles[2] = Rectangle{ bgX + offset, bgY + offset, thickness, 370 };
    obstacles[3] = Rectangle{ bgX + bgWidth - thickness - offset, bgY + offset, thickness, 370 };
    obstacles[4] = Rectangle{ bgX + offset, bgY + offset, thickness, thickness };
    obstacles[5] = Rectangle{ bgX + bgWidth - thickness - offset, bgY + offset, thickness, thickness };
    obstacles[6] = Rectangle{ bgX + offset, bgY + bgHeight - thickness, thickness, thickness };
    obstacles[7] = Rectangle{ bgX + bgWidth - thickness - offset, bgY + bgHeight - thickness, thickness, thickness };
    obstacles[8] = Rectangle{ bgX + offset, bgY + bgHeight - thickness, bgHeight, thickness };
    
    float halfWidth = (bgWidth - gapWidth) / 2;
    
    // Top barriers with gap in the middle (for the entry point) - moved inward
    obstacles[0] = Rectangle{ bgX + offset, bgY + offset, halfWidth, thickness };
    obstacles[8] = Rectangle{ bgX + offset + halfWidth + gapWidth, bgY + offset, halfWidth, thickness };
    
    // Add level 2 specific inner walls at the specified tile coordinates
    // The coordinates are in a 16x16 grid, so we need to convert them to screen coordinates
    if (levelNumber == 2) {
        float tileWidth = bgWidth / 16.0f;
        float tileHeight = bgHeight / 16.0f;
    
        // Top-left L-shape
        obstacles[9] = Rectangle{
            bgX + 4 * tileWidth,
            bgY + 4 * tileHeight,
            2 * tileWidth,
            tileHeight
        };
        obstacles[10] = Rectangle{
            bgX + 4 * tileWidth,
            bgY + 4 * tileHeight,
            tileWidth,
            2 * tileHeight
        };
    
        // Top-right L-shape (moved one column right)
        obstacles[11] = Rectangle{
            bgX + 11 * tileWidth, // 9 -> 10
            bgY + 4 * tileHeight,
            2 * tileWidth,
            tileHeight
        };
        obstacles[12] = Rectangle{
            bgX + 12 * tileWidth, // 11 -> 12
            bgY + 4 * tileHeight,
            tileWidth,
            2 * tileHeight
        };
    
        // Bottom-left L-shape (already moved down one row)
        obstacles[13] = Rectangle{
            bgX + 4 * tileWidth,
            bgY + 11 * tileHeight,
            tileWidth,
            2 * tileHeight
        };
        obstacles[14] = Rectangle{
            bgX + 4 * tileWidth,
            bgY + 12 * tileHeight,
            2 * tileWidth,
            tileHeight
        };
    
        // Bottom-right L-shape (moved one column right + already moved down one row)
        obstacles[15] = Rectangle{
            bgX + 12 * tileWidth, // 11 -> 12
            bgY + 11 * tileHeight,
            tileWidth,
            2* tileHeight
        };
        obstacles[16] = Rectangle{
            bgX + 11 * tileWidth, // 9 -> 10
            bgY + 12 * tileHeight,
            2 * tileWidth,
            tileHeight
        };
    }
    
    // Configure obstacles for levels 3-9 as needed
    // For now, we'll use the same basic obstacles for all levels except level 2
    // You can add specific obstacle configurations for each level here
}

void Level::UnloadResources() {
    // Keep track of which textures we've already unloaded
    int unloadedIds[4] = {0};
    int count = 0;
    
    // Unload backgrounds
    for (int i = 0; i < 2; i++) {
        bool alreadyUnloaded = false;
        for (int j = 0; j < count; j++) {
            if (backgrounds[i].id == unloadedIds[j]) {
                alreadyUnloaded = true;
                break;
            }
        }
        
        if (!alreadyUnloaded) {
            UnloadTexture(backgrounds[i]);
            unloadedIds[count++] = backgrounds[i].id;
        }
    }
    
    // Unload completed backgrounds (if they're different)
    for (int i = 0; i < 2; i++) {
        bool alreadyUnloaded = false;
        for (int j = 0; j < count; j++) {
            if (completedBackgrounds[i].id == unloadedIds[j]) {
                alreadyUnloaded = true;
                break;
            }
        }
        
        if (!alreadyUnloaded) {
            UnloadTexture(completedBackgrounds[i]);
            unloadedIds[count++] = completedBackgrounds[i].id;
        }
    }
    
    UnloadTexture(arrowTexture);
}

void Level::SetCompleted() {
    // Only unload if the backgrounds are different from completed backgrounds
    if (backgrounds[0].id != completedBackgrounds[0].id) {
        Texture2D tempBg0 = backgrounds[0];
        backgrounds[0] = completedBackgrounds[0];
        UnloadTexture(tempBg0);
    }
    
    if (backgrounds[1].id != completedBackgrounds[1].id) {
        Texture2D tempBg1 = backgrounds[1];
        backgrounds[1] = completedBackgrounds[1];
        UnloadTexture(tempBg1);
    }
}

bool Level::IsCompleted() const {
    // Check if we're using completed backgrounds
    // This is a simplified approach - you could have a flag instead
    return backgrounds[0].id == completedBackgrounds[0].id;
}

void Level::Update(float deltaTime) {
    // Update background animation (only when not transitioning)
    if (!levelTransition) {
        backgroundFrameCounter += deltaTime;
        if (backgroundFrameCounter >= backgroundFrameTime) {
            backgroundFrameCounter = 0.0f;
            currentBackground = (currentBackground + 1) % 2;
        }
    }
    
    // Update arrow blinking if level is completed
    if (IsCompleted()) {
        arrowBlinkTimer += deltaTime;
        if (arrowBlinkTimer >= arrowBlinkInterval) {
            arrowBlinkTimer = 0.0f;
            arrowVisible = !arrowVisible;
        }
    }
    
    // Update level transition in two phases:
    // 1. First slide up the current level
    // 2. Then pause showing only the cubes for 1-2 seconds
    if (levelTransition) {
        if (!pauseComplete) {
            // Phase 1: Slide animation
            if (transitionProgress < 1.0f) {
                transitionProgress += deltaTime / transitionDuration;
                
                // Calculate eased progress (cubic ease out)
                float easedProgress = 1.0f - powf(1.0f - transitionProgress, 3);
                transitionY = levelBounds.y + levelBounds.height - (levelBounds.height * easedProgress);
                
                // When slide animation completes, start pause timer and hide the background
                if (transitionProgress >= 1.0f) {
                    transitionProgress = 1.0f;
                    showBackgroundDuringTransition = false;
                    transitionPause = 0.0f;
                }
            } 
            else {
                // Phase 2: Pause showing only cubes
                transitionPause += deltaTime;
                if (transitionPause >= pauseDuration) {
                    pauseComplete = true;
                }
            }
        }
    }
}

void Level::Draw() {
    Rectangle sourceRec = {
        0, 0,
        (float)backgrounds[currentBackground].width, 
        (float)backgrounds[currentBackground].height
    };
    
    Rectangle destRec = {
        levelBounds.x, levelBounds.y,
        levelBounds.width, levelBounds.height
    };
    
    Vector2 origin = { 0, 0 };
    
    if (levelTransition) {
        if (transitionProgress < 1.0f && showBackgroundDuringTransition) {
            // Phase 1: Draw current level sliding up out of view
            float easedProgress = 1.0f - powf(1.0f - transitionProgress, 3);
            transitionY = levelBounds.y + levelBounds.height - (levelBounds.height * easedProgress);
            
            DrawTexturePro(backgrounds[currentBackground], sourceRec, 
                Rectangle{ levelBounds.x, transitionY - levelBounds.height, levelBounds.width, levelBounds.height }, 
                origin, 0.0f, WHITE);
        }
        
        // Always draw the obstacle blocks during transition (even when map is gone)
        // This creates the effect of only cubes remaining after the level slides away
        int obstacleCount = GetObstacleCount();
        for (int i = 0; i < obstacleCount; i++) {
            DrawRectangleLinesEx(obstacles[i], 2, DARKGRAY);
        }
    }
    else {
        // Normal gameplay: draw level and obstacles
        DrawTexturePro(backgrounds[currentBackground], sourceRec, destRec, origin, 0.0f, WHITE);
        
        // Draw obstacle blocks on top of the background
        int obstacleCount = GetObstacleCount();
        for (int i = 0; i < obstacleCount; i++) {
            DrawRectangleLinesEx(obstacles[i], 2, DARKGRAY);
        }
    }
}

bool Level::IsPlayerInExitZone(Rectangle playerRect) const {
    return IsCompleted() && CheckCollisionRecs(playerRect, exitZone);
}

void Level::StartTransition() {
    if (!levelTransition) {
        levelTransition = true;
        transitionProgress = 0.0f;
        transitionY = levelBounds.y + levelBounds.height;
        showBackgroundDuringTransition = true;
        pauseComplete = false;
        transitionPause = 0.0f;
    }
}

bool Level::IsTransitioning() const {
    return levelTransition;
}

bool Level::IsTransitionComplete() const {
    return levelTransition && pauseComplete;
}

Rectangle Level::GetLevelBounds() const {
    return levelBounds;
}

Rectangle Level::GetObstacle(int index) const {
    // For level 2, we have additional inner walls (up to index 16)
    // For other levels, we only have the standard 9 obstacles
    int maxIndex = (currentLevel == 2) ? 16 : 8;
    
    if (index >= 0 && index <= maxIndex) {
        return obstacles[index];
    }
    return Rectangle{ 0, 0, 0, 0 };
}

int Level::GetObstacleCount() const {
    // Return the appropriate number of obstacles based on the current level
    return (currentLevel == 2) ? 17 : 9;
}

bool Level::ShouldShowArrow() const {
    return IsCompleted() && arrowVisible;
}

void Level::DrawArrow() {
    if (ShouldShowArrow()) {
        float arrowScale = 0.18f;
        Vector2 arrowPos = {
            levelBounds.x + levelBounds.width / 2 - (arrowTexture.width * arrowScale / 2) + 30,
            levelBounds.y + levelBounds.height - (arrowTexture.height * arrowScale) - 20
        };
        DrawTextureEx(arrowTexture, arrowPos, 0.0f, arrowScale, WHITE);
    }
}

int Level::GetCurrentLevel() const {
    return currentLevel;
}
