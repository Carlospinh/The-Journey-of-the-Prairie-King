#include "../include/Enemy.h"
#include <math.h>

// Initialize static members
Texture2D Enemy::orcFrames[2];
Texture2D Enemy::ogreFrames[2];
Texture2D Enemy::mummyFrames[2];
Texture2D Enemy::bossFrames[2];

Enemy::Enemy() 
    : Entity(), 
    currentFrame(0),
    frameTimer(0.0f),
    frameDuration(0.2f),
    speed(100.0f),
    type(ENEMY_ORC),
    health(1),
    hitFlashTime(0.0f),
    chasingBeaver(false),
    beaverTarget({0, 0}),
    beaverChaseRange(400.0f),
    bossSpawnPos({0, 0}),
    bossMovementRange(300.0f),
    bossDirection(1),
    bossRainTimer(0.0f),
    bossRainInterval(4.0f)  , 
    bossShootTimer(0.0f),
    bossShootInterval(3.0f),
    bossRailTimer(0.0f),
    bossRailInterval(5.0f),
    bossRailBurstCount(0),
    bossRailBurstDelay(0.1f),
    bossRailBurstTimer(0.0f),
    bossRailBurstActive(false)
    {
}

Enemy::~Enemy() {
}

void Enemy::LoadSharedTextures() {
    // Load textures for all enemy types
    orcFrames[0] = LoadTexture("resources/Enemies/Orc/Orc1.png");
    orcFrames[1] = LoadTexture("resources/Enemies/Orc/Orc2.png");
    
    ogreFrames[0] = LoadTexture("resources/Enemies/Ogre/Ogre1.png");
    ogreFrames[1] = LoadTexture("resources/Enemies/Ogre/Ogre2.png");
    
    mummyFrames[0] = LoadTexture("resources/Enemies/Mummy/Mummy1.png");
    mummyFrames[1] = LoadTexture("resources/Enemies/Mummy/Mummy2.png");
    
    bossFrames[0] = LoadTexture("resources/Boss Battle/Boss/Boss_Walking_1.png");
    bossFrames[1] = LoadTexture("resources/Boss Battle/Boss/Boss_Walking_2.png");
}

void Enemy::UnloadSharedTextures() {
    // Unload textures for all enemy types
    UnloadTexture(orcFrames[0]);
    UnloadTexture(orcFrames[1]);
    
    UnloadTexture(ogreFrames[0]);
    UnloadTexture(ogreFrames[1]);
    
    UnloadTexture(mummyFrames[0]);
    UnloadTexture(mummyFrames[1]);
    
    UnloadTexture(bossFrames[0]);
    UnloadTexture(bossFrames[1]);
}

void Enemy::Init(Vector2 position, float scale, float speed, EnemyType enemyType) {
    this->position = position;
    this->scale = scale;
    this->type = enemyType;
    this->active = true;
    this->currentFrame = 0;
    this->frameTimer = 0.0f;
    this->hitFlashTime = 0.0f;
    
    // Set speed and health based on enemy type
    switch (enemyType) {
        case ENEMY_ORC:
            this->speed = speed;             // Default speed for orcs
            this->frameDuration = 0.2f;      // Default animation speed
            this->health = 1;                // Orcs have 1 health point
            break;
        case ENEMY_OGRE:
            this->speed = speed * 0.8f;      // Ogres are slower but have more health
            this->frameDuration = 0.25f;     // Slower animation
            this->health = 3;                // Ogres have 2 health points
            break;
        case ENEMY_MUMMY:
            this->speed = speed * 1.2f;      // Mummies are faster
            this->frameDuration = 0.15f;     // Faster animation
            this->health = 6;                // Mummies have 4 health points
            break;
        case ENEMY_BOSS:
            this->speed = speed * 1.2f;      // Same speed as mummy
            this->frameDuration = 0.15f;     // Same animation speed as mummy
            this->health = 6;                // Same health as mummy
            this->bossSpawnPos = position;   // Store spawn position for horizontal movement
            this->bossMovementRange = 300.0f; // Boss can move 300 pixels left/right from spawn
            this->bossDirection = 1;         // Start moving right
            break;
    }
    
    // Copy appropriate textures based on enemy type
    switch (enemyType) {
        case ENEMY_ORC:
            this->frames[0] = orcFrames[0];
            this->frames[1] = orcFrames[1];
            break;
        case ENEMY_OGRE:
            this->frames[0] = ogreFrames[0];
            this->frames[1] = ogreFrames[1];
            break;
        case ENEMY_MUMMY:
            this->frames[0] = mummyFrames[0];
            this->frames[1] = mummyFrames[1];
            break;
        case ENEMY_BOSS:
            this->frames[0] = bossFrames[0];
            this->frames[1] = bossFrames[1];
            break;
    }
}

void Enemy::Update(float deltaTime) {
    if (!active) return;
    
    // Update hit flash effect
    if (hitFlashTime > 0.0f) {
        hitFlashTime -= deltaTime;
    }
    
    // Update animation
    frameTimer += deltaTime;
    if (frameTimer >= frameDuration) {
        frameTimer = 0.0f;
        currentFrame = (currentFrame + 1) % 2;
    }
    
    // Special behaviors based on enemy type
    switch (type) {
        case ENEMY_ORC:
            // Standard movement behavior is handled by MoveToward
            break;
        
        case ENEMY_OGRE:
            // Ogres occasionally pause their movement
            if (GetRandomValue(0, 120) == 0) {
                // Pause for a moment (by temporarily setting speed to 0)
                speed *= 0.5f;
                // Speed will be restored in the next movement
            }
            break;
        
        case ENEMY_MUMMY:
            // Mummies can occasionally make quick dashes
            if (GetRandomValue(0, 180) == 0) {
                // Quick burst of speed
                speed *= 1.8f;
                // Speed will be normalized in the next movement
            }
            break;
            
        case ENEMY_BOSS:
            // Boss has same behavior as mummy but can be customized
            if (GetRandomValue(0, 180) == 0) {
                // Quick burst of speed like mummy
                speed *= 1.8f;
                // Speed will be normalized in the next movement
            }
            break;
    }
}

void Enemy::Draw() {
    if (!active) return;
    
    Color tint = WHITE;
    
    // Flash white when hit
    if (hitFlashTime > 0.0f) {
        tint = (int)(hitFlashTime * 10) % 2 == 0 ? RED : WHITE;
    }
    
    DrawTextureEx(frames[currentFrame], position, 0.0f, scale, tint);
}

Rectangle Enemy::GetCollisionRect() const {
    float width = frames[0].width * scale;
    float height = frames[0].height * scale;
    return { position.x, position.y, width, height };
}

void Enemy::MoveToward(Vector2 targetPos, float deltaTime) {
    if (!active) return;
    
    // Reset any temporary speed modifiers
    switch (type) {
        case ENEMY_ORC:
            // No special handling needed
            break;
        case ENEMY_OGRE:
            // Gradually restore original speed if it was reduced
            speed = speed * 0.9f + (100.0f * 0.8f) * 0.1f;
            break;
        case ENEMY_MUMMY:
            // Gradually restore original speed if it was increased
            speed = speed * 0.9f + (100.0f * 1.2f) * 0.1f;
            break;
        case ENEMY_BOSS:
            // Gradually restore original speed like mummy
            speed = speed * 0.9f + (100.0f * 1.2f) * 0.1f;
            break;
    }
    
    // Prioritize beaver target if chasing one
    Vector2 actualTarget = chasingBeaver ? beaverTarget : targetPos;
    
    Vector2 direction = {
        actualTarget.x - position.x,
        actualTarget.y - position.y
    };
    
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    }
    
    // Apply movement based on enemy type
    switch (type) {
        case ENEMY_ORC:
            // Direct movement toward target (player or beaver)
            position.x += direction.x * speed * deltaTime;
            position.y += direction.y * speed * deltaTime;
            break;
            
        case ENEMY_OGRE:
            // Slightly unpredictable movement
            position.x += (direction.x + GetRandomValue(-10, 10) / 100.0f) * speed * deltaTime;
            position.y += (direction.y + GetRandomValue(-10, 10) / 100.0f) * speed * deltaTime;
            break;
            
        case ENEMY_MUMMY:
            // More precise movement with occasional sidesteps
            if (GetRandomValue(0, 60) == 0) {
                // Sidestep perpendicular to target direction
                position.x += direction.y * speed * 0.5f * deltaTime;
                position.y -= direction.x * speed * 0.5f * deltaTime;
            } else {
                position.x += direction.x * speed * deltaTime;
                position.y += direction.y * speed * deltaTime;
            }
            break;
            
        case ENEMY_BOSS:
            // Boss moves only horizontally in a fixed path from spawn position
            // Ignore target position and move left/right based on current direction
            
            // Calculate distance from spawn position
            float distanceFromSpawn = position.x - bossSpawnPos.x;
            
            // Check if boss has reached movement limits and needs to change direction
            if (distanceFromSpawn >= bossMovementRange) {
                bossDirection = -1; // Move left
            } else if (distanceFromSpawn <= -bossMovementRange) {
                bossDirection = 1;  // Move right
            }
            
            // Move horizontally only (no vertical movement)
            position.x += bossDirection * speed * deltaTime;
            // Keep Y position fixed at spawn position
            position.y = bossSpawnPos.y;
            break;
    }
}

bool Enemy::IsOutOfBounds(float bgX, float bgY, float bgWidth, float bgHeight) const {
    return (position.x < bgX - 150 || 
            position.x > bgX + bgWidth + 150 ||
            position.y < bgY - 150 || 
            position.y > bgY + bgHeight + 150);
}

void Enemy::HandleObstacleCollision(Rectangle obstacle) {
    Rectangle enemyRect = GetCollisionRect();
    
    if (!CheckCollisionRecs(enemyRect, obstacle)) return;
    
    // Resolve collision by pushing enemy away from obstacle
    if (position.x + enemyRect.width > obstacle.x && position.x < obstacle.x) {
        position.x = obstacle.x - enemyRect.width;
    }
    if (position.x < obstacle.x + obstacle.width && position.x + enemyRect.width > obstacle.x + obstacle.width) {
        position.x = obstacle.x + obstacle.width;
    }
    if (position.y + enemyRect.height > obstacle.y && position.y < obstacle.y) {
        position.y = obstacle.y - enemyRect.height;
    }
    if (position.y < obstacle.y + obstacle.height && position.y + enemyRect.height > obstacle.y + obstacle.height) {
        position.y = obstacle.y + obstacle.height;
    }
}

EnemyType Enemy::GetType() const {
    return type;
}

bool Enemy::CanAppearInLevel(EnemyType type, int level) {
    switch (type) {
        case ENEMY_ORC:
            return level >= 1 && level <= 9; // Orcs appear in levels 1-9
        case ENEMY_OGRE:
            return level >= 2 && level <= 9; // Ogres appear in levels 2-9
        case ENEMY_MUMMY:
            return level >= 4 && level <= 9; // Mummies appear in levels 4-9
        case ENEMY_BOSS:
            return level == 10; // Boss only appears in level 10
        default:
            return false;
    }
}

bool Enemy::TakeDamage(int damage) {
    health -= damage;
    hitFlashTime = 0.2f; // Flash for 0.2 seconds when hit
    
    if (health <= 0) {
        return true; // Enemy dies
    }
    
    return false; // Enemy still alive
}

int Enemy::GetHealth() const {
    return health;
}

void Enemy::SetBeaverTarget(Vector2 beaverPos) {
    // Check if beaver is within chase range
    float distance = sqrtf((beaverPos.x - position.x) * (beaverPos.x - position.x) + 
                          (beaverPos.y - position.y) * (beaverPos.y - position.y));
    
    if (distance <= beaverChaseRange) {
        chasingBeaver = true;
        beaverTarget = beaverPos;
    }
}

void Enemy::ClearBeaverTarget() {
    chasingBeaver = false;
    beaverTarget = {0, 0};
}

bool Enemy::IsChasingBeaver() const {
    return chasingBeaver;
}

float Enemy::GetDistanceToBeaver() const {
    if (chasingBeaver) {
        float dx = beaverTarget.x - position.x;
        float dy = beaverTarget.y - position.y;
        return sqrtf(dx * dx + dy * dy);
    }
    return 0.0f;
}

bool Enemy::ShouldBossShoot(float deltaTime, Vector2 playerPos) {
    if (type != ENEMY_BOSS || !active) return false;
    
    bossShootTimer += deltaTime;
    if (bossShootTimer >= bossShootInterval) {
        bossShootTimer = 0.0f;
        return true;
    }
    
    return false;
}

Vector2 Enemy::GetBossShootDirection(Vector2 playerPos) const {
    if (type != ENEMY_BOSS || !active) return {0, 0};
    
    // Calculate direction from boss center to player
    Vector2 bossCenter = {
        position.x + (frames[0].width * scale) / 2,
        position.y + (frames[0].height * scale) / 2
    };
    
    Vector2 direction = {
        playerPos.x - bossCenter.x,
        playerPos.y - bossCenter.y
    };
    
    // Normalize the direction
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    }
    
    return direction;
}

bool Enemy::ShouldBossShootRain(float deltaTime, Vector2 playerPos) {
    if (type != ENEMY_BOSS || !active) return false;
    
    // Check if we're in the middle of a rail burst
    if (bossRailBurstActive) {
        bossRailBurstTimer += deltaTime;
        if (bossRailBurstTimer >= bossRailBurstDelay) {
            bossRailBurstTimer = 0.0f;
            bossRailBurstCount--;
            
            if (bossRailBurstCount <= 0) {
                bossRailBurstActive = false;
                return false;
            }
            return true; // Fire another bullet in the burst
        }
        return false;
    }
    
    // Check if it's time to start a new rail burst
    bossRailTimer += deltaTime;
    if (bossRailTimer >= bossRailInterval) {
        bossRailTimer = 0.0f;
        bossRailBurstActive = true;
        bossRailBurstCount = 20; // 10 consecutive bullets
        bossRailBurstTimer = 0.0f;
        return true; // Fire the first bullet
    }
    
    return false;
}


void Enemy::GetRainBulletDirections(Vector2 playerPos, Vector2* directions, int bulletCount) const {
    if (type != ENEMY_BOSS || !active || bulletCount <= 0) return;
    
    // Calculate direction from boss center to player (same direction for all bullets in rail)
    Vector2 bossCenter = {
        position.x + (frames[0].width * scale) / 2,
        position.y + (frames[0].height * scale) / 2
    };
    
    Vector2 baseDirection = {
        playerPos.x - bossCenter.x,
        playerPos.y - bossCenter.y
    };
    
    // Normalize the direction
    float length = sqrtf(baseDirection.x * baseDirection.x + baseDirection.y * baseDirection.y);
    if (length > 0) {
        baseDirection.x /= length;
        baseDirection.y /= length;
    }
    
    // All bullets go in the same direction (straight line rail gun)
    for (int i = 0; i < bulletCount; i++) {
        directions[i] = baseDirection;
    }
}