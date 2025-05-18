#include "../include/GameManager.h"
#include <stdio.h>

GameManager::GameManager() 
    : currentState(INTRO_1),
    isMusicPlaying(false),
    timeRemaining(60.0f),
    timeElapsed(0.0f),
    enemiesKilled(0),
    coinsCollected(0),
    wheelPowerUpOnField(false),
    wheelDropCooldown(0.0f),
    enemySpawnTimer(0.0f),
    enemySpawnInterval(2.0f) {
}

GameManager::~GameManager() {
    CloseGame();
}

void GameManager::InitGame() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Journey of the Prairie King");
    ToggleFullscreen();
    SetTargetFPS(60);
    
    // Initialize audio
    InitAudioDevice();
    SetMasterVolume(1.0f);
    
    srand((unsigned int)time(NULL));
    
    // Load sounds
    startSound = LoadSound("resources/Sounds/start.wav");
    SetSoundVolume(startSound, 1.0f);
    
    backgroundMusic = LoadMusicStream("resources/BKMusic/TheOutlaw.mp3");
    backgroundMusic.looping = true;
    
    // Load shared resources
    Bullet::LoadSharedTexture();
    Enemy::LoadSharedTextures();
    Coin::LoadSharedResources();
    PowerUp::LoadSharedResources();
    
    // Initialize UI
    ui.LoadResources();
    
    // Initialize player
    player.LoadTextures();
    
    // Initialize level
    level.LoadResources(1);
    
    // Reset game state
    ResetGame();
}

void GameManager::ResetGame() {
    player.SetPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
    player.SetLives(3);
    
    timeRemaining = 60.0f; // Changed from 60.0f to 3.0f for faster testing
    timeElapsed = 0.0f;
    enemiesKilled = 0;
    coinsCollected = 0;
    
    player.SetWheelPowerUp(false);
    wheelPowerUpOnField = false;
    wheelDropCooldown = 0.0f;
    
    // Reset level
    level.LoadResources(1);
    
    // Reset active objects
    for (int i = 0; i < MAX_ENEMIES; i++) enemies[i].SetActive(false);
    for (int i = 0; i < MAX_BULLETS; i++) bullets[i].SetActive(false);
    for (int i = 0; i < MAX_COINS; i++) coins[i].SetActive(false);
    for (int i = 0; i < MAX_POWERUPS; i++) powerUps[i].SetActive(false);
    for (int i = 0; i < MAX_DEATH_ANIMATIONS; i++) deathAnimations[i].SetActive(false);
    
    // Reset music
    StopMusicStream(backgroundMusic);
    PlayMusicStream(backgroundMusic);
    SetMusicVolume(backgroundMusic, 0.5f);
    isMusicPlaying = true;
    
    // Player starts with "Hi" animation
    player.PlayHiAnimation();
}

void GameManager::CloseGame() {
    StopMusicStream(backgroundMusic);
    UnloadMusicStream(backgroundMusic);
    
    UnloadSound(startSound);
    
    // Unload shared resources
    Bullet::UnloadSharedTexture();
    Enemy::UnloadSharedTextures();
    Coin::UnloadSharedResources();
    PowerUp::UnloadSharedResources();
    
    ui.UnloadResources();
    
    CloseAudioDevice();
    CloseWindow();
}

void GameManager::Update() {
    float deltaTime = GetFrameTime();
    
    // Update current state from UI (handle state transitions)
    currentState = ui.GetGameState();
    
    switch (currentState) {
        case INTRO_1:
        case INTRO_2:
        case INTRO_3:
            UpdateIntro(deltaTime);
            break;
            
        case MENU:
            UpdateMenu(deltaTime);
            break;
            
        case PLAYING:
            UpdatePlaying(deltaTime);
            break;
            
        case GAME_OVER:
            UpdateGameOver(deltaTime);
            break;
            
        case LEVEL_COMPLETED:
            UpdateLevelCompleted(deltaTime);
            break;
    }
}

void GameManager::UpdateIntro(float deltaTime) {
    ui.Update(deltaTime);
}

void GameManager::UpdateMenu(float deltaTime) {
    if (IsKeyPressed(KEY_ENTER)) {
        ui.SetGameState(PLAYING);
        PlayMusicStream(backgroundMusic);
        SetMusicVolume(backgroundMusic, 0.5f);
        isMusicPlaying = true;
    }
}

void GameManager::UpdatePlaying(float deltaTime) {
    if (isMusicPlaying) {
        UpdateMusicStream(backgroundMusic);
    }
    
    // Check for game over condition
    if (player.GetLives() <= 0) {
        ui.SetGameState(GAME_OVER);
        return;
    }
    
    // Update timer
    timeElapsed += deltaTime;
    if (timeRemaining > 0) {
        timeRemaining -= deltaTime;
    }
    else {
        timeRemaining = 0;
        
        // Check if this is level 9 and time is up (game completed)
        if (level.GetCurrentLevel() == 9) {
            ui.SetGameState(LEVEL_COMPLETED);
            return;
        }
        
        // If not already showing completed level
        if (!level.IsCompleted()) {
            level.SetCompleted();
        }
        
        // Check for level transition
        Rectangle playerRect = player.GetCollisionRect();
        if (level.IsPlayerInExitZone(playerRect)) {
            level.StartTransition();
        }
    }
    
    // Level transition logic
    if (level.IsTransitioning()) {
        level.Update(deltaTime);
        
        if (level.IsTransitionComplete()) {
            // Store the current level number before resetting
            int currentLevel = level.GetCurrentLevel();
            
            // Reset level transition state (important!)
            level = Level(); // Create a new clean level instance
            
            // Load the next level
            int nextLevel = currentLevel + 1;
            if (nextLevel > 9) nextLevel = 1; // Wrap around to level 1 (shouldn't happen normally)
            
            level.LoadResources(nextLevel); // Load next level
            timeRemaining = 60.0f;
            
            // Reset objects
            for (int i = 0; i < MAX_ENEMIES; i++) enemies[i].SetActive(false);
            for (int i = 0; i < MAX_BULLETS; i++) bullets[i].SetActive(false);
            for (int i = 0; i < MAX_COINS; i++) coins[i].SetActive(false);
            for (int i = 0; i < MAX_POWERUPS; i++) powerUps[i].SetActive(false);
            
            player.SetPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
            return;  // Skip the rest of the update for this frame
        }
    }
    
    // Update level
    level.Update(deltaTime);
    
    // Update player
    player.Update(deltaTime);
    
    // Handle player power-up activation
    if (player.HasWheelPowerUp() && IsKeyPressed(KEY_SPACE) && !player.IsWheelPowerUpActive() && !level.IsTransitioning()) {
        player.ActivateWheelPowerUp();
    }
    
    // Update player power-ups
    if (player.IsWheelPowerUpActive()) {
        int bulletCount = 0;
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].IsActive()) bulletCount++;
        }
        player.UpdateWheelPowerUp(deltaTime, bullets, bulletCount);
    }
    
    // Spawn enemies
    if (timeRemaining > 0 && !level.IsTransitioning()) {
        enemySpawnTimer += deltaTime;
        if (enemySpawnTimer >= enemySpawnInterval) {
            enemySpawnTimer = 0.0f;
            SpawnEnemy();
        }
    }
    
    // Only update game objects when not transitioning
    if (!level.IsTransitioning()) {
        // Update game objects
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].IsActive()) {
                enemies[i].Update(deltaTime);
                
                // Move enemies toward player
                enemies[i].MoveToward(player.GetPosition(), deltaTime);
                
                // Check enemy-obstacle collisions
                for (int j = 0; j < level.GetObstacleCount(); j++) {
                    enemies[i].HandleObstacleCollision(level.GetObstacle(j));
                }
                
                // Check if enemy went off-screen
                Rectangle bounds = level.GetLevelBounds();
                if (enemies[i].IsOutOfBounds(bounds.x, bounds.y, bounds.width, bounds.height)) {
                    enemies[i].SetActive(false);
                }
            }
        }
        
        for (int i = 0; i < MAX_COINS; i++) {
            if (coins[i].IsActive()) {
                coins[i].Update(deltaTime);
                
                // Check for player-coin collision
                if (CheckCollisionRecs(player.GetCollisionRect(), coins[i].GetCollisionRect())) {
                    coins[i].SetActive(false);
                    coinsCollected++;
                    Coin::PlayPickupSound();
                }
                
                // Check if coin went off-screen
                Rectangle bounds = level.GetLevelBounds();
                if (coins[i].IsOutOfBounds(bounds.x, bounds.y, bounds.width, bounds.height)) {
                    coins[i].SetActive(false);
                }
            }
        }
        
        for (int i = 0; i < MAX_POWERUPS; i++) {
            if (powerUps[i].IsActive()) {
                powerUps[i].Update(deltaTime);
                
                // Check for player-powerup collision
                if (CheckCollisionRecs(player.GetCollisionRect(), powerUps[i].GetCollisionRect())) {
                    powerUps[i].SetActive(false);
                    player.SetWheelPowerUp(true);
                    wheelPowerUpOnField = false;
                    PowerUp::PlayPickupSound();
                }
                
                // Check if powerup went off-screen
                Rectangle bounds = level.GetLevelBounds();
                if (powerUps[i].IsOutOfBounds(bounds.x, bounds.y, bounds.width, bounds.height)) {
                    powerUps[i].SetActive(false);
                    wheelPowerUpOnField = false;
                }
            }
        }
        
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].IsActive()) {
                bullets[i].Update(deltaTime);
                
                // Check if bullet went off-screen
                Rectangle bounds = level.GetLevelBounds();
                Rectangle bulletRect = bullets[i].GetCollisionRect();
                if (bulletRect.x < bounds.x || 
                    bulletRect.x > bounds.x + bounds.width ||
                    bulletRect.y < bounds.y || 
                    bulletRect.y > bounds.y + bounds.height) {
                    bullets[i].SetActive(false);
                }
            }
        }
        
        // Update death animations
        for (int i = 0; i < MAX_DEATH_ANIMATIONS; i++) {
            if (deathAnimations[i].IsActive()) {
                deathAnimations[i].Update(deltaTime);
            }
        }
        
        // Check for collisions
        // Player-enemy collisions
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].IsActive()) {
                if (CheckCollisionRecs(player.GetCollisionRect(), enemies[i].GetCollisionRect())) {
                    enemies[i].SetActive(false);
                    player.DecreaseLives();
                    
                    if (player.GetLives() <= 0) {
                        ui.SetGameState(GAME_OVER);
                        return;
                    }
                }
            }
        }
        
        // Handle bullet-enemy collisions
        HandleBulletCollisions();
        
        // Handle player obstacle collisions
        for (int i = 0; i < level.GetObstacleCount(); i++) {
            player.HandleObstacleCollision(level.GetObstacle(i));
        }
        
        // Handle player movement
        Rectangle bounds = level.GetLevelBounds();
        bool moving = false;
        
        if (IsKeyDown(KEY_W)) {
            player.MoveUp(deltaTime, bounds.y);
            moving = true;
        }
        
        if (IsKeyDown(KEY_S)) {
            player.MoveDown(deltaTime, bounds.y, bounds.height);
            moving = true;
        }
        
        if (IsKeyDown(KEY_A)) {
            player.MoveLeft(deltaTime, bounds.x);
            moving = true;
        }
        
        if (IsKeyDown(KEY_D)) {
            player.MoveRight(deltaTime, bounds.x, bounds.width);
            moving = true;
        }
        
        player.UpdateAnimation(deltaTime, moving);
        
        // Handle player shooting
        int bulletCount = 0;
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].IsActive()) bulletCount++;
        }
        player.Shoot(bullets, bulletCount, deltaTime);
    }
}

void GameManager::UpdateGameOver(float deltaTime) {
    if (IsKeyPressed(KEY_SPACE)) {
        ResetGame();
        ui.SetGameState(MENU);
    }
}

void GameManager::UpdateLevelCompleted(float deltaTime) {
    if (IsKeyPressed(KEY_SPACE)) {
        ResetGame();
        ui.SetGameState(MENU);
    }
}

void GameManager::SpawnEnemy() {
    Rectangle bounds = level.GetLevelBounds();
    float bgX = bounds.x;
    float bgY = bounds.y;
    float bgWidth = bounds.width;
    float bgHeight = bounds.height;
    
    Vector2 spawnPositions[] = {
        { bgX + 15, bgY + bgHeight * 0.4f + (float)(rand() % (int)(bgHeight * 0.2f)) },
        { bgX + bgWidth - 15, bgY + bgHeight * 0.4f + (float)(rand() % (int)(bgHeight * 0.2f)) },
        { bgX + bgWidth * 0.4f + (float)(rand() % (int)(bgWidth * 0.2f)), bgY + 15 },
        { bgX + bgWidth * 0.4f + (float)(rand() % (int)(bgWidth * 0.2f)), bgY + bgHeight - 15 }
    };
    
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].IsActive()) {
            int spawnIndex = rand() % 4;
            
            enemies[i].Init(spawnPositions[spawnIndex], 0.2f, 100.0f + (rand() % 50));
            break;
        }
    }
}

void GameManager::SpawnCoin(Vector2 position) {
    for (int i = 0; i < MAX_COINS; i++) {
        if (!coins[i].IsActive()) {
            coins[i].Init(position);
            break;
        }
    }
}

void GameManager::SpawnPowerUp(Vector2 position) {
    // Only one wheel power-up can be active at a time
    if (wheelPowerUpOnField || player.HasWheelPowerUp() || player.IsWheelPowerUpActive()) return;
    
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!powerUps[i].IsActive()) {
            powerUps[i].InitWheel(position);
            wheelPowerUpOnField = true;
            break;
        }
    }
}

void GameManager::CreateDeathAnimation(Vector2 position) {
    for (int i = 0; i < MAX_DEATH_ANIMATIONS; i++) {
        if (!deathAnimations[i].IsActive()) {
            deathAnimations[i].Init(position);
            break;
        }
    }
}

void GameManager::HandleBulletCollisions() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].IsActive()) {
            for (int j = 0; j < MAX_ENEMIES; j++) {
                if (enemies[j].IsActive() && 
                    CheckCollisionRecs(bullets[i].GetCollisionRect(), enemies[j].GetCollisionRect())) {
                    
                    bullets[i].SetActive(false);
                    enemies[j].SetActive(false);
                    enemiesKilled++;
                    
                    // Create death animation
                    CreateDeathAnimation(enemies[j].GetPosition());
                    
                    // Chance to drop power-up or coin
                    int dropChance = rand() % 100;
                    
                    if (dropChance < 25 && 
                        !wheelPowerUpOnField && 
                        !player.HasWheelPowerUp() && 
                        !player.IsWheelPowerUpActive()) {
                        SpawnPowerUp(enemies[j].GetPosition());
                    }
                    else if (dropChance < 100) {
                        SpawnCoin(enemies[j].GetPosition());
                    }
                    
                    break;
                }
            }
        }
    }
}

void GameManager::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);
    
    switch (currentState) {
        case INTRO_1:
            ui.DrawIntroScreen1();
            break;
            
        case INTRO_2:
            ui.DrawIntroScreen2();
            break;
            
        case INTRO_3:
            ui.DrawIntroScreen3();
            break;
            
        case MENU:
            ui.DrawMenu();
            break;
            
        case PLAYING:
            DrawPlaying();
            break;
            
        case GAME_OVER:
            ui.DrawGameOver();
            break;
            
        case LEVEL_COMPLETED:
            ui.DrawLevelCompleted();
            break;
    }
    
    EndDrawing();
}

void GameManager::DrawPlaying() {
    if (isMusicPlaying) {
        UpdateMusicStream(backgroundMusic);
        if (IsMusicStreamPlaying(backgroundMusic) == false) {
            PlayMusicStream(backgroundMusic);
        }
    }
    
    // Draw level
    level.Draw();
    
    // Draw death animations
    for (int i = 0; i < MAX_DEATH_ANIMATIONS; i++) {
        if (deathAnimations[i].IsActive()) {
            deathAnimations[i].Draw();
        }
    }
    
    // Draw collectibles and enemies
    for (int i = 0; i < MAX_COINS; i++) {
        if (coins[i].IsActive()) {
            coins[i].Draw();
        }
    }
    
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (powerUps[i].IsActive()) {
            powerUps[i].Draw();
        }
    }
    
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].IsActive()) {
            enemies[i].Draw();
        }
    }
    
    // Draw bullets
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].IsActive()) {
            bullets[i].Draw();
        }
    }
    
    // Draw player
    player.Draw();
    
    // Draw level exit arrow
    if (level.ShouldShowArrow()) {
        level.DrawArrow();
    }
    
    // Draw UI elements
    ui.DrawHUD(
        player.GetLives(),
        coinsCollected,
        enemiesKilled,
        timeRemaining,
        player.HasWheelPowerUp(),
        player.IsWheelPowerUpActive(),
        player.GetWheelPowerUpTimer(),  // Pass the actual timer value
        level.GetCurrentLevel()         // Pass the current level number
    );
}

bool GameManager::ShouldClose() const {
    return WindowShouldClose();
}
