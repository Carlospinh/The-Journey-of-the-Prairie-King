#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Common.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Coin.h"
#include "PowerUp.h"
#include "DeathAnimation.h"
#include "Level.h"
#include "UI.h"

// Maximum number of game objects
#define MAX_ENEMIES 20
#define MAX_BULLETS 100
#define MAX_COINS 50
#define MAX_POWERUPS 10
#define MAX_DEATH_ANIMATIONS 10

class GameManager {
private:
    // Game state
    GameState currentState;
    bool isMusicPlaying;
    
    // Game time
    float timeRemaining;
    float timeElapsed;
    
    // Game objects
    Player player;
    Bullet bullets[MAX_BULLETS];
    Enemy enemies[MAX_ENEMIES];
    Coin coins[MAX_COINS];
    PowerUp powerUps[MAX_POWERUPS];
    DeathAnimation deathAnimations[MAX_DEATH_ANIMATIONS];
    Level level;
    UI ui;
    
    // Game statistics
    int enemiesKilled;
    int coinsCollected;
    
    // PowerUp state - Wheel
    bool wheelPowerUpOnField;
    float wheelDropCooldown;
    
    // PowerUp state - Shotgun
    bool shotgunPowerUpOnField;
    float shotgunDropCooldown;
    
    // PowerUp state - Coffee
    bool coffeePowerUpOnField;
    float coffeeDropCooldown;
    
    // PowerUp state - Nuke
    bool nukePowerUpOnField;
    float nukeDropCooldown;
    
    // PowerUp drop timing
    float powerUpDropTimer;
    float powerUpDropInterval;
    
    // Enemy spawning
    float enemySpawnTimer;
    float enemySpawnInterval;
    
    // Audio
    Sound startSound;
    Music backgroundMusic;
    
    // Helper methods
    void UpdatePlaying(float deltaTime);
    void UpdateGameOver(float deltaTime);
    void UpdateLevelCompleted(float deltaTime);
    void UpdateMenu(float deltaTime);
    void UpdateIntro(float deltaTime);
    
    void DrawPlaying();
    
    void SpawnEnemy();
    void SpawnCoin(Vector2 position);
    void SpawnPowerUp(Vector2 position);
    void SpawnSpecificPowerUp(Vector2 position, PowerUpType type);
    void CreateDeathAnimation(Vector2 position);
    
    bool CheckCollisions();
    void HandleBulletCollisions();
    void HandleNukeEffect(); // Eliminate all enemies when nuke is activated
    
public:
    GameManager();
    ~GameManager();
    
    // Initialize the game
    void InitGame();
    
    // Reset game state
    void ResetGame();
    
    // Close and cleanup
    void CloseGame();
    
    // Main game loop methods
    void Update();
    void Draw();
    
    // Check if game window should close
    bool ShouldClose() const;
};

#endif // GAME_MANAGER_H
