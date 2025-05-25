#include "../include/Player.h"

Player::Player()
    : Entity({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}, 3.0f),
    currentFrame(1),
    frameTime(0.2f),
    frameCounter(0.0f),
    speed(200.0f),
    defaultSpeed(200.0f),
    movingLeft(false),
    movingRight(false),
    currentShootDirection(NONE),
    shootAnimationTimer(0.0f),
    shootAnimationDuration(0.3f),
    lastBulletTime(0.0f),
    bulletCooldown(0.2f),
    footstepCooldown(0.0f),
    footstepDelay(0.3f),
    lives(3),
    hiTimer(0.0f),
    hiDuration(1.0f),
    // Wheel power-up
    hasWheelPowerUp(false),
    wheelPowerUpActive(false),
    wheelPowerUpTimer(0.0f),
    wheelPowerUpDuration(12.0f),  // Set to 12 seconds
    wheelShootTimer(0.0f),
    wheelShootCooldown(0.5f),
    // Shotgun power-up
    hasShotgunPowerUp(false),
    shotgunPowerUpActive(false),
    shotgunPowerUpTimer(0.0f),
    shotgunPowerUpDuration(12.0f),  // Set to 12 seconds
    // Coffee power-up
    hasCoffeePowerUp(false),
    coffeePowerUpActive(false),
    coffeePowerUpTimer(0.0f),
    coffeePowerUpDuration(12.0f),  // Set to 12 seconds
    coffeeSpeedMultiplier(1.6f),
    // Nuke power-up
    hasNukePowerUp(false),
    nukeActivated(false),
    nukeEffectTimer(0.0f),
    // Shop upgrades
    gunUpgradeLevel(0),
    bootUpgradeLevel(0),
    boxUpgradeLevel(0),
    baseBulletCooldown(0.2f),
    baseSpeed(200.0f),
    baseMaxLives(3) {
    
    active = true;
}

Player::~Player() {
    UnloadTextures();
}

void Player::LoadTextures() {
    sprites[0] = LoadTexture("resources/player/player1.png");
    sprites[1] = LoadTexture("resources/player/player2.png");
    sprites[2] = LoadTexture("resources/player/player3.png");
    sprites[3] = LoadTexture("resources/player/player4.png");

    hiTexture = LoadTexture("resources/player/hi.png");

    shootSprites[0] = LoadTexture("resources/player/move_left_shoot_down.png");
    shootSprites[1] = LoadTexture("resources/player/move_left_shoot_left.png");
    shootSprites[2] = LoadTexture("resources/player/move_left_shoot_right.png");
    shootSprites[3] = LoadTexture("resources/player/move_left_shoot_up.png");
    shootSprites[4] = LoadTexture("resources/player/move_right_shoot_down.png");
    shootSprites[5] = LoadTexture("resources/player/move_right_shoot_left.png");
    shootSprites[6] = LoadTexture("resources/player/move_right_shoot_right.png");
    shootSprites[7] = LoadTexture("resources/player/move_right_shoot_up.png");
    shootSprites[8] = LoadTexture("resources/player/stay_shoot_down.png");
    shootSprites[9] = LoadTexture("resources/player/stay_shoot_left.png");
    shootSprites[10] = LoadTexture("resources/player/stay_shoot_right.png");
    shootSprites[11] = LoadTexture("resources/player/stay_shoot_up.png");
    
    // Load sounds
    footstepSound = LoadSound("resources/Sounds/Footstep.wav");
    gunshotSound = LoadSound("resources/Sounds/Gunshot.wav");
    hitSound = LoadSound("resources/Sounds/Hit.wav");
    pickupPowerUpSound = LoadSound("resources/Sounds/PickUpPowerUp.wav");
    nukeSound = LoadSound("resources/Sounds/PickUpPowerUp.wav");  // You may want to use a different sound
    
    // Set sound volumes
    SetSoundVolume(footstepSound, 0.5f);
    SetSoundVolume(gunshotSound, 1.0f);  // Increased from 0.7f to 1.0f for maximum volume
    SetSoundVolume(hitSound, 0.6f);
    SetSoundVolume(pickupPowerUpSound, 0.6f);
    SetSoundVolume(nukeSound, 0.8f);
}

void Player::UnloadTextures() {
    for (int i = 0; i < 4; i++) UnloadTexture(sprites[i]);
    for (int i = 0; i < 12; i++) UnloadTexture(shootSprites[i]);
    UnloadTexture(hiTexture);
    
    // Unload sounds
    UnloadSound(footstepSound);
    UnloadSound(gunshotSound);
    UnloadSound(hitSound);
    UnloadSound(pickupPowerUpSound);
    UnloadSound(nukeSound);
}

void Player::Update(float deltaTime) {
    if (!active) return;

    if (hiTimer > 0) {
        hiTimer -= deltaTime;
    }

    if (shootAnimationTimer > 0) {
        shootAnimationTimer -= deltaTime;
    } else {
        currentShootDirection = NONE;
    }
    
    // Update shotgun power-up
    if (shotgunPowerUpActive) {
        UpdateShotgunPowerUp(deltaTime);
    }
    
    // Update coffee power-up
    if (coffeePowerUpActive) {
        UpdateCoffeePowerUp(deltaTime);
    }
    
    // Update nuke effect
    if (nukeActivated) {
        UpdateNukeEffect(deltaTime);
    }
}

void Player::Draw() {
    if (!active) return;
    
    DrawTextureEx(GetCurrentTexture(), position, 0.0f, scale, WHITE);
}

void Player::MoveUp(float deltaTime, float bgY) {
    if (position.y > bgY) {
        position.y -= speed * deltaTime;
    }
}

void Player::MoveDown(float deltaTime, float bgY, float bgHeight) {
    if (position.y + sprites[0].height * scale < bgY + bgHeight) {
        position.y += speed * deltaTime;
    }
}

void Player::MoveLeft(float deltaTime, float bgX) {
    if (position.x > bgX) {
        position.x -= speed * deltaTime;
        movingLeft = true;
        movingRight = false;
    }
}

void Player::MoveRight(float deltaTime, float bgX, float bgWidth) {
    if (position.x + sprites[0].width * scale < bgX + bgWidth) {
        position.x += speed * deltaTime;
        movingRight = true;
        movingLeft = false;
    }
}

void Player::UpdateAnimation(float deltaTime, bool moving) {
    if (!moving) {
        currentFrame = 1;
        movingLeft = false;
        movingRight = false;
    } else {
        frameCounter += deltaTime;
        if (frameCounter >= frameTime) {
            frameCounter = 0.0f;

            if (movingLeft) {
                currentFrame = (currentFrame == 0) ? 1 : 0;
            } else if (movingRight) {
                currentFrame = (currentFrame == 2) ? 3 : 2;
            } else {
                currentFrame = (currentFrame == 1) ? 2 : 1;
            }
        }
        
        // Handle footstep sound
        footstepCooldown -= deltaTime;
        if (footstepCooldown <= 0.0f) {
            PlaySound(footstepSound);
            footstepCooldown = footstepDelay;
        }
    }
}

void Player::Shoot(Bullet bullets[], int& bulletCount, float deltaTime) {
    if (GetTime() - lastBulletTime < bulletCooldown) return;
    
    if (bulletCount >= MAX_BULLETS) return;
    
    Vector2 bulletDir = { 0, 0 };
    bool shouldShoot = false;
    
    if (IsKeyPressed(KEY_UP)) {
        bulletDir = { 0, -1 };
        shouldShoot = true;
        currentShootDirection = UP;
        shootAnimationTimer = shootAnimationDuration;
    }
    if (IsKeyPressed(KEY_DOWN)) {
        bulletDir = { 0, 1 };
        shouldShoot = true;
        currentShootDirection = DOWN;
        shootAnimationTimer = shootAnimationDuration;
    }
    if (IsKeyPressed(KEY_LEFT)) {
        bulletDir = { -1, 0 };
        shouldShoot = true;
        currentShootDirection = LEFT;
        shootAnimationTimer = shootAnimationDuration;
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        bulletDir = { 1, 0 };
        shouldShoot = true;
        currentShootDirection = RIGHT;
        shootAnimationTimer = shootAnimationDuration;
    }
    
    if (shouldShoot) {
        // If shotgun power-up is active, shoot a shotgun blast
        if (shotgunPowerUpActive) {
            ShootShotgun(bullets, bulletCount);
        } else {
            // Normal shooting
            Texture2D currentTex = GetCurrentTexture();
            float pWidth = currentTex.width * scale;
            float pHeight = currentTex.height * scale;
            float playerCenterX = position.x + pWidth / 2;
            float playerCenterY = position.y + pHeight / 2;
            
            // Find an inactive bullet
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!bullets[i].IsActive()) {
                    Vector2 bulletPos = {
                        playerCenterX - bullets[i].GetCollisionRect().width / 2 + bulletDir.x * 20,
                        playerCenterY - bullets[i].GetCollisionRect().height / 2 + bulletDir.y * 20
                    };
                    
                    bullets[i].Init(bulletPos, bulletDir);
                    bulletCount++;
                    lastBulletTime = GetTime();
                    
                    PlaySound(gunshotSound);
                    break;
                }
            }
        }
    }
}

void Player::ShootCircle(Bullet bullets[], int& bulletCount) {
    if (bulletCount >= MAX_BULLETS - 8) return;

    float playerCenterX = position.x + (sprites[0].width * scale) / 2;
    float playerCenterY = position.y + (sprites[0].height * scale) / 2;

    for (int i = 0; i < 8; i++) {
        float angle = i * 45 * DEG2RAD;
        Vector2 direction = { cosf(angle), sinf(angle) };

        // Find inactive bullets to use
        for (int j = 0; j < MAX_BULLETS; j++) {
            if (!bullets[j].IsActive()) {
                Vector2 bulletPos = {
                    playerCenterX - bullets[j].GetCollisionRect().width / 2,
                    playerCenterY - bullets[j].GetCollisionRect().height / 2
                };
                
                bullets[j].Init(bulletPos, direction);
                bulletCount++;
                break;
            }
        }
    }
    PlaySound(gunshotSound);
}

// New method to shoot in a shotgun pattern
void Player::ShootShotgun(Bullet bullets[], int& bulletCount) {
    if (bulletCount >= MAX_BULLETS - 3) return;

    Texture2D currentTex = GetCurrentTexture();
    float pWidth = currentTex.width * scale;
    float pHeight = currentTex.height * scale;
    float playerCenterX = position.x + pWidth / 2;
    float playerCenterY = position.y + pHeight / 2;
    
    // Base direction based on shoot direction
    Vector2 baseDir = {0, 0};
    switch (currentShootDirection) {
        case UP:    baseDir = {0, -1}; break;
        case DOWN:  baseDir = {0, 1}; break;
        case LEFT:  baseDir = {-1, 0}; break;
        case RIGHT: baseDir = {1, 0}; break;
        default:    return;  // No direction, no shot
    }
    
    // Create 3 bullet directions: one straight, two at angles
    Vector2 directions[3] = {
        baseDir,  // Center bullet - straight ahead
        {0, 0},   // Left spread bullet
        {0, 0}    // Right spread bullet
    };
    
    // Calculate spread directions based on base direction
    if (baseDir.x == 0) { // Shooting up or down
        // Add horizontal spread
        directions[1] = { -0.3f, baseDir.y * 0.7f };
        directions[2] = { 0.3f, baseDir.y * 0.7f };
    } else { // Shooting left or right
        // Add vertical spread
        directions[1] = { baseDir.x * 0.7f, -0.3f };
        directions[2] = { baseDir.x * 0.7f, 0.3f };
    }

    // Normalize the spread directions
    for (int i = 1; i < 3; i++) {
        float length = sqrtf(directions[i].x * directions[i].x + directions[i].y * directions[i].y);
        directions[i].x /= length;
        directions[i].y /= length;
    }
    
    // Shoot all three bullets
    int bulletsFired = 0;
    for (int i = 0; i < 3 && bulletsFired < 3; i++) {
        // Find an inactive bullet
        for (int j = 0; j < MAX_BULLETS; j++) {
            if (!bullets[j].IsActive()) {
                Vector2 bulletPos = {
                    playerCenterX - bullets[j].GetCollisionRect().width / 2 + directions[i].x * 20,
                    playerCenterY - bullets[j].GetCollisionRect().height / 2 + directions[i].y * 20
                };
                
                bullets[j].Init(bulletPos, directions[i]);
                bulletCount++;
                bulletsFired++;
                break;
            }
        }
    }
    
    // Only play sound once for all three bullets
    PlaySound(gunshotSound);
    lastBulletTime = GetTime();
}

void Player::SetWheelPowerUp(bool has) {
    hasWheelPowerUp = has;
}

bool Player::HasWheelPowerUp() const {
    return hasWheelPowerUp;
}

bool Player::IsWheelPowerUpActive() const {
    return wheelPowerUpActive;
}

void Player::ActivateWheelPowerUp() {
    if (hasWheelPowerUp && !wheelPowerUpActive) {
        wheelPowerUpActive = true;
        wheelPowerUpTimer = wheelPowerUpDuration;
        wheelShootTimer = 0.0f;
        hasWheelPowerUp = false;
    }
}

void Player::UpdateWheelPowerUp(float deltaTime, Bullet bullets[], int& bulletCount) {
    if (!wheelPowerUpActive) return;
    
    wheelPowerUpTimer -= deltaTime;
    wheelShootTimer -= deltaTime;
    
    if (wheelShootTimer <= 0 && bullets != nullptr) {
        ShootCircle(bullets, bulletCount);
        wheelShootTimer = wheelShootCooldown;
    }
    
    if (wheelPowerUpTimer <= 0) {
        wheelPowerUpActive = false;
    }
}

float Player::GetWheelPowerUpTimer() const {
    return wheelPowerUpTimer;
}

// SHOTGUN POWER-UP METHODS
void Player::SetShotgunPowerUp(bool has) {
    hasShotgunPowerUp = has;
}

bool Player::HasShotgunPowerUp() const {
    return hasShotgunPowerUp;
}

bool Player::IsShotgunPowerUpActive() const {
    return shotgunPowerUpActive;
}

float Player::GetShotgunPowerUpTimer() const {
    return shotgunPowerUpTimer;
}

void Player::ActivateShotgunPowerUp() {
    if (hasShotgunPowerUp && !shotgunPowerUpActive) {
        shotgunPowerUpActive = true;
        shotgunPowerUpTimer = shotgunPowerUpDuration;
        hasShotgunPowerUp = false;
    }
}

void Player::UpdateShotgunPowerUp(float deltaTime) {
    if (!shotgunPowerUpActive) return;
    
    shotgunPowerUpTimer -= deltaTime;
    
    if (shotgunPowerUpTimer <= 0) {
        shotgunPowerUpActive = false;
    }
}

// COFFEE POWER-UP METHODS
void Player::SetCoffeePowerUp(bool has) {
    hasCoffeePowerUp = has;
}

bool Player::HasCoffeePowerUp() const {
    return hasCoffeePowerUp;
}

bool Player::IsCoffeePowerUpActive() const {
    return coffeePowerUpActive;
}

float Player::GetCoffeePowerUpTimer() const {
    return coffeePowerUpTimer;
}

void Player::ActivateCoffeePowerUp() {
    if (hasCoffeePowerUp && !coffeePowerUpActive) {
        coffeePowerUpActive = true;
        coffeePowerUpTimer = coffeePowerUpDuration;
        speed = defaultSpeed * coffeeSpeedMultiplier;
        hasCoffeePowerUp = false;
    }
}

void Player::UpdateCoffeePowerUp(float deltaTime) {
    if (!coffeePowerUpActive) return;
    
    coffeePowerUpTimer -= deltaTime;
    
    if (coffeePowerUpTimer <= 0) {
        coffeePowerUpActive = false;
        speed = defaultSpeed; // Reset speed back to default
    }
}

// NUKE POWER-UP METHODS
void Player::SetNukePowerUp(bool has) {
    hasNukePowerUp = has;
}

bool Player::HasNukePowerUp() const {
    return hasNukePowerUp;
}

void Player::ActivateNukePowerUp() {
    if (hasNukePowerUp && !nukeActivated) {
        nukeActivated = true;
        nukeEffectTimer = 0.5f; // Duration of visual effect
        hasNukePowerUp = false;
        PlaySound(nukeSound);
    }
}

bool Player::IsNukeActivated() const {
    return nukeActivated;
}

float Player::GetNukeEffectTimer() const {
    return nukeEffectTimer;
}

void Player::UpdateNukeEffect(float deltaTime) {
    if (!nukeActivated) return;
    
    nukeEffectTimer -= deltaTime;
    
    if (nukeEffectTimer <= 0) {
        nukeActivated = false;
    }
}

void Player::ResetNukeEffect() {
    nukeActivated = false;
    nukeEffectTimer = 0.0f;
}

void Player::HandleObstacleCollision(Rectangle obstacle) {
    Texture2D currentTex = GetCurrentTexture();
    float pWidth = currentTex.width * scale;
    float pHeight = currentTex.height * scale;
    Rectangle playerRect = { position.x, position.y, pWidth, pHeight };
    
    if (!CheckCollisionRecs(playerRect, obstacle)) return;
    
    // Resolve collision
    if (position.x + pWidth > obstacle.x && position.x < obstacle.x) {
        position.x = obstacle.x - pWidth;
    }
    if (position.x < obstacle.x + obstacle.width && position.x + pWidth > obstacle.x + obstacle.width) {
        position.x = obstacle.x + obstacle.width;
    }
    if (position.y + pHeight > obstacle.y && position.y < obstacle.y) {
        position.y = obstacle.y - pHeight;
    }
    if (position.y < obstacle.y + obstacle.height && position.y + pHeight > obstacle.y + obstacle.height) {
        position.y = obstacle.y + obstacle.height;
    }
}

Texture2D Player::GetCurrentTexture() const {
    if (hiTimer > 0) {
        return hiTexture;
    }

    if (shootAnimationTimer > 0) {
        if (movingLeft) {
            switch (currentShootDirection) {
            case UP: return shootSprites[3];
            case DOWN: return shootSprites[0];
            case LEFT: return shootSprites[1];
            case RIGHT: return shootSprites[2];
            default: return sprites[currentFrame];
            }
        } else if (movingRight) {
            switch (currentShootDirection) {
            case UP: return shootSprites[7];
            case DOWN: return shootSprites[4];
            case LEFT: return shootSprites[5];
            case RIGHT: return shootSprites[6];
            default: return sprites[currentFrame];
            }
        } else {
            switch (currentShootDirection) {
            case UP: return shootSprites[11];
            case DOWN: return shootSprites[8];
            case LEFT: return shootSprites[9];
            case RIGHT: return shootSprites[10];
            default: return sprites[currentFrame];
            }
        }
    }

    return sprites[currentFrame];
}

int Player::GetLives() const {
    return lives;
}

void Player::SetLives(int lives) {
    this->lives = lives;
}

void Player::DecreaseLives() {
    lives--;
    PlaySound(hitSound);
}

Rectangle Player::GetCollisionRect() const {
    Texture2D currentTex = GetCurrentTexture();
    float width = currentTex.width * scale;
    float height = currentTex.height * scale;
    return { position.x, position.y, width, height };
}

void Player::PlayHiAnimation() {
    hiTimer = hiDuration;
}

bool Player::IsHiAnimationPlaying() const {
    return hiTimer > 0;
}

// SHOP UPGRADE METHODS
void Player::UpgradeGun() {
    if (gunUpgradeLevel < 3) {
        gunUpgradeLevel++;
        // Reduce bullet cooldown for faster shooting
        float reduction = 0.03f * gunUpgradeLevel; // 3% reduction per level
        bulletCooldown = baseBulletCooldown - reduction;
        if (bulletCooldown < 0.05f) bulletCooldown = 0.05f; // Minimum cooldown
    }
}

void Player::UpgradeBoot() {
    if (bootUpgradeLevel < 2) {
        bootUpgradeLevel++;
        // Increase movement speed
        float speedIncrease = 80.0f * bootUpgradeLevel; // 50 units per level
        speed = baseSpeed + speedIncrease;
        defaultSpeed = speed; // Update default speed too
    }
}

void Player::UpgradeBox() {
    if (boxUpgradeLevel < 3) {
        boxUpgradeLevel++;
        // Increase max lives
        int lifeIncrease = boxUpgradeLevel; // 1 life per level
        lives += lifeIncrease;
        // Update base max lives for future reference
        baseMaxLives = 3 + boxUpgradeLevel;
    }
}

// Getter methods for shop upgrade levels
int Player::GetGunUpgradeLevel() const {
    return gunUpgradeLevel;
}

int Player::getBootUpgradeLevel() const {
    return bootUpgradeLevel;
}

int Player::GetBoxUpgradeLevel() const {
    return boxUpgradeLevel;
}
