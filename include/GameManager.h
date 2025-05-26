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
#include "Shop.h"
#include "Beaver.h"
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
    
    // Boss health bar variables
    bool showBossHealthBar;
    int bossMaxHealth;
    int bossCurrentHealth;
    Rectangle bossHealthBarBounds;
    
    // Game objects
    Player player;
    Bullet bullets[MAX_BULLETS];
    Enemy enemies[MAX_ENEMIES];
    Coin coins[MAX_COINS];
    PowerUp powerUps[MAX_POWERUPS];
    DeathAnimation deathAnimations[MAX_DEATH_ANIMATIONS];
    Level level;
    UI ui;
    Shop shop; // Added Shop object
    Texture2D trophyTexture;
    Vector2 trophyPosition;
    float trophyLiftOffset;
    bool trophyLiftAnimation;
    
    // Boss defeat sequence variables
    bool bossDefeated;
    bool showWhiteScreen;
    bool showCrackEffect;
    bool playingLightning;
    bool showBridge;
    bool trophySpawned;
    float bossDefeatTimer;
    float whiteScreenDuration;
    float crackEffectTimer;
    float lightningTimer;
    float bridgeTimer;
    int lightningFrame;
    int maxLightningFrames;
    Vector2 bridgePosition;
    Vector2 bossDeathPosition;  // Store where the boss died
    
    // Boss introduction sequence variables
    bool showBossIntro;
    bool bossIntroComplete;
    float bossIntroTimer;
    float bossIntroDisplayDuration;
    Texture2D bossTextTexture;
    Vector2 bossTextPosition;
    
    // Bridge texture for boss defeat sequence
    Texture2D bridgeTexture;

    // Trophy management
    bool trophyOnField;

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
    
    // PowerUp state - Life
    bool lifePowerUpOnField;
    float lifeDropCooldown;
    
    // PowerUp drop timing
    float powerUpDropTimer;
    float powerUpDropInterval;
    
    // Enemy spawning
    float enemySpawnTimer;
    float enemySpawnInterval;
    
    // Shop state
    bool shopActivated;
    bool shopCompletedThisLevel;
    
    // Audio
    Sound startSound;
    Sound enemyHitSound;  // Add enemy hit sound
    Music backgroundMusic;
    Music overworldMusic;  // Add second music for levels 5-9
    Music bossMusic;
    static const int MAX_BEAVERS = 3;
Beaver beavers[MAX_BEAVERS];
float beaverSpawnTimer;
float beaverSpawnInterval;

// Add these method declarations in the private section:
void UpdateBeavers(float deltaTime);
void SpawnBeaver();
std::vector<Vector2> GenerateBeaverPath() const;

// Player exit animation
    bool playerExiting;
    float playerExitTimer;
    Vector2 playerExitTarget;

    // Helper methods
    void UpdatePlaying(float deltaTime);
    void UpdateGameOver(float deltaTime);
    void UpdateLevelCompleted(float deltaTime);
    void UpdateMenu(float deltaTime);
    void UpdateIntro(float deltaTime);
        
    void SwitchMusicForLevel(int levelNumber);  // Add this line
    
    void DrawPlaying();
    
    void SpawnEnemy();
    void SpawnCoin(Vector2 position);
    void SpawnTrophy(Vector2 position);  // Add trophy spawning method
    void SpawnPowerUp(Vector2 position);
    void SpawnSpecificPowerUp(Vector2 position, PowerUpType type);
    void CreateDeathAnimation(Vector2 position);
    
    bool CheckCollisions();
    void HandleBulletCollisions();
    void HandleNukeEffect(); // Eliminate all enemies when nuke is activated
    void UpdateEnemyBeaverDistraction(); // Make enemies chase beavers when active
    
    // Boss defeat sequence methods
    void HandleBossDefeat(Vector2 bossPosition);
    void UpdateBossDefeatSequence(float deltaTime);
    void DrawBossDefeatSequence();  // Add this missing declaration
    void PlayWhiteScreenWithCrack();
    void PlayLightningAnimation();
    void DisplayBridge();
    void UpdateTrophy(float deltaTime);
    void DrawBossDefeatEffects();
    
    // Boss introduction sequence methods
    void StartBossIntroduction();
    void UpdateBossIntroduction(float deltaTime);
    void DrawBossIntroduction();
    
    // Boss health bar methods
    void InitBossHealthBar();
    void UpdateBossHealthBar();
    void DrawBossHealthBar();
    
    // Bridge gap management for level 10
    void RemoveBridgeGapObstacle();
    bool HasBridgeGapObstacle() const;
    
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
