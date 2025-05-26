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
      shotgunPowerUpOnField(false),
      shotgunDropCooldown(0.0f),
      coffeePowerUpOnField(false),
      coffeeDropCooldown(0.0f),
      nukePowerUpOnField(false),
      nukeDropCooldown(0.0f),
      lifePowerUpOnField(false),
      lifeDropCooldown(0.0f),
      powerUpDropTimer(0.0f),
      powerUpDropInterval(10.0f),
      enemySpawnTimer(0.0f),
      enemySpawnInterval(2.0f),
      shopActivated(false),
      shopCompletedThisLevel(false),
      beaverSpawnTimer(0.0f),
      beaverSpawnInterval(10.0f),
      // Boss defeat sequence initialization
      bossDefeated(false),
      showWhiteScreen(false),
      showCrackEffect(false),
      playingLightning(false),
      showBridge(false),
      trophySpawned(false),
      bossDefeatTimer(0.0f),
      whiteScreenDuration(2.0f),
      crackEffectTimer(0.0f),
      lightningTimer(0.0f),
      bridgeTimer(0.0f),
      lightningFrame(0),
      maxLightningFrames(8),
      bridgePosition({0, 0}),
      bossDeathPosition({0, 0}),  // Initialize boss death position
      // Player exit animation initialization
      playerExiting(false),
      playerExitTimer(0.0f),
      playerExitTarget({0, 0})
{
}

GameManager::~GameManager()
{
    CloseGame();
}

void GameManager::InitGame()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Journey of the Prairie King");
    ToggleFullscreen();
    SetTargetFPS(60);

    InitAudioDevice();
    SetMasterVolume(0.7f);

    srand((unsigned int)time(NULL));

    startSound = LoadSound("resources/Sounds/start.wav");
    SetSoundVolume(startSound, 1.0f);

    // Load enemy hit sound
    enemyHitSound = LoadSound("resources/Audio/FX/Enemy Hit Sound.mp3");
    SetSoundVolume(enemyHitSound, 0.8f);

    backgroundMusic = LoadMusicStream("resources/BKMusic/TheOutlaw.mp3");
    backgroundMusic.looping = true;
    trophyTexture = LoadTexture("resources/Boss Battle/Trophy/Trophy.png");
    
    // Load bridge texture for boss defeat sequence
    bridgeTexture = LoadTexture("resources/Boss Battle/Puente/Puente.png");
    
    Bullet::LoadSharedTexture();
    Enemy::LoadSharedTextures();
    Coin::LoadSharedResources();
    PowerUp::LoadSharedResources();

    ui.LoadResources();
    shop.LoadResources();

    player.LoadTextures();

    level.LoadResources(1);
    Beaver::LoadSharedResources();

    // Ensure UI state is properly set to INTRO_1
    ui.SetGameState(INTRO_1);
    currentState = INTRO_1;

    ResetGame();
}

void GameManager::ResetGame()
{
    player.SetPosition({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
    player.SetPosition({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 });
    player.SetLives(3);

    timeRemaining = 60.0f;
    timeElapsed = 0.0f;
    enemiesKilled = 0;
    coinsCollected = 0;

    player.SetWheelPowerUp(false);
    player.SetShotgunPowerUp(false);
    player.SetCoffeePowerUp(false);
    player.SetNukePowerUp(false);

    wheelPowerUpOnField = false;
    shotgunPowerUpOnField = false;
    coffeePowerUpOnField = false;
    nukePowerUpOnField = false;
    lifePowerUpOnField = false;

    wheelDropCooldown = 0.0f;
    shotgunDropCooldown = 0.0f;
    coffeeDropCooldown = 0.0f;
    nukeDropCooldown = 0.0f;
    lifeDropCooldown = 0.0f;

    powerUpDropTimer = 0.0f;

    shopActivated = false;
    trophyOnField = false;
    level.LoadResources(1); // Changed from 1 to 5 for testing level 5
    for (int i = 0; i < MAX_BEAVERS; i++)
        beavers[i].Deactivate();
    beaverSpawnTimer = 0.0f;

    for (int i = 0; i < MAX_ENEMIES; i++)
        enemies[i].SetActive(false);
    for (int i = 0; i < MAX_BULLETS; i++)
        bullets[i].SetActive(false);
    for (int i = 0; i < MAX_COINS; i++)
        coins[i].SetActive(false);
    for (int i = 0; i < MAX_POWERUPS; i++)
        powerUps[i].SetActive(false);
    for (int i = 0; i < MAX_DEATH_ANIMATIONS; i++)
        deathAnimations[i].SetActive(false);

    StopMusicStream(backgroundMusic);
    PlayMusicStream(backgroundMusic);
    SetMusicVolume(backgroundMusic, 0.5f);
    isMusicPlaying = true;

    player.PlayHiAnimation();
}

void GameManager::CloseGame()
{
    StopMusicStream(backgroundMusic);
    UnloadMusicStream(backgroundMusic);

    UnloadSound(startSound);
    UnloadSound(enemyHitSound); // Add enemy hit sound cleanup

    Bullet::UnloadSharedTexture();
    Enemy::UnloadSharedTextures();
    Coin::UnloadSharedResources();
    PowerUp::UnloadSharedResources();

    UnloadTexture(trophyTexture);
    UnloadTexture(bridgeTexture); // Unload bridge texture
    Beaver::UnloadSharedResources();
    ui.UnloadResources();

    CloseAudioDevice();
    CloseWindow();
}

void GameManager::Update()
{
    float deltaTime = GetFrameTime();

    currentState = ui.GetGameState();

    switch (currentState)
    {
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

void GameManager::UpdateIntro(float deltaTime)
{
    ui.Update(deltaTime);
}

void GameManager::UpdateMenu(float deltaTime)
{
    if (IsKeyPressed(KEY_ENTER))
    {
        ui.SetGameState(PLAYING);
        PlayMusicStream(backgroundMusic);
        SetMusicVolume(backgroundMusic, 0.5f);
        isMusicPlaying = true;
    }
}

std::vector<Vector2> GameManager::GenerateBeaverPath() const
{
    Rectangle bounds = level.GetLevelBounds();
    std::vector<Vector2> path;

    // Randomly choose horizontal (0) or vertical (1) direction
    int direction = GetRandomValue(0, 1);

    if (direction == 0)
    {
        // Horizontal path (left to right or right to left)
        float y = bounds.y + GetRandomValue(50, (int)bounds.height - 50);

        if (GetRandomValue(0, 1) == 0)
        {
            // Left to right
            Vector2 start = { bounds.x - 30, y };
            Vector2 end = { bounds.x + bounds.width + 30, y };
            path.push_back(start);
            path.push_back(end);
        }
        else
        {
            // Right to left
            Vector2 start = { bounds.x + bounds.width + 30, y };
            Vector2 end = { bounds.x - 30, y };
            path.push_back(start);
            path.push_back(end);
        }
    }
    else
    {
        // Vertical path (top to bottom or bottom to top)
        float x = bounds.x + GetRandomValue(50, (int)bounds.width - 50);

        if (GetRandomValue(0, 1) == 0)
        {
            // Top to bottom
            Vector2 start = { x, bounds.y - 30 };
            Vector2 end = { x, bounds.y + bounds.height + 30 };
            path.push_back(start);
            path.push_back(end);
        }
        else
        {
            // Bottom to top
            Vector2 start = { x, bounds.y + bounds.height + 30 };
            Vector2 end = { x, bounds.y - 30 };
            path.push_back(start);
            path.push_back(end);
        }
    }

    return path;
}

void GameManager::SpawnBeaver()
{
    for (int i = 0; i < MAX_BEAVERS; i++)
    {
        if (beavers[i].IsActive())
        {
            return;
        }
    }

    for (int i = 0; i < MAX_BEAVERS; i++)
    {
        if (!beavers[i].IsActive())
        {
            std::vector<Vector2> path = GenerateBeaverPath();
            beavers[i].Activate(path);
            break;
        }
    }
}

void GameManager::UpdateBeavers(float deltaTime)
{
    for (int i = 0; i < MAX_BEAVERS; i++)
    {
        if (beavers[i].IsActive())
        {
            beavers[i].Update(deltaTime);
        }
    }
}

void GameManager::UpdatePlaying(float deltaTime)
{
    if (isMusicPlaying)
    {
        UpdateMusicStream(backgroundMusic);
    }

    if (player.GetLives() <= 0)
    {
        ui.SetGameState(GAME_OVER);
        return;
    }

    // Handle player exit animation after trophy pickup
    if (playerExiting) {
        playerExitTimer += deltaTime;

        Vector2 currentPos = player.GetPosition();
        Vector2 direction = {
            playerExitTarget.x - currentPos.x,
            playerExitTarget.y - currentPos.y
        };

        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 5.0f) {
            direction.x /= length;
            direction.y /= length;

            float exitSpeed = 150.0f;
            Vector2 newPos = {
                currentPos.x + direction.x * exitSpeed * deltaTime,
                currentPos.y + direction.y * exitSpeed * deltaTime
            };

            player.SetPosition(newPos);
            player.UpdateAnimation(deltaTime, true);
        }
        else {
            playerExiting = false;
            ui.SetGameState(LEVEL_COMPLETED);
            return;
        }
        return; // No m�s l�gica durante la animaci�n de salida
    }

    timeElapsed += deltaTime;

    if (timeRemaining > 0)
    {
        timeRemaining -= deltaTime;
    }
    else
    {
        timeRemaining = 0;

        // --- Activa la tienda solo en niveles 2,4,6,8 y s�lo si no est� activa ni activada antes ---
        if ((level.GetCurrentLevel() == 2 || level.GetCurrentLevel() == 4 ||
            level.GetCurrentLevel() == 6 || level.GetCurrentLevel() == 8) &&
            !shop.IsActive() && !shopActivated)
        {
            shopActivated = true;
            shop.Activate(coinsCollected, &player);
        }
        else if (!level.IsCompleted() && !shop.IsActive() && !shopActivated)
        {
            // Si no hay tienda en este nivel, marcar como completado directamente
            level.SetCompleted();
            shopCompletedThisLevel = true;
        }

        // Para nivel 10 ir directo a LEVEL_COMPLETED sin tienda
        if (level.GetCurrentLevel() == 10)
        {
            ui.SetGameState(LEVEL_COMPLETED);
            return;
        }
    }

    // --- Actualiza la tienda si est� activa ---
    if (shop.IsActive())
    {
        shop.Update(deltaTime);
        shop.CheckItemPickup(player.GetCollisionRect());

        if (!shop.IsActive() && shopActivated)
        {
            coinsCollected = shop.GetRemainingCoins();
            shop.MarkShopCompletedForLevel(level.GetCurrentLevel());
            shopCompletedThisLevel = true;

            if (!level.IsCompleted())
            {
                level.SetCompleted();
            }
        }
    }

    // --- Solo si el nivel est� completado y no hay transici�n ni tienda activa,
    //      verifica si el jugador est� en zona de salida para iniciar transici�n ---
    if (level.IsCompleted() && !level.IsTransitioning() &&
        !shop.IsActive() && shopCompletedThisLevel)
    {
        Rectangle playerRect = player.GetCollisionRect();
        if (level.IsPlayerInExitZone(playerRect))
        {
            // Limpia elementos antes de la transici�n
            for (int i = 0; i < MAX_ENEMIES; i++)
                enemies[i].SetActive(false);
            for (int i = 0; i < MAX_DEATH_ANIMATIONS; i++)
                deathAnimations[i].SetActive(false);
            for (int i = 0; i < MAX_COINS; i++)
                coins[i].SetActive(false);
            for (int i = 0; i < MAX_POWERUPS; i++)
                powerUps[i].SetActive(false);

            Vector2 currentPlayerPos = player.GetPosition();
            Rectangle nextLevelBounds = level.GetLevelBounds();
            Vector2 targetPlayerPos = {
                nextLevelBounds.x + nextLevelBounds.width / 2,
                nextLevelBounds.y + nextLevelBounds.height * 0.15f
            };

            level.SetPlayerTransition(currentPlayerPos, targetPlayerPos);
            level.StartSwipeTransition(level.GetCurrentLevel() + 1);
        }
    }

    // --- Actualiza la transici�n si est� activa ---
    if (level.IsTransitioning())
    {
        level.Update(deltaTime);

        if (level.ShouldAnimatePlayer())
        {
            Vector2 interpolatedPos = level.GetPlayerTransitionPosition();
            player.SetPosition(interpolatedPos);
        }

        if (level.IsTransitionComplete())
        {
            int currentLevel = level.GetCurrentLevel();

            level = Level();

            int nextLevel = currentLevel + 1;
            if (nextLevel > 10)
                nextLevel = 1;

            level.LoadResources(nextLevel);
            timeRemaining = 60.0f;
            shopActivated = false;
            shopCompletedThisLevel = false;

            for (int i = 0; i < MAX_ENEMIES; i++)
                enemies[i].SetActive(false);
            for (int i = 0; i < MAX_BULLETS; i++)
                bullets[i].SetActive(false);
            for (int i = 0; i < MAX_COINS; i++)
                coins[i].SetActive(false);
            for (int i = 0; i < MAX_POWERUPS; i++)
                powerUps[i].SetActive(false);

            Rectangle bounds = level.GetLevelBounds();
            float playerX = bounds.x + bounds.width / 2;
            float playerY = bounds.y + bounds.height * 0.15f;
            player.SetPosition({ playerX, playerY });
        }
    }

    level.Update(deltaTime);

    player.Update(deltaTime);

    powerUpDropTimer += deltaTime;
    if (powerUpDropTimer >= powerUpDropInterval)
    {
        powerUpDropTimer = 0;

        if (!level.IsTransitioning() &&
            !wheelPowerUpOnField && !shotgunPowerUpOnField &&
            !coffeePowerUpOnField && !nukePowerUpOnField)
        {

            Rectangle bounds = level.GetLevelBounds();
            float paddingX = bounds.width * 0.15f;
            float paddingY = bounds.height * 0.15f;

            Vector2 spawnPos = {
                bounds.x + paddingX + static_cast<float>(rand()) / (RAND_MAX / (bounds.width - 2 * paddingX)),
                bounds.y + paddingY + static_cast<float>(rand()) / (RAND_MAX / (bounds.height - 2 * paddingY)) };

            int powerUpType = rand() % 5; // Changed from 4 to 5 to include life power-ups
            SpawnSpecificPowerUp(spawnPos, static_cast<PowerUpType>(powerUpType));
        }
    }

    if (!level.IsTransitioning())
    {
        if (player.HasWheelPowerUp() && IsKeyPressed(KEY_SPACE) && !player.IsWheelPowerUpActive())
        {
            player.ActivateWheelPowerUp();
        }

        if (player.HasShotgunPowerUp() && IsKeyPressed(KEY_SPACE) && !player.IsShotgunPowerUpActive())
        {
            player.ActivateShotgunPowerUp();
        }

        if (player.HasCoffeePowerUp() && IsKeyPressed(KEY_SPACE) && !player.IsCoffeePowerUpActive())
        {
            player.ActivateCoffeePowerUp();
        }

        if (player.HasNukePowerUp() && IsKeyPressed(KEY_SPACE))
        {
            player.ActivateNukePowerUp();
            HandleNukeEffect();
        }
    }

    if (player.IsWheelPowerUpActive())
    {
        int bulletCount = 0;
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (bullets[i].IsActive())
                bulletCount++;
        }
        player.UpdateWheelPowerUp(deltaTime, bullets, bulletCount);
    }

    if (timeRemaining > 0 && !level.IsTransitioning())
    {
        enemySpawnTimer += deltaTime;
        if (enemySpawnTimer >= enemySpawnInterval)
        {
            enemySpawnTimer = 0.0f;
            SpawnEnemy();
        }
    }

    if (timeRemaining > 0 && !level.IsTransitioning())
    {
        beaverSpawnTimer += deltaTime;
        if (beaverSpawnTimer >= beaverSpawnInterval)
        {
            beaverSpawnTimer = 0.0f;
            // Set next spawn interval to random time around 20 seconds (18-22 seconds)
            beaverSpawnInterval = GetRandomValue(18, 22);
            SpawnBeaver();
        }

        UpdateBeavers(deltaTime);
    }

    if (!level.IsTransitioning())
    {
        UpdateEnemyBeaverDistraction();

        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (enemies[i].IsActive())
            {
                enemies[i].Update(deltaTime);

                // Boss shooting logic - ADD THIS SECTION
                if (enemies[i].GetType() == ENEMY_BOSS)
                {
                    // Regular boss shooting (single targeted bullet)
                    if (enemies[i].ShouldBossShoot(deltaTime, player.GetPosition()))
                    {
                        // Find an available bullet slot
                        for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++)
                        {
                            if (!bullets[bulletIndex].IsActive())
                            {
                                Vector2 bossCenter = {
                                    enemies[i].GetPosition().x + (enemies[i].GetCollisionRect().width / 2),
                                    enemies[i].GetPosition().y + (enemies[i].GetCollisionRect().height / 2) };
                                Vector2 direction = enemies[i].GetBossShootDirection(player.GetPosition());
                                bullets[bulletIndex].Init(bossCenter, direction, BULLET_BOSS);
                                break;
                            }
                        }
                    }

                    // Rain gun shooting (spread pattern)

                    if (enemies[i].ShouldBossShootRain(deltaTime, player.GetPosition()))
                    {
                        // Find an available bullet slot for rail bullet
                        for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; bulletIndex++)
                        {
                            if (!bullets[bulletIndex].IsActive())
                            {
                                Vector2 bossCenter = {
                                    enemies[i].GetPosition().x + (enemies[i].GetCollisionRect().width / 2),
                                    enemies[i].GetPosition().y + (enemies[i].GetCollisionRect().height / 2) };
                                Vector2 direction = enemies[i].GetBossShootDirection(player.GetPosition());
                                bullets[bulletIndex].Init(bossCenter, direction, BULLET_BOSS_RAIN);
                                break;
                            }
                        }
                    }
                }
                enemies[i].MoveToward(player.GetPosition(), deltaTime);

                for (int j = 0; j < level.GetObstacleCount(); j++)
                {
                    enemies[i].HandleObstacleCollision(level.GetObstacle(j));
                }

                Rectangle bounds = level.GetLevelBounds();
                if (enemies[i].IsOutOfBounds(bounds.x, bounds.y, bounds.width, bounds.height))
                {
                    enemies[i].SetActive(false);
                }
            }
        }

        for (int i = 0; i < MAX_COINS; i++)
        {
            if (coins[i].IsActive())
            {
                coins[i].Update(deltaTime);

                if (CheckCollisionRecs(player.GetCollisionRect(), coins[i].GetCollisionRect())) {
                    if (coins[i].IsTrophy()) {
                        // Trophy picked up - start boss defeat sequence first
                        coins[i].SetActive(false);
                        trophyOnField = false;

                        // Start boss defeat sequence instead of immediately starting player exit
                        HandleBossDefeat(player.GetPosition());

                        return;
                    }
                    else {
                        // Regular coin
                        coins[i].SetActive(false);
                        coinsCollected++;
                        Coin::PlayPickupSound();
                    }
                }

                Rectangle bounds = level.GetLevelBounds();
                if (coins[i].IsOutOfBounds(bounds.x, bounds.y, bounds.width, bounds.height))
                {
                    coins[i].SetActive(false);
                }
            }
        }

        for (int i = 0; i < MAX_POWERUPS; i++)
        {
            if (powerUps[i].IsActive())
            {
                powerUps[i].Update(deltaTime);

                if (CheckCollisionRecs(player.GetCollisionRect(), powerUps[i].GetCollisionRect()))
                {
                    PowerUpType type = powerUps[i].GetType();
                    powerUps[i].SetActive(false);

                    switch (type)
                    {
                    case POWERUP_WHEEL:
                        player.SetWheelPowerUp(true);
                        wheelPowerUpOnField = false;
                        break;
                    case POWERUP_SHOTGUN:
                        player.SetShotgunPowerUp(true);
                        shotgunPowerUpOnField = false;
                        break;
                    case POWERUP_COFFEE:
                        player.SetCoffeePowerUp(true);
                        coffeePowerUpOnField = false;
                        break;
                    case POWERUP_NUKE:
                        player.SetNukePowerUp(true);
                        nukePowerUpOnField = false;
                        break;
                    case POWERUP_LIFE:
                        player.SetLives(player.GetLives() + 1);
                        lifePowerUpOnField = false;
                        break;
                    }

                    PowerUp::PlayPickupSound();
                }

                Rectangle bounds = level.GetLevelBounds();
                if (powerUps[i].IsOutOfBounds(bounds.x, bounds.y, bounds.width, bounds.height))
                {
                    PowerUpType type = powerUps[i].GetType();
                    powerUps[i].SetActive(false);

                    switch (type)
                    {
                    case POWERUP_WHEEL:
                        wheelPowerUpOnField = false;
                        break;
                    case POWERUP_SHOTGUN:
                        shotgunPowerUpOnField = false;
                        break;
                    case POWERUP_COFFEE:
                        coffeePowerUpOnField = false;
                        break;
                    case POWERUP_NUKE:
                        nukePowerUpOnField = false;
                        break;
                    case POWERUP_LIFE:
                        lifePowerUpOnField = false;
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (bullets[i].IsActive())
            {
                bullets[i].Update(deltaTime);

                Rectangle bounds = level.GetLevelBounds();
                Rectangle bulletRect = bullets[i].GetCollisionRect();
                if (bulletRect.x < bounds.x ||
                    bulletRect.x > bounds.x + bounds.width ||
                    bulletRect.y < bounds.y ||
                    bulletRect.y > bounds.y + bounds.height)
                {
                    bullets[i].SetActive(false);
                }
            }
        }

        for (int i = 0; i < MAX_DEATH_ANIMATIONS; i++)
        {
            if (deathAnimations[i].IsActive())
            {
                deathAnimations[i].Update(deltaTime);
            }
        }

        for (int i = 0; i < MAX_ENEMIES; i++)
        {
            if (enemies[i].IsActive())
            {
                if (CheckCollisionRecs(player.GetCollisionRect(), enemies[i].GetCollisionRect()))
                {
                    enemies[i].SetActive(false);
                    player.DecreaseLives();

                    if (player.GetLives() <= 0)
                    {
                        ui.SetGameState(GAME_OVER);
                        return;
                    }
                }
            }
        }

        HandleBulletCollisions();

        for (int i = 0; i < level.GetObstacleCount(); i++)
        {
            player.HandleObstacleCollision(level.GetObstacle(i));
        }

        Rectangle bounds = level.GetLevelBounds();
        bool moving = false;

        if (IsKeyDown(KEY_W))
        {
            player.MoveUp(deltaTime, bounds.y);
            moving = true;
        }

        if (IsKeyDown(KEY_S))
        {
            player.MoveDown(deltaTime, bounds.y, bounds.height);
            moving = true;
        }

        if (IsKeyDown(KEY_A))
        {
            player.MoveLeft(deltaTime, bounds.x);
            moving = true;
        }

        if (IsKeyDown(KEY_D))
        {
            player.MoveRight(deltaTime, bounds.x, bounds.width);
            moving = true;
        }

        player.UpdateAnimation(deltaTime, moving);

        int bulletCount = 0;
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (bullets[i].IsActive())
                bulletCount++;
        }
        player.Shoot(bullets, bulletCount, deltaTime);
    }

    // Update boss defeat sequence if active
    if (bossDefeated) {
        UpdateBossDefeatSequence(deltaTime);
    }
}

void GameManager::UpdateGameOver(float deltaTime)
{
    if (IsKeyPressed(KEY_SPACE))
    {
        ResetGame();
        ui.SetGameState(MENU);
    }
}

void GameManager::UpdateLevelCompleted(float deltaTime)
{
    if (IsKeyPressed(KEY_SPACE))
    {
        ResetGame();
        ui.SetGameState(MENU);
    }
}

void GameManager::SpawnEnemy()
{
    int currentLevel = level.GetCurrentLevel();

    // For level 10, only spawn boss enemy
    if (currentLevel == 10) {
        // Check if boss has ever been spawned (active or inactive)
        bool bossExists = false;
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemies[i].GetType() == ENEMY_BOSS) {
                bossExists = true;
                break;
            }
        }

        // Only spawn boss if no boss has been created yet
        if (!bossExists) {
            // Spawn boss at 13th row, 8th column of 16x16 grid
            Rectangle bounds = level.GetLevelBounds();
            float tileWidth = bounds.width / 16.0f;
            float tileHeight = bounds.height / 16.0f;

            Vector2 bossSpawnPos = {
                bounds.x + 8 * tileWidth,  // 8th column (0-indexed as 7)
                bounds.y + 13 * tileHeight // 13th row (0-indexed as 12)
            };

            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (!enemies[i].IsActive()) {
                    float baseSpeed = 100.0f;
                    float speedBonus = currentLevel * 5.0f;
                    float finalSpeed = baseSpeed + speedBonus;

                    // Boss scale decreased by 30% (0.2f * 0.7 = 0.14f)
                    enemies[i].Init(bossSpawnPos, 0.14f, finalSpeed, ENEMY_BOSS);
                    break;
                }
            }
        }
        return; // Don't spawn any other enemies on level 10
    }

    // Original spawning logic for levels 1-9
    Rectangle bounds = level.GetLevelBounds();
    float bgX = bounds.x;
    float bgY = bounds.y;
    float bgWidth = bounds.width;
    float bgHeight = bounds.height;

    Vector2 spawnPositions[] = {
        {bgX + 15, bgY + bgHeight * 0.4f + (float)(rand() % (int)(bgHeight * 0.2f))},
        {bgX + bgWidth - 15, bgY + bgHeight * 0.4f + (float)(rand() % (int)(bgHeight * 0.2f))},
        {bgX + bgWidth * 0.4f + (float)(rand() % (int)(bgWidth * 0.2f)), bgY + 15},
        {bgX + bgWidth * 0.4f + (float)(rand() % (int)(bgWidth * 0.2f)), bgY + bgHeight - 15} };

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemies[i].IsActive())
        {
            int spawnIndex = rand() % 4;

            EnemyType enemyType = ENEMY_ORC;
            int randomVal = rand() % 100;

            if (currentLevel >= 4)
            {
                if (randomVal < 40)
                {
                    enemyType = ENEMY_ORC;
                }
                else if (randomVal < 70)
                {
                    enemyType = ENEMY_OGRE;
                }
                else
                {
                    enemyType = ENEMY_MUMMY;
                }
            }
            else if (currentLevel >= 2)
            {
                if (randomVal < 60)
                {
                    enemyType = ENEMY_ORC;
                }
                else
                {
                    enemyType = ENEMY_OGRE;
                }
            }
            else
            {
                enemyType = ENEMY_ORC;
            }

            float baseSpeed = 100.0f;
            float speedBonus = currentLevel * 5.0f;
            float randomVariation = rand() % 50;
            float finalSpeed = baseSpeed + speedBonus + randomVariation;

            enemies[i].Init(spawnPositions[spawnIndex], 0.2f, finalSpeed, enemyType);
            break;
        }
    }
}

void GameManager::SpawnCoin(Vector2 position)
{
    for (int i = 0; i < MAX_COINS; i++)
    {
        if (!coins[i].IsActive())
        {
            coins[i].Init(position);
            break;
        }
    }
}

void GameManager::SpawnPowerUp(Vector2 position)
{
    int powerUpType = rand() % 4;
    SpawnSpecificPowerUp(position, static_cast<PowerUpType>(powerUpType));
}

void GameManager::SpawnSpecificPowerUp(Vector2 position, PowerUpType type)
{
    switch (type)
    {
    case POWERUP_WHEEL:
        if (wheelPowerUpOnField || player.HasWheelPowerUp() || player.IsWheelPowerUpActive())
            return;
        break;
    case POWERUP_SHOTGUN:
        if (shotgunPowerUpOnField || player.HasShotgunPowerUp() || player.IsShotgunPowerUpActive())
            return;
        break;
    case POWERUP_COFFEE:
        if (coffeePowerUpOnField || player.HasCoffeePowerUp() || player.IsCoffeePowerUpActive())
            return;
        break;
    case POWERUP_NUKE:
        if (nukePowerUpOnField || player.HasNukePowerUp())
            return;
        break;
    case POWERUP_LIFE:
        if (lifePowerUpOnField)
            return;
        break;
    }

    for (int i = 0; i < MAX_POWERUPS; i++)
    {
        if (!powerUps[i].IsActive())
        {
            switch (type)
            {
            case POWERUP_WHEEL:
                powerUps[i].InitWheel(position);
                wheelPowerUpOnField = true;
                break;
            case POWERUP_SHOTGUN:
                powerUps[i].InitShotgun(position);
                shotgunPowerUpOnField = true;
                break;
            case POWERUP_COFFEE:
                powerUps[i].InitCoffee(position);
                coffeePowerUpOnField = true;
                break;
            case POWERUP_NUKE:
                powerUps[i].InitNuke(position);
                nukePowerUpOnField = true;
                break;
            case POWERUP_LIFE:
                powerUps[i].InitLife(position);
                lifePowerUpOnField = true;
                break;
            }
            break;
        }
    }
}

void GameManager::CreateDeathAnimation(Vector2 position)
{
    for (int i = 0; i < MAX_DEATH_ANIMATIONS; i++)
    {
        if (!deathAnimations[i].IsActive())
        {
            deathAnimations[i].Init(position);
            break;
        }
    }
}

void GameManager::HandleNukeEffect()
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].IsActive())
        {
            Vector2 enemyPos = enemies[i].GetPosition();

            CreateDeathAnimation(enemyPos);

            enemies[i].SetActive(false);
            enemiesKilled++;

            SpawnCoin(enemyPos);
        }
    }
}
void GameManager::SpawnTrophy(Vector2 position) {
    // Use the existing coin system to spawn a trophy
    for (int i = 0; i < MAX_COINS; i++) {
        if (!coins[i].IsActive()) {
            coins[i].Init(position);
            coins[i].SetAsTrophy(); // Use the correct method name
            trophyOnField = true;
            break;
        }
    }
}
void GameManager::HandleBulletCollisions()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].IsActive())
        {

            if (bullets[i].GetBulletType() == BULLET_BOSS || bullets[i].GetBulletType() == BULLET_BOSS_RAIN)
            {
                if (CheckCollisionRecs(bullets[i].GetCollisionRect(), player.GetCollisionRect()))
                {
                    bullets[i].SetActive(false);
                    player.DecreaseLives();

                    if (player.GetLives() <= 0)
                    {
                        ui.SetGameState(GAME_OVER);
                        return;
                    }
                    continue; // Skip other collision checks for this bullet
                }

                // ADD THIS SECTION - Check boss bullet collision with obstacles
                bool bulletHitObstacle = false;
                for (int k = 0; k < level.GetObstacleCount(); k++)
                {
                    Rectangle obstacle = level.GetObstacle(k);
                    // Boss bullets are blocked by non-penetrable obstacles
                    if (!level.IsObstaclePenetrable(k) && CheckCollisionRecs(bullets[i].GetCollisionRect(), obstacle))
                    {
                        bullets[i].SetActive(false);
                        bulletHitObstacle = true;
                        break;
                    }
                }

                if (bulletHitObstacle)
                {
                    continue; // Skip to next bullet if this one hit an obstacle
                }

                continue; // Boss bullets don't hit enemies or obstacles
            }
            // Check bullet collision with obstacles first
            bool bulletHitObstacle = false;
            for (int k = 0; k < level.GetObstacleCount(); k++)
            {
                Rectangle obstacle = level.GetObstacle(k);
                // Only check collision if obstacle is not penetrable by bullets
                if (!level.IsObstaclePenetrable(k) && CheckCollisionRecs(bullets[i].GetCollisionRect(), obstacle))
                {
                    bullets[i].SetActive(false);
                    bulletHitObstacle = true;
                    break;
                }
            }

            // Only check enemy collisions if bullet didn't hit an obstacle
            if (!bulletHitObstacle)
            {
                for (int j = 0; j < MAX_ENEMIES; j++)
                {
                    if (enemies[j].IsActive() &&
                        CheckCollisionRecs(bullets[i].GetCollisionRect(), enemies[j].GetCollisionRect()))
                    {

                        int bulletDamage = bullets[i].GetDamage();
                        bool hasPenetration = bullets[i].HasPenetration();

                        bool enemyDied = enemies[j].TakeDamage(bulletDamage);

                        // Play enemy hit sound when bullet hits enemy
                        PlaySound(enemyHitSound);

                        if (enemyDied)
                        {
                            enemies[j].SetActive(false);
                            enemiesKilled++;

                            CreateDeathAnimation(enemies[j].GetPosition());

                            // Check if boss was defeated
                            if (enemies[j].GetType() == ENEMY_BOSS && !trophyOnField) {
                                // Store boss death position and spawn trophy there
                                bossDeathPosition = enemies[j].GetPosition();
                                SpawnTrophy(bossDeathPosition);
                            }
                            else {
                                // Normal enemy death drops
                                int dropChance = rand() % 100;

                                if (dropChance < 2 && !wheelPowerUpOnField && !player.HasWheelPowerUp() && !player.IsWheelPowerUpActive())
                                {
                                    SpawnSpecificPowerUp(enemies[j].GetPosition(), POWERUP_WHEEL);
                                }
                                else if (dropChance < 4 && !shotgunPowerUpOnField && !player.HasShotgunPowerUp() && !player.IsShotgunPowerUpActive())
                                {
                                    SpawnSpecificPowerUp(enemies[j].GetPosition(), POWERUP_SHOTGUN);
                                }
                                else if (dropChance < 6 && !coffeePowerUpOnField && !player.HasCoffeePowerUp() && !player.IsCoffeePowerUpActive())
                                {
                                    SpawnSpecificPowerUp(enemies[j].GetPosition(), POWERUP_COFFEE);
                                }
                                else if (dropChance < 8 && !nukePowerUpOnField && !player.HasNukePowerUp())
                                {
                                    SpawnSpecificPowerUp(enemies[j].GetPosition(), POWERUP_NUKE);
                                }
                                else if (dropChance < 12 && !lifePowerUpOnField)
                                {
                                    SpawnSpecificPowerUp(enemies[j].GetPosition(), POWERUP_LIFE);
                                }
                                else if (dropChance < 90)
                                {
                                    SpawnCoin(enemies[j].GetPosition());
                                }
                            }
                        }

                        // Handle penetration: if bullet has penetration, reduce its damage instead of destroying it
                        if (hasPenetration)
                        {
                            bullets[i].ReduceDamage(1); // Reduce damage by 1 per enemy hit
                            // Bullet will be deactivated automatically if damage reaches 0 or below
                        }
                        else
                        {
                            // No penetration, bullet is destroyed on impact
                            bullets[i].SetActive(false);
                        }

                        break; // Only hit one enemy per frame to avoid multiple hits on same bullet
                    }
                }
            }
        }
    }
}

void GameManager::Draw()
{
    BeginDrawing();
    ClearBackground(BLACK);

    switch (currentState)
    {
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

void GameManager::DrawPlaying()
{
    if (isMusicPlaying)
    {
        UpdateMusicStream(backgroundMusic);
        if (IsMusicStreamPlaying(backgroundMusic) == false)
        {
            PlayMusicStream(backgroundMusic);
        }
    }

    level.Draw();

    for (int i = 0; i < MAX_DEATH_ANIMATIONS; i++)
    {
        if (deathAnimations[i].IsActive())
        {
            deathAnimations[i].Draw();
        }
    }
    if (shop.IsActive())
    {
        shop.Draw();
    }
    for (int i = 0; i < MAX_COINS; i++)
    {
        if (coins[i].IsActive())
        {
            coins[i].Draw();
        }
    }

    for (int i = 0; i < MAX_POWERUPS; i++)
    {
        if (powerUps[i].IsActive())
        {
            powerUps[i].Draw();
        }
    }

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].IsActive())
        {
            enemies[i].Draw();
        }
    }

    for (int i = 0; i < MAX_BEAVERS; i++)
    {
        if (beavers[i].IsActive())
        {
            beavers[i].Draw();
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].IsActive())
        {
            bullets[i].Draw();
        }
    }

    player.Draw();

    if (level.ShouldShowArrow())
    {
        level.DrawArrow();
    }

    float activeTimer = 0.0f;
    if (player.IsWheelPowerUpActive())
    {
        activeTimer = player.GetWheelPowerUpTimer();
    }
    else if (player.IsShotgunPowerUpActive())
    {
        activeTimer = player.GetShotgunPowerUpTimer();
    }
    else if (player.IsCoffeePowerUpActive())
    {
        activeTimer = player.GetCoffeePowerUpTimer();
    }

    // Boss defeat sequence drawing is handled in DrawBossDefeatSequence()
    // which is called at the end of DrawPlaying()

    ui.DrawHUD(
        player.GetLives(),
        coinsCollected,
        enemiesKilled,
        timeRemaining,
        player.HasWheelPowerUp() || player.HasShotgunPowerUp() || player.HasCoffeePowerUp() || player.HasNukePowerUp(),
        player.IsWheelPowerUpActive() || player.IsShotgunPowerUpActive() || player.IsCoffeePowerUpActive(),
        activeTimer,
        level.GetCurrentLevel(),
        player.HasWheelPowerUp(),
        player.HasShotgunPowerUp(),
        player.HasCoffeePowerUp(),
        player.HasNukePowerUp(),
        player.IsWheelPowerUpActive(),
        player.IsShotgunPowerUpActive(),
        player.IsCoffeePowerUpActive(),
        shop.GetGunUpgradeLevel(),
        shop.GetBootUpgradeLevel(),
        shop.GetBoxUpgradeLevel());

    // Draw purchased shop items permanently in bottom-left corner
    ui.DrawShopItems(
        shop.GetGunUpgradeLevel(),
        shop.GetBootUpgradeLevel(),
        shop.GetBoxUpgradeLevel());
    // Draw purchase notification if there is a recent purchase

    if (player.IsNukeActivated())
    {
        float alpha = player.GetNukeEffectTimer() * 2.0f;
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(WHITE, alpha));
    }

    DrawBossDefeatSequence();
}

bool GameManager::ShouldClose() const
{
    return WindowShouldClose();
}

void GameManager::UpdateEnemyBeaverDistraction()
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (enemies[i].IsActive())
        {
            enemies[i].ClearBeaverTarget();
        }
    }

    for (int beaverIndex = 0; beaverIndex < MAX_BEAVERS; beaverIndex++)
    {
        if (beavers[beaverIndex].IsActive())
        {
            Vector2 beaverPos = beavers[beaverIndex].GetPosition();

            for (int enemyIndex = 0; enemyIndex < MAX_ENEMIES; enemyIndex++)
            {
                if (enemies[enemyIndex].IsActive())
                {
                    enemies[enemyIndex].SetBeaverTarget(beaverPos);
                }
            }

            break;
        }
    }
}

void GameManager::HandleBossDefeat(Vector2 bossPosition) {
    bossDefeated = true;
    bossDefeatTimer = 0.0f;
    showWhiteScreen = true;
    showCrackEffect = false;
    playingLightning = false;
    showBridge = false;
    trophySpawned = false;

    // Set up bridge position (center of level)
    Rectangle bounds = level.GetLevelBounds();
    bridgePosition = {
        bounds.x + bounds.width / 2 - (bridgeTexture.width * 0.2f) / 2,
        bounds.y + bounds.height / 2 - (bridgeTexture.height * 0.2f) / 2
    };

    // Set completed level background
    level.SetCompleted();
}

void GameManager::UpdateBossDefeatSequence(float deltaTime) {
    if (!bossDefeated) return;

    bossDefeatTimer += deltaTime;

    // Phase 1: White screen (0-2 seconds)
    if (bossDefeatTimer < whiteScreenDuration) {
        showWhiteScreen = true;
        showCrackEffect = false;
        playingLightning = false;
        showBridge = false;
    }
    // Phase 2: Crack effect (2-3 seconds)
    else if (bossDefeatTimer < whiteScreenDuration + 1.0f) {
        showWhiteScreen = false;
        showCrackEffect = true;
        playingLightning = false;
        showBridge = false;

        crackEffectTimer += deltaTime;
    }
    // Phase 3: Lightning animation (3-4.5 seconds)
    else if (bossDefeatTimer < whiteScreenDuration + 2.5f) {
        showWhiteScreen = false;
        showCrackEffect = false;
        playingLightning = true;
        showBridge = false;

        lightningTimer += deltaTime;
        if (lightningTimer >= 0.1f) // Change frame every 0.1 seconds
        {
            lightningFrame = (lightningFrame + 1) % maxLightningFrames;
            lightningTimer = 0.0f;
        }
    }
    // Phase 4: Bridge appears and trophy spawns (4.5-6 seconds)
    else if (bossDefeatTimer < whiteScreenDuration + 4.0f) {
        showWhiteScreen = false;
        showCrackEffect = false;
        playingLightning = false;
        showBridge = true;

        if (!trophySpawned) {
            // Spawn trophy at boss death position
            Vector2 trophyPos = bossDeathPosition;
            SpawnTrophy(trophyPos);
            trophySpawned = true;
        }
    }
    // Phase 5: Boss defeat sequence complete - start player exit animation
    else {
        showWhiteScreen = false;
        showCrackEffect = false;
        playingLightning = false;
        showBridge = true; // Keep bridge visible during player exit

        // Start player exit animation if not already started
        if (!playerExiting) {
            Rectangle bounds = level.GetLevelBounds();
            playerExiting = true;
            playerExitTimer = 0.0f;
            // Set exit target to the right edge of the screen
            playerExitTarget = { bounds.x + bounds.width + 100, player.GetPosition().y };
        }

        // Boss defeat sequence is now complete, but player exit continues in UpdatePlaying
        bossDefeated = false;
    }
}

void GameManager::DrawBossDefeatSequence() {
    if (!bossDefeated && !showBridge) return;

    // Draw white screen
    if (showWhiteScreen) {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    }

    // Draw crack effect
    if (showCrackEffect) {
        // Draw multiple crack lines across the screen
        for (int i = 0; i < 10; i++) {
            Vector2 start = { (float)(rand() % SCREEN_WIDTH), 0 };
            Vector2 end = { (float)(rand() % SCREEN_WIDTH), SCREEN_HEIGHT };
            DrawLineEx(start, end, 3.0f, BLACK);
        }

        // Draw some horizontal cracks too
        for (int i = 0; i < 5; i++) {
            Vector2 start = { 0, (float)(rand() % SCREEN_HEIGHT) };
            Vector2 end = { SCREEN_WIDTH, (float)(rand() % SCREEN_HEIGHT) };
            DrawLineEx(start, end, 2.0f, BLACK);
        }
    }

    // Draw lightning animation
    if (playingLightning) {
        // Create lightning effect with random lines
        Color lightningColor = WHITE;
        for (int i = 0; i < 15; i++) {
            Vector2 start = { SCREEN_WIDTH / 2.0f + (rand() % 200 - 100), 0 };
            Vector2 end = { SCREEN_WIDTH / 2.0f + (rand() % 200 - 100), SCREEN_HEIGHT };
            DrawLineEx(start, end, 8.0f, lightningColor);
        }

        // Add some glow effect
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(YELLOW, 0.3f));
    }

    // Draw bridge when visible (during phase 4 and player exit)
    // if (showBridge) {
    //     DrawTextureEx(bridgeTexture, bridgePosition, 0.0f, 0.2f, WHITE);
    // }
}

void GameManager::PlayWhiteScreenWithCrack() {
    showWhiteScreen = true;
    showCrackEffect = false;
}

void GameManager::PlayLightningAnimation() {
    playingLightning = true;
    lightningFrame = 0;
    lightningTimer = 0.0f;
}

void GameManager::DisplayBridge() {
    showBridge = true;
}