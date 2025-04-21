#include "Player.h"
#include "raylib.h"
#include "ResourceManager.h"
#include "GameState.h"

void Player::Init(ResourceManager& resources) {
    sprites[0] = resources.GetTexture("player1");
    sprites[1] = resources.GetTexture("player2");
    sprites[2] = resources.GetTexture("player3");
    sprites[3] = resources.GetTexture("player4");
    hiTexture = resources.GetTexture("hi");

    shootSprites[0] = resources.GetTexture("shoot_down_left");
    shootSprites[1] = resources.GetTexture("shoot_left_left");
    shootSprites[2] = resources.GetTexture("shoot_right_left");
    shootSprites[3] = resources.GetTexture("shoot_up_left");
    shootSprites[4] = resources.GetTexture("shoot_down_right");
    shootSprites[5] = resources.GetTexture("shoot_left_right");
    shootSprites[6] = resources.GetTexture("shoot_right_right");
    shootSprites[7] = resources.GetTexture("shoot_up_right");
    shootSprites[8] = resources.GetTexture("shoot_down_idle");
    shootSprites[9] = resources.GetTexture("shoot_left_idle");
    shootSprites[10] = resources.GetTexture("shoot_right_idle");
    shootSprites[11] = resources.GetTexture("shoot_up_idle");
     
    position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    speed = 200.0f;
    scale = 3.0f;
    frameTime = 0.2f;
    frameCounter = 0.0f;
    currentFrame = 1;
    hiTimer = hiDuration;
    movingLeft = false;
    movingRight = false;
    lives = 3;
}

void Player::Update(float deltaTime) {
    if (hiTimer > 0) hiTimer -= deltaTime;

    movingLeft = false;
    movingRight = false;
    bool moving = false;

    if (IsKeyDown(KEY_W)) {
        position.y -= speed * deltaTime;
        moving = true;
    }
    if (IsKeyDown(KEY_S)) {
        position.y += speed * deltaTime;
        moving = true;
    }
    if (IsKeyDown(KEY_A)) {
        position.x -= speed * deltaTime;
        moving = true;
        movingLeft = true;
    }
    if (IsKeyDown(KEY_D)) {
        position.x += speed * deltaTime;
        moving = true;
        movingRight = true;
    }

    if (!moving) {
        currentFrame = 1;
    }
    else {
        frameCounter += deltaTime;
        if (frameCounter >= frameTime) {
            frameCounter = 0.0f;
            if (movingLeft) currentFrame = (currentFrame == 0) ? 1 : 0;
            else if (movingRight) currentFrame = (currentFrame == 2) ? 3 : 2;
            else currentFrame = (currentFrame == 1) ? 2 : 1;
        }
    }
}

void Player::Draw() {
    DrawTextureEx(GetCurrentTexture(), position, 0.0f, scale, WHITE);
}

Texture2D Player::GetCurrentTexture() const {
    if (hiTimer > 0) return hiTexture;
    return sprites[currentFrame];
}

Vector2 Player::GetPosition() const {
    return position;
}

int Player::Lives() const {
    return lives;
}

void Player::Reset() {
    position = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    currentFrame = 1;
    hiTimer = hiDuration; 
    lives = 3;
}