#include "../include/Enemy.h"

// Initialize static member
Texture2D Enemy::sharedFrames[2];

Enemy::Enemy() 
    : Entity(), 
    currentFrame(0),
    frameTimer(0.0f),
    frameDuration(0.2f),
    speed(100.0f) {
}

Enemy::~Enemy() {
}

void Enemy::LoadSharedTextures() {
    sharedFrames[0] = LoadTexture("resources/Enemies/Orc/Orc1.png");
    sharedFrames[1] = LoadTexture("resources/Enemies/Orc/Orc2.png");
}

void Enemy::UnloadSharedTextures() {
    UnloadTexture(sharedFrames[0]);
    UnloadTexture(sharedFrames[1]);
}

void Enemy::Init(Vector2 position, float scale, float speed) {
    this->position = position;
    this->scale = scale;
    this->speed = speed;
    this->active = true;
    this->currentFrame = 0;
    this->frameTimer = 0.0f;
    
    // Copy shared textures
    this->frames[0] = sharedFrames[0];
    this->frames[1] = sharedFrames[1];
}

void Enemy::Update(float deltaTime) {
    if (!active) return;
    
    // Update animation
    frameTimer += deltaTime;
    if (frameTimer >= frameDuration) {
        frameTimer = 0.0f;
        currentFrame = (currentFrame + 1) % 2;
    }
}

void Enemy::Draw() {
    if (!active) return;
    
    DrawTextureEx(frames[currentFrame], position, 0.0f, scale, WHITE);
}

Rectangle Enemy::GetCollisionRect() const {
    float width = frames[0].width * scale;
    float height = frames[0].height * scale;
    return Rectangle{ position.x, position.y, width, height };
}

void Enemy::MoveToward(Vector2 targetPos, float deltaTime) {
    if (!active) return;
    
    Vector2 direction = {
        targetPos.x - position.x,
        targetPos.y - position.y
    };
    
    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    }
    
    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;
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
