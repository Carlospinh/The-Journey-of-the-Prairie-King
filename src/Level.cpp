#include "../include/Level.h"
#include <cstdio>  // Added for sprintf
Level::Level()
    : currentBackground(0),
    backgroundFrameTime(0.5f),
    backgroundFrameCounter(0.0f),
    scale(3.8f),
    currentLevel(1),
    nextLevel(1),
    levelTransition(false),
    transitionY(0.0f),
    transitionProgress(0.0f),
    transitionDuration(1.5f),
    showBackgroundDuringTransition(true),
    transitionPause(0.0f),
    pauseDuration(1.0f),
    pauseComplete(false),
    currentLevelY(0.0f),
    nextLevelY(0.0f),
    isSwipeTransition(false),
    playerStartPos({0, 0}),
    playerEndPos({0, 0}),
    shouldAnimatePlayer(false),
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
    } else if (levelNumber == 10) {
        // Level 10 uses boss level textures
        sprintf(path1, "levels/Boss_Level_1.png");
        sprintf(path2, "levels/Boss_Level_1.png"); // Use same frame for both
        sprintf(compPath1, "levels/Boss_Level_Completed_1.png");
        sprintf(compPath2, "levels/Boss_Level_Completed_2.png");
    } else {
        // Level 2 follows this pattern
        sprintf(path1, "levels/Level_%d.png", levelNumber);
        sprintf(path2, "levels/Level_%d_2.png", levelNumber);
        sprintf(compPath1, "levels/Level_%d_Completed_1.png", levelNumber);
        sprintf(compPath2, "levels/Level_%d_Completed_2.png", levelNumber);
    }
    
    backgrounds[0] = LoadTexture(path1);
    
    if (backgrounds[0].id == 0) {
        TraceLog(LOG_WARNING, "Failed to load texture: %s", path1);
    }
    
    backgrounds[1] = LoadTexture(path2);
    if (backgrounds[1].id == 0) {
        TraceLog(LOG_WARNING, "Failed to load texture: %s", path2);
        backgrounds[1] = backgrounds[0];
    }
    
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
    
    float bgWidth = backgrounds[0].width * scale;
    float bgHeight = backgrounds[0].height * scale;
    float bgX = (SCREEN_WIDTH - bgWidth) / 2;
    float bgY = (SCREEN_HEIGHT - bgHeight) / 2;
    levelBounds = { bgX, bgY, bgWidth, bgHeight };
    
    // Set up exit zone at bottom of level
    exitZone = {
        bgX + bgWidth * 0.4f,
        bgY + bgHeight - 50,
        bgWidth * 0.2f,
        50
    };
    
    // Calculate tile dimensions for 16x16 grid
    float tileWidth = bgWidth / 16.0f;
    float tileHeight = bgHeight / 16.0f;
    
    // Clear all obstacles first - use maximum possible size
    for (int i = 0; i < 50; i++) {
        obstacles[i] = {0, 0, 0, 0};
        penetrableObstacles[i] = false;  // Default: all obstacles are solid
    }
    
    int obstacleIndex = 0;
    
    // Create boundary walls using 16x16 grid system
    // These represent the green bushes around the perimeter
    // Gap distribution: 7 bushes + 3 gap + 6 bushes = 16 total
    
    float leftBushes = 7.0f;    // 7 bushes on left/top side
    float gapSize = 3.0f;       // 3 empty blocks for gap
    float rightBushes = 6.0f;   // 6 bushes on right/bottom side
    
    // Top wall - 7 bushes, 3 gap, 6 bushes
    obstacles[obstacleIndex++] = {
        bgX,
        bgY,
        leftBushes * tileWidth,
        tileHeight * 0.95f
    };
    obstacles[obstacleIndex++] = {
        bgX + (leftBushes + gapSize) * tileWidth,
        bgY,
        rightBushes * tileWidth,
        tileHeight * 0.95f
    };
    
    // Bottom wall - 7 bushes, gap reduced by 0.25 from both sides, 6 bushes
    obstacles[obstacleIndex++] = {
        bgX,
        bgY + 15 * tileHeight,
        (leftBushes + 0.25f) * tileWidth,  // Extended 0.25 blocks inward
        tileHeight * 0.95f
    };
    obstacles[obstacleIndex++] = {
        bgX + (leftBushes + gapSize - 0.25f) * tileWidth,  // Started 0.25 blocks inward
        bgY + 15 * tileHeight,
        rightBushes * tileWidth,
        tileHeight * 0.95f
    };
    
    // Left wall - 7 bushes (top), 3 gap, 6 bushes (bottom)
    obstacles[obstacleIndex++] = {
        bgX,
        bgY,
        tileWidth * 0.95f,
        leftBushes * tileHeight
    };
    obstacles[obstacleIndex++] = {
        bgX,
        bgY + (leftBushes + gapSize + 0.5f) * tileHeight,  // Gap increased by 0.5, then translated down 1 block total
        tileWidth * 0.95f,
        (rightBushes - 0.5f) * tileHeight  // Height adjusted accordingly
    };
    
    // Right wall - 7 bushes (top), 3 gap, 6 bushes (bottom)
    obstacles[obstacleIndex++] = {
        bgX + 15 * tileWidth,
        bgY,
        tileWidth * 0.95f,
        leftBushes * tileHeight
    };
    obstacles[obstacleIndex++] = {
        bgX + 15 * tileWidth,
        bgY + (leftBushes + gapSize + 0.5f) * tileHeight,  // Gap increased by 0.5, then translated down 1 block total
        tileWidth * 0.95f,
        (rightBushes - 0.5f) * tileHeight  // Height adjusted accordingly
    };
    
    // Add level 2 specific inner walls at the specified tile coordinates
    // Keep the existing Level 2 logic unchanged
    if (levelNumber == 2) {
        // Top-left L-shape
        obstacles[obstacleIndex++] = {
            bgX + 4 * tileWidth,
            bgY + 4 * tileHeight,
            2 * tileWidth,
            tileHeight
        };
        obstacles[obstacleIndex++] = {
            bgX + 4 * tileWidth,
            bgY + 4 * tileHeight,
            tileWidth,
            2 * tileHeight
        };
    
        // Top-right L-shape (moved one column right)
        obstacles[obstacleIndex++] = {
            bgX + 11 * tileWidth, // 9 -> 10
            bgY + 4 * tileHeight,
            2 * tileWidth,
            tileHeight
        };
        obstacles[obstacleIndex++] = {
            bgX + 12 * tileWidth, // 11 -> 12
            bgY + 4 * tileHeight,
            tileWidth,
            2 * tileHeight
        };
    
        // Bottom-left L-shape (already moved down one row)
        obstacles[obstacleIndex++] = {
            bgX + 4 * tileWidth,
            bgY + 11 * tileHeight,
            tileWidth,
            2 * tileHeight
        };
        obstacles[obstacleIndex++] = {
            bgX + 4 * tileWidth,
            bgY + 12 * tileHeight,
            2 * tileWidth,
            tileHeight
        };
    
        // Bottom-right L-shape (moved one column right + already moved down one row)
        obstacles[obstacleIndex++] = {
            bgX + 12 * tileWidth, // 11 -> 12
            bgY + 11 * tileHeight,
            tileWidth,
            2* tileHeight
        };
        obstacles[obstacleIndex++] = {
            bgX + 11 * tileWidth, // 9 -> 10
            bgY + 12 * tileHeight,
            2 * tileWidth,
            tileHeight
        };
    }
    
    // Configure obstacles for levels 3-10 as needed
    if (levelNumber >= 3 && levelNumber <= 10) {
        // Add level-specific obstacles here if needed
        // The boundary walls are already set up above
        
        // Example: Add some inner obstacles for higher levels
        // You can customize this based on each level's design
        switch (levelNumber) {
            case 3:
                // Add level 3 specific obstacles - Diamond pattern (4 obstacles)
                // Center the diamond around position (8, 8) on the 16x16 grid
                // Each obstacle is now 4 blocks away from center (was 3 blocks)
                obstacles[obstacleIndex++] = {
                    bgX + 1 * tileWidth,     // Start at column 1
                    bgY + 1 * tileHeight,   // Row 14 (0-indexed as 13)
                    2 * tileWidth,           // Span 4 bushes horizontally
                    tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 1 * tileWidth,     // Start at column 1
                    bgY + 2 * tileHeight,   // Row 14 (0-indexed as 13)
                    1 * tileWidth,           // Span 4 bushes horizontally
                    tileHeight
                };

                obstacles[obstacleIndex++] = {
                    bgX + 13 * tileWidth,     // Start at column 1
                    bgY + 1 * tileHeight,   // Row 14 (0-indexed as 13)
                    2 * tileWidth,           // Span 4 bushes horizontally
                    tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 14 * tileWidth,     // Start at column 1
                    bgY + 2 * tileHeight,   // Row 14 (0-indexed as 13)
                    1 * tileWidth,           // Span 4 bushes horizontally
                    tileHeight
                };

                obstacles[obstacleIndex++] = {
                    bgX + 1 * tileWidth,     // Start at column 1
                    bgY + 13 * tileHeight,   // Row 14 (0-indexed as 13)
                    1 * tileWidth,           // Span 4 bushes horizontally
                    tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 1 * tileWidth,     // Start at column 1
                    bgY + 14 * tileHeight,   // Row 14 (0-indexed as 13)
                    2 * tileWidth,           // Span 4 bushes horizontally
                    tileHeight
                };

                obstacles[obstacleIndex++] = {
                    bgX + 14 * tileWidth,     // Start at column 1
                    bgY + 13 * tileHeight,   // Row 14 (0-indexed as 13)
                    1 * tileWidth,           // Span 4 bushes horizontally
                    tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 13 * tileWidth,     // Start at column 1
                    bgY + 14 * tileHeight,   // Row 14 (0-indexed as 13)
                    1 * tileWidth,           // Span 4 bushes horizontally
                    tileHeight
                };
                // Top obstacle - moved one block further up
                obstacles[obstacleIndex++] = {
                    bgX + 8 * tileWidth,
                    bgY + 4 * tileHeight,
                    tileWidth,
                    tileHeight
                };
                
                // Left obstacle - moved one block further left
                obstacles[obstacleIndex++] = {
                    bgX + 4 * tileWidth,
                    bgY + 8 * tileHeight,
                    tileWidth,
                    tileHeight
                };
                
                // Right obstacle - moved one block further right
                obstacles[obstacleIndex++] = {
                    bgX + 12 * tileWidth,
                    bgY + 8 * tileHeight,
                    tileWidth,
                    tileHeight
                };
                
                // Bottom obstacle - moved one block further down
                obstacles[obstacleIndex++] = {
                    bgX + 8 * tileWidth,
                    bgY + 12 * tileHeight,
                    tileWidth,
                    tileHeight
                };
                break;
            case 4:
                // Add level 4 specific obstacles - 7x7 square centered at (8,8) with gaps in middle of each side
                // Center of 16x16 grid is at (8,8), so 7x7 square spans from (5,5) to (11,11)
                
                // Top side of square (y=5, x=5 to 11) - skip middle block (x=8)
                obstacles[obstacleIndex++] = {
                    bgX + 5 * tileWidth, bgY + 5 * tileHeight,
                    3 * tileWidth, tileHeight  // x=5,6,7
                };
                obstacles[obstacleIndex++] = {
                    bgX + 9 * tileWidth, bgY + 5 * tileHeight,
                    3 * tileWidth, tileHeight  // x=9,10,11
                };
                
                // Bottom side of square (y=11, x=5 to 11) - skip middle block (x=8)
                obstacles[obstacleIndex++] = {
                    bgX + 5 * tileWidth, bgY + 11 * tileHeight,
                    3 * tileWidth, tileHeight  // x=5,6,7
                };
                obstacles[obstacleIndex++] = {
                    bgX + 9 * tileWidth, bgY + 11 * tileHeight,
                    3 * tileWidth, tileHeight  // x=9,10,11
                };
                
                // Left side of square (x=5, y=6 to 10) - skip middle block (y=8)
                obstacles[obstacleIndex++] = {
                    bgX + 5 * tileWidth, bgY + 6 * tileHeight,
                    tileWidth, 2 * tileHeight  // y=6,7
                };
                obstacles[obstacleIndex++] = {
                    bgX + 5 * tileWidth, bgY + 9 * tileHeight,
                    tileWidth, 2 * tileHeight  // y=9,10
                };
                
                // Right side of square (x=11, y=6 to 10) - skip middle block (y=8)
                obstacles[obstacleIndex++] = {
                    bgX + 11 * tileWidth, bgY + 6 * tileHeight,
                    tileWidth, 2 * tileHeight  // y=6,7
                };
                obstacles[obstacleIndex++] = {
                    bgX + 11 * tileWidth, bgY + 10 * tileHeight,
                    tileWidth, 1 * tileHeight  // y=9 only (removed y=10 block)
                };
                break;
    case 5:
                // Add level 5 specific obstacles - covering bush patterns
                // Based on 16x16 grid system
                
                // Row 2: Horizontal block spanning 4 bushes at start, 3 blocks at end
                obstacles[obstacleIndex++] = {
                    bgX + 0 * tileWidth,     // Start at column 1
                    bgY + 1 * tileHeight,    // Row 2
                    4 * tileWidth,           // Span 4 bushes horizontally
                    tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 13 * tileWidth,    // Near end of row (columns 12-14)
                    bgY + 1 * tileHeight,    // Row 2
                    3 * tileWidth,           // 3 blocks horizontal
                    tileHeight
                };
                
                // Row 14 (16-2): Same pattern as row 2
                obstacles[obstacleIndex++] = {
                    bgX + 0 * tileWidth,     // Start at column 1
                    bgY + 14 * tileHeight,   // Row 14 (0-indexed as 13)
                    4 * tileWidth,           // Span 4 bushes horizontally
                    tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 12 * tileWidth,    // Near end of row (columns 12-14)
                    bgY + 13 * tileHeight,   // Row 14 (0-indexed as 13)
                    3 * tileWidth,           // 3 blocks horizontal
                    tileHeight
                };
                
                // Row 3: 2x2 blocks at start and end
                obstacles[obstacleIndex++] = {
                    bgX + 0 * tileWidth,     // Start at column 1
                    bgY + 2 * tileHeight,    // Row 3
                    2 * tileWidth,           // 2x2 block
                    2 * tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 14 * tileWidth,    // End columns (13-14)
                    bgY + 1 * tileHeight,    // Row 3
                    2 * tileWidth,           // 2x2 block
                    2 * tileHeight
                };
                //new 
                obstacles[obstacleIndex++] = {
                    bgX + 13 * tileWidth,    // End columns (13-14)
                    bgY + 3 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 3 * tileWidth,    // End columns (13-14)
                    bgY + 3 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };

                obstacles[obstacleIndex++] = {
                    bgX + 3 * tileWidth,    // End columns (13-14)
                    bgY + 12 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 13 * tileWidth,    // End columns (13-14)
                    bgY + 12 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 13 * tileWidth,    // End columns (13-14)
                    bgY + 14 * tileHeight,    // Row 3
                    2 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };
                // Row 4: 2x2 blocks at start and end (part of the previous 2x2 blocks)
                // These are already covered by the 2x2 blocks above
                
                // Mirror at bottom: Row 12-13 (corresponding to rows 3-4)
              
                break;
            case 6:
                // Add level 6 specific obstacles

                obstacles[obstacleIndex++] = {
                    bgX + 2 * tileWidth,    // End columns (13-14)
                    bgY + 3 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 3 * tileWidth,    // End columns (13-14)
                    bgY + 4 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };

                obstacles[obstacleIndex++] = {
                    bgX + 11 * tileWidth,    // End columns (13-14)
                    bgY + 3 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 10 * tileWidth,    // End columns (13-14)
                    bgY + 6 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 10 * tileWidth,    // End columns (13-14)
                    bgY + 8 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    2 * tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 5 * tileWidth,    // End columns (13-14)
                    bgY + 9 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };

                obstacles[obstacleIndex++] = {
                    bgX + 3 * tileWidth,    // End columns (13-14)
                    bgY + 12 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 10 * tileWidth,    // End columns (13-14)
                    bgY + 12 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 12 * tileWidth,    // End columns (13-14)
                    bgY + 12 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 12 * tileWidth,    // End columns (13-14)
                    bgY + 10 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };
                break;
            case 7:
                // Add level 7 specific obstacles

                obstacles[obstacleIndex++] = {
                    bgX + 0 * tileWidth,    // End columns (13-14)
                    bgY + 5 * tileHeight,    // Row 3
                    4 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };

                obstacles[obstacleIndex++] = {
                    bgX + 5 * tileWidth,    // End columns (13-14)
                    bgY + 5 * tileHeight,    // Row 3
                    3 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 9 * tileWidth,    // End columns (13-14)
                    bgY + 5 * tileHeight,    // Row 3
                    3 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };

                obstacles[obstacleIndex++] = {
                    bgX + 13 * tileWidth,    // End columns (13-14)
                    bgY + 5 * tileHeight,    // Row 3
                    3 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };


                obstacles[obstacleIndex++] = {
                    bgX + 0 * tileWidth,    // End columns (13-14)
                    bgY + 10 * tileHeight,    // Row 3
                    4 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };

                obstacles[obstacleIndex++] = {
                    bgX + 5 * tileWidth,    // End columns (13-14)
                    bgY + 10 * tileHeight,    // Row 3
                    3 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 9 * tileWidth,    // End columns (13-14)
                    bgY + 10 * tileHeight,    // Row 3
                    3 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };

                obstacles[obstacleIndex++] = {
                    bgX + 13 * tileWidth,    // End columns (13-14)
                    bgY + 10 * tileHeight,    // Row 3
                    3 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };

                break;
            case 8:
                // Add level 8 specific obstacles

                obstacles[obstacleIndex++] = {
                    bgX + 4 * tileWidth,    // End columns (13-14)
                    bgY + 4 * tileHeight,    // Row 3
                    2 * tileWidth,           // 2x2 block
                    2 * tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 10 * tileWidth,    // End columns (13-14)
                    bgY + 4 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    2 * tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 12 * tileWidth,    // End columns (13-14)
                    bgY + 4 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    2 * tileHeight
                };
                //bottom half
              
                obstacles[obstacleIndex++] = {
                    bgX + 4 * tileWidth,    // End columns (13-14)
                    bgY + 12 * tileHeight,    // Row 3
                    2 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 4 * tileWidth,    // End columns (13-14)
                    bgY + 10 * tileHeight,    // Row 3
                    2 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };
                obstacles[obstacleIndex++] = {
                    bgX + 10 * tileWidth,    // End columns (13-14)
                    bgY + 10 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                }; 
                obstacles[obstacleIndex++] = {
                    bgX + 10 * tileWidth,    // End columns (13-14)
                    bgY + 12 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                }; 
                obstacles[obstacleIndex++] = {
                    bgX + 12 * tileWidth,    // End columns (13-14)
                    bgY + 10 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };  
                obstacles[obstacleIndex++] = {
                    bgX + 12 * tileWidth,    // End columns (13-14)
                    bgY + 12 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };  
                break;
            case 9:
                // Add level 9 specific obstacles
                obstacles[obstacleIndex++] = {
                    bgX + 0 * tileWidth,    // End columns (13-14)
                    bgY + 1 * tileHeight,    // Row 3
                    7 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };  
                obstacles[obstacleIndex++] = {
                    bgX + 10 * tileWidth,    // End columns (13-14)
                    bgY + 1 * tileHeight,    // Row 3
                    6 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };  

                obstacles[obstacleIndex++] = {
                    bgX + 0 * tileWidth,    // End columns (13-14)
                    bgY + 14 * tileHeight,    // Row 3
                    7 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };  
                obstacles[obstacleIndex++] = {
                    bgX + 10 * tileWidth,    // End columns (13-14)
                    bgY + 14 * tileHeight,    // Row 3
                    6 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };  

                obstacles[obstacleIndex++] = {
                    bgX + 6 * tileWidth,    // End columns (13-14)
                    bgY + 8 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };  

                obstacles[obstacleIndex++] = {
                    bgX + 8 * tileWidth,    // End columns (13-14)
                    bgY + 6 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };  
                obstacles[obstacleIndex++] = {
                    bgX + 10 * tileWidth,    // End columns (13-14)
                    bgY + 8 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };  
                obstacles[obstacleIndex++] = {
                    bgX + 8 * tileWidth,    // End columns (13-14)
                    bgY + 9 * tileHeight,    // Row 3
                    1 * tileWidth,           // 2x2 block
                    1 * tileHeight
                };  
                break;
            case 10:
                // Add level 10 specific obstacles - Boss level with strategic cover
                // Create a more complex obstacle layout for the boss level
                // 8 obs
                // Central fortress-like structure
                obstacles[obstacleIndex] = {
                    bgX + 0 * tileWidth,    // Left pillar
                    bgY + 8 * tileHeight,
                    8 * tileWidth,
                    1 * tileHeight
                };
                penetrableObstacles[obstacleIndex] = true;  // This is penetrable by bullets
                obstacleIndex++;
                
                // Bridge gap obstacle - blocks the gap between the two penetrable obstacles
                obstacles[obstacleIndex] = {
                    bgX + 8 * tileWidth,    // Gap position (column 8)
                    bgY + 8 * tileHeight,   // Middle row
                    1 * tileWidth,          // 1x1 block
                    1 * tileHeight
                };
                penetrableObstacles[obstacleIndex] = false;  // This is solid initially
                obstacleIndex++;
                
                obstacles[obstacleIndex] = {
                    bgX + 9 * tileWidth,    // Right pillar
                    bgY + 8 * tileHeight,
                    7 * tileWidth,
                    1 * tileHeight
                };
                penetrableObstacles[obstacleIndex] = true;  // This is penetrable by bullets
                obstacleIndex++;

                // Rest of the obstacles are solid (default false)
                obstacles[obstacleIndex++] = {
                    bgX + 7 * tileWidth,    
                    bgY + 12 * tileHeight,
                    3 * tileWidth,
                    1 * tileHeight
                };  
                obstacles[obstacleIndex++] = {
                    bgX + 8 * tileWidth,    
                    bgY + 4 * tileHeight,
                    1 * tileWidth,
                    1 * tileHeight
                }; 
                obstacles[obstacleIndex++] = {
                    bgX + 5 * tileWidth,    
                    bgY + 6 * tileHeight,
                    1 * tileWidth,
                    1 * tileHeight
                }; 
                obstacles[obstacleIndex++] = {
                    bgX + 10 * tileWidth,    
                    bgY + 6 * tileHeight,
                    1 * tileWidth,
                    1 * tileHeight
                }; 
                break;
        }
    }
}
void Level::UnloadResources() {
    // Keep track of which textures we've already unloaded
    int unloadedIds[8] = {0}; // Increased to handle next level textures too
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
        
        if (!alreadyUnloaded && backgrounds[i].id != 0) {
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
        
        if (!alreadyUnloaded && completedBackgrounds[i].id != 0) {
            UnloadTexture(completedBackgrounds[i]);
            unloadedIds[count++] = completedBackgrounds[i].id;
        }
    }
    
    // Unload next level backgrounds (if they're different)
    for (int i = 0; i < 2; i++) {
        bool alreadyUnloaded = false;
        for (int j = 0; j < count; j++) {
            if (nextLevelBackgrounds[i].id == unloadedIds[j]) {
                alreadyUnloaded = true;
                break;
            }
        }
        
        if (!alreadyUnloaded && nextLevelBackgrounds[i].id != 0) {
            UnloadTexture(nextLevelBackgrounds[i]);
            unloadedIds[count++] = nextLevelBackgrounds[i].id;
        }
    }
    
    if (arrowTexture.id != 0) {
        UnloadTexture(arrowTexture);
    }
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
    if (levelTransition && isSwipeTransition) {
        // Use the new swipe transition drawing
        DrawSwipeTransition();
        return;
    }
    
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
                { levelBounds.x, transitionY - levelBounds.height, levelBounds.width, levelBounds.height }, 
                origin, 0.0f, WHITE);
        }
        
        // Always draw the obstacle blocks during transition (even when map is gone)
        // This creates the effect of only cubes remaining after the level slides away
        int obstacleCount = GetObstacleCount();
        for (int i = 0; i < obstacleCount; i++) {
            DrawRectangleLinesEx(obstacles[i], 2, BLANK);
        }
    }
    else {
        // Normal gameplay: draw level and obstacles
        DrawTexturePro(backgrounds[currentBackground], sourceRec, destRec, origin, 0.0f, WHITE);
        
        // Draw obstacle blocks on top of the background
        int obstacleCount = GetObstacleCount();
        for (int i = 0; i < obstacleCount; i++) {
            DrawRectangleLinesEx(obstacles[i], 2, BLANK);
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
    // Updated obstacle counts for each level
    int maxIndex;
    if (currentLevel == 2) {
        maxIndex = 16;  // 8 boundary walls + 8 L-shaped inner obstacles
    } else if (currentLevel == 3) {
        maxIndex = 12 + 12;  // 8 boundary walls + 4 diamond pattern obstacles
    } else if (currentLevel == 4) {
        maxIndex = 16;  // 8 boundary walls + 8 square perimeter obstacles
    } else if (currentLevel == 5) {
        maxIndex = 18; 
    } else if (currentLevel == 6) {
        maxIndex = 18; 
    } else if (currentLevel == 7) {
        maxIndex = 12+4;  // 8 boundary walls + 4 horizontal bars
    } else if (currentLevel == 8) {
       maxIndex = 8+ 9; 
    } else if (currentLevel == 9) {
        maxIndex = 8+ 8; 
    } else if (currentLevel == 10) {
        maxIndex = 8 + 9 + 1;  // 8 boundary walls + 9 boss obstacles (including bridge gap) + 1 extra
    } else {
        maxIndex = 8;   // Just boundary walls for other levels
    }
    
    if (index >= 0 && index <= maxIndex) {
        return obstacles[index];
    }
    return { 0, 0, 0, 0 };
}

int Level::GetObstacleCount() const {
    // Return the appropriate number of obstacles based on the current level
    if (currentLevel == 2) {
        return 17;  // 8 boundary walls + 8 L-shaped inner obstacles + 1 extra
    } else if (currentLevel == 3) {
        return 13 + 12;  // 8 boundary walls + 4 diamond pattern obstacles + 1 extra
    } else if (currentLevel == 4) {
        return 17;  // 8 boundary walls + 8 square perimeter obstacles + 1 extra
    } else if (currentLevel == 5) {
        return 19;  // 8 boundary walls + 10 scattered obstacles + 1 extra
    } else if (currentLevel == 6) {
        return 19;  // 8 boundary walls + 10 scattered obstacles + 1 extra
    } else if (currentLevel == 7) {
        return 13+4;  // 8 boundary walls + 4 horizontal bars + 1 extra
    } else if (currentLevel == 8) {
        return 8+ 9 +1; 
    } else if (currentLevel == 9) {
        return 8 + 8 +1; 
    } else if (currentLevel == 10) {
        return 8 + 9 + 1;  // 8 boundary walls + 9 boss obstacles (including bridge gap) + 1 extra
    } else {
        return 9;   // Just the 8 boundary walls + 1 extra for other levels
    }
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

void Level::StartSwipeTransition(int nextLevelNumber) {
    if (!levelTransition) {
        nextLevel = nextLevelNumber;
        levelTransition = true;
        isSwipeTransition = true;
        transitionProgress = 0.0f;
        transitionDuration = 2.0f; // Slower transition (was 1.2f)
        pauseComplete = false;
        transitionPause = 0.0f;
        pauseDuration = 0.3f; // Slightly longer pause
        
        // Initialize positions for swipe transition
        currentLevelY = levelBounds.y;
        nextLevelY = levelBounds.y + levelBounds.height;
        
        // Load next level backgrounds for transition
        char path1[100], path2[100];
        
        if (nextLevelNumber == 1) {
            sprintf(path1, "levels/Level_1.png");
            sprintf(path2, "levels/Level_1_2.png");
        } else if (nextLevelNumber >= 3 && nextLevelNumber <= 7) {
            sprintf(path1, "levels/Level_%d.png", nextLevelNumber);
            sprintf(path2, "levels/Level_%d_2.png", nextLevelNumber);
        } else if (nextLevelNumber >= 8 && nextLevelNumber <= 9) {
            sprintf(path1, "levels/Level_%d_1.png", nextLevelNumber);
            sprintf(path2, "levels/Level_%d_1.png", nextLevelNumber);
        } else if (nextLevelNumber == 10) {
            sprintf(path1, "levels/Boss_Level_1.png");
            sprintf(path2, "levels/Boss_Level_1.png");
        } else {
            sprintf(path1, "levels/Level_%d.png", nextLevelNumber);
            sprintf(path2, "levels/Level_%d_2.png", nextLevelNumber);
        }
        
        nextLevelBackgrounds[0] = LoadTexture(path1);
        nextLevelBackgrounds[1] = LoadTexture(path2);
        
        if (nextLevelBackgrounds[1].id == 0) {
            nextLevelBackgrounds[1] = nextLevelBackgrounds[0];
        }
    }
}

void Level::DrawSwipeTransition() {
    if (!isSwipeTransition) return;
    
    Rectangle sourceRec = {
        0, 0,
        (float)backgrounds[currentBackground].width,
        (float)backgrounds[currentBackground].height
    };
    
    Vector2 origin = { 0, 0 };
    
    // Draw current level sliding up
    if (transitionProgress < 1.0f) {
        float easedProgress = 1.0f - powf(1.0f - transitionProgress, 3);
        currentLevelY = levelBounds.y - (levelBounds.height * easedProgress);
        
        Rectangle currentDestRec = {
            levelBounds.x, currentLevelY,
            levelBounds.width, levelBounds.height
        };
        
        DrawTexturePro(backgrounds[currentBackground], sourceRec, currentDestRec, origin, 0.0f, WHITE);
    }
    
    // Draw next level sliding up from bottom
    if (nextLevelBackgrounds[0].id != 0) {
        float easedProgress = 1.0f - powf(1.0f - transitionProgress, 3);
        nextLevelY = levelBounds.y + levelBounds.height - (levelBounds.height * easedProgress);
        
        Rectangle nextSourceRec = {
            0, 0,
            (float)nextLevelBackgrounds[currentBackground].width,
            (float)nextLevelBackgrounds[currentBackground].height
        };
        
        Rectangle nextDestRec = {
            levelBounds.x, nextLevelY,
            levelBounds.width, levelBounds.height
        };
        
        DrawTexturePro(nextLevelBackgrounds[currentBackground], nextSourceRec, nextDestRec, origin, 0.0f, WHITE);
    }
}

int Level::GetNextLevel() const {
    return nextLevel;
}

void Level::SetPlayerTransition(Vector2 startPos, Vector2 endPos) {
    playerStartPos = startPos;
    playerEndPos = endPos;
    shouldAnimatePlayer = true;
}

Vector2 Level::GetPlayerTransitionPosition() const {
    if (!shouldAnimatePlayer || !isSwipeTransition) {
        return playerStartPos;
    }
    
    // Use smoother easing for player movement (ease-in-out)
    float t = transitionProgress;
    float easedT = t * t * (3.0f - 2.0f * t); // Smoothstep interpolation
    
    return {
        playerStartPos.x + (playerEndPos.x - playerStartPos.x) * easedT,
        playerStartPos.y + (playerEndPos.y - playerStartPos.y) * easedT
    };
}

bool Level::ShouldAnimatePlayer() const {
    return shouldAnimatePlayer && isSwipeTransition;
}

bool Level::IsObstaclePenetrable(int index) const {
    if (index >= 0 && index < 50) {
        return penetrableObstacles[index];
    }
    return false;
}

// Bridge gap management for level 10
void Level::RemoveBridgeGapObstacle() {
    if (currentLevel != 10) return;
    
    // The bridge gap obstacle is at index 9 (after 8 boundary walls + 1 left pillar)
    // Make it have zero size to effectively remove it
    obstacles[9] = {0, 0, 0, 0};
}

bool Level::HasBridgeGapObstacle() const {
    if (currentLevel != 10) return false;
    
    // Check if the bridge gap obstacle (index 9) has non-zero size
    return (obstacles[9].width > 0 && obstacles[9].height > 0);
}
