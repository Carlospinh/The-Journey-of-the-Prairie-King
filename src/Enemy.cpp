#include "../include/Enemy.h"
#include <math.h>

// Initialize static members
Texture2D Enemy::orcFrames[2];
Texture2D Enemy::ogreFrames[2];
Texture2D Enemy::mummyFrames[2];

Enemy::Enemy() 
    : Entity(), 
    currentFrame(0),
    frameTimer(0.0f),
    frameDuration(0.2f),
    speed(100.0f),
    type(ENEMY_ORC),
    health(1),
    hitFlashTime(0.0f) {
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
}

void Enemy::UnloadSharedTextures() {
    // Unload textures for all enemy types
    UnloadTexture(orcFrames[0]);
    UnloadTexture(orcFrames[1]);
    
    UnloadTexture(ogreFrames[0]);
    UnloadTexture(ogreFrames[1]);
    
    UnloadTexture(mummyFrames[0]);
    UnloadTexture(mummyFrames[1]);
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
            this->health = 2;                // Ogres have 2 health points
            break;
        case ENEMY_MUMMY:
            this->speed = speed * 1.2f;      // Mummies are faster
            this->frameDuration = 0.15f;     // Faster animation
            this->health = 4;                // Mummies have 4 health points
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
    return Rectangle{ position.x, position.y, width, height };
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
    }
    
    Vector2 direction = {
        targetPos.x - position.x,
        targetPos.y - position.y
    };
    
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    }
    
    // Apply movement based on enemy type
    switch (type) {
        case ENEMY_ORC:
            // Direct movement toward player
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
                // Sidestep perpendicular to player direction
                position.x += direction.y * speed * 0.5f * deltaTime;
                position.y -= direction.x * speed * 0.5f * deltaTime;
            } else {
                position.x += direction.x * speed * deltaTime;
                position.y += direction.y * speed * deltaTime;
            }
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
            return level >= 1; // Orcs appear in levels 1-9
        case ENEMY_OGRE:
            return level >= 2; // Ogres appear in levels 2-9
        case ENEMY_MUMMY:
            return level >= 4; // Mummies appear in levels 4-9
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
