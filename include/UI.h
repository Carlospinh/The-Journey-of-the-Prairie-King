#ifndef UI_H
#define UI_H

#include "Common.h"

class UI {
private:
    // Logo & menus
    Texture2D menuLogo;
    Rectangle startButton;
    
    // Intro screens
    Texture2D introScreen1;
    Texture2D introScreen2;
    Texture2D introScreen3;
    float fadeAlpha;
    bool fadingIn;
    bool fadingOut;
    float introTimer;
    float introDuration;
    
    // HUD textures
    Texture2D liveTexture;
    Texture2D coinTexture;
    Texture2D powerUpCartelTexture;
    Texture2D clockTexture;
    
    // Game state
    GameState currentGameState;
    
public:
    UI();
    ~UI();
    
    // Initialize UI components
    void LoadResources();
    void UnloadResources();
    
    // Update UI state
    void Update(float deltaTime);
    
    // Draw different screens based on game state
    void DrawIntroScreen1();
    void DrawIntroScreen2();
    void DrawIntroScreen3();
    void DrawMenu();
    void DrawGameOver();
    void DrawLevelCompleted();
    
    // Draw HUD elements during gameplay
    void DrawHUD(int playerLives, int coinsCollected, int enemiesKilled, float timeRemaining, bool hasWheelPowerUp, bool wheelPowerUpActive, float wheelPowerUpTimer, int currentLevel);
    
    // Get current game state
    GameState GetGameState() const;
    
    // Set game state
    void SetGameState(GameState state);
    
    // Check if mouse is over button
    bool IsMouseOverStart() const;
};

#endif // UI_H
