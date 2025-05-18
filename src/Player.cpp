#include "../include/Player.h"

Player::Player()
    : Entity({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}, 3.0f),
    currentFrame(1),
    frameTime(0.2f),
    frameCounter(0.0f),
    speed(200.0f),
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
    hasWheelPowerUp(false),
    wheelPowerUpActive(false),
    wheelPowerUpTimer(0.0f),
    wheelPowerUpDuration(12.0f),
    wheelShootTimer(0.0f),
    wheelShootCooldown(0.5f) {
    
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
    
    // Set sound volumes
    SetSoundVolume(footstepSound, 0.5f);
    SetSoundVolume(gunshotSound, 0.7f);
    SetSoundVolume(hitSound, 0.6f);
    SetSoundVolume(pickupPowerUpSound, 0.6f);
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
    
    // Note: We're removing the power-up timer update from here
    // because it's already handled in UpdateWheelPowerUp
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
        bulletDir = Vector2 { 0, -1 };
        shouldShoot = true;
        currentShootDirection = UP;
        shootAnimationTimer = shootAnimationDuration;
    }
    if (IsKeyPressed(KEY_DOWN)) {
        bulletDir = Vector2 { 0, 1 };
        shouldShoot = true;
        currentShootDirection = DOWN;
        shootAnimationTimer = shootAnimationDuration;
    }
    if (IsKeyPressed(KEY_LEFT)) {
        bulletDir = Vector2 { -1, 0 };
        shouldShoot = true;
        currentShootDirection = LEFT;
        shootAnimationTimer = shootAnimationDuration;
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        bulletDir = Vector2 { 1, 0 };
        shouldShoot = true;
        currentShootDirection = RIGHT;
        shootAnimationTimer = shootAnimationDuration;
    }
    
    if (shouldShoot) {
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
    return Rectangle{ position.x, position.y, width, height };
}

void Player::PlayHiAnimation() {
    hiTimer = hiDuration;
}

bool Player::IsHiAnimationPlaying() const {
    return hiTimer > 0;
}

float Player::GetWheelPowerUpTimer() const {
    return wheelPowerUpTimer;
}
