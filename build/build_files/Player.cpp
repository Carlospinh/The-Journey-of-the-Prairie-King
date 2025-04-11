#include "Player.h"
#include "Constants.h"

Player::Player(Vector2 pos, float spd) :
    Entity(pos, spd, 100.0f),
    currentFrame(1), frameTime(0.2f), frameCounter(0.0f),
    movingLeft(false), movingRight(false),
    lastBulletTime(0.0f), bulletCooldown(0.2f) {

    sprites[0] = LoadTexture("resources/player1.png");
    sprites[1] = LoadTexture("resources/player2.png");
    sprites[2] = LoadTexture("resources/player3.png");
    sprites[3] = LoadTexture("resources/player4.png");

    SetTexture(sprites[currentFrame]);
}

Player::~Player() {
    for (int i = 0; i < 4; i++) {
        UnloadTexture(sprites[i]);
    }
}

void Player::Update(float deltaTime) {
    frameCounter += deltaTime;
    if (frameCounter >= frameTime) {
        frameCounter = 0.0f;
        if (movingLeft) {
            currentFrame = (currentFrame == 0) ? 1 : 0;
        }
        else if (movingRight) {
            currentFrame = (currentFrame == 2) ? 3 : 2;
        }
    }
    collisionBox.x = position.x;
    collisionBox.y = position.y;
}

void Player::Draw() const {
    DrawTextureEx(sprites[currentFrame], position, 0.0f, 1.5f, WHITE);
}

void Player::HandleInput(float deltaTime) {
    movingLeft = (IsKeyDown(KEY_A) || IsKeyDown(KEY_S));
    movingRight = (IsKeyDown(KEY_D) || IsKeyDown(KEY_W));
}

bool Player::CanShoot() const {
    return (GetTime() - lastBulletTime >= bulletCooldown);
}

void Player::ResetShootTimer() {
    lastBulletTime = GetTime();
}

int Player::GetCurrentFrame() const {
    return currentFrame;
}

Texture2D Player::GetCurrentTexture() const {
    return sprites[currentFrame];
}