#include "Player.h"
#include <raylib.h>

void Player::Init() {
    position = { 1920.0f / 2, 1080.0f / 2 };
    speed = 200.0f;
    frameCounter = 0.0f;
    frameTime = 0.2f;
    currentFrame = 1;
    movingLeft = false;
    movingRight = false;
    scale = 3.0f;
    shootAnimationTimer = 0.0f;
    shootAnimationDuration = 0.3f;
    shootDir = NONE;
    lives = 3;

    sprites[0] = LoadTexture("resources/player/player1.png");
    sprites[1] = LoadTexture("resources/player/player2.png");
    sprites[2] = LoadTexture("resources/player/player3.png");
    sprites[3] = LoadTexture("resources/player/player4.png");

    const char* shootPaths[12] = {
        "resources/player/move_left_shoot_down.png",
        "resources/player/move_left_shoot_left.png",
        "resources/player/move_left_shoot_right.png",
        "resources/player/move_left_shoot_up.png",
        "resources/player/move_right_shoot_down.png",
        "resources/player/move_right_shoot_left.png",
        "resources/player/move_right_shoot_right.png",
        "resources/player/move_right_shoot_up.png",
        "resources/player/stay_shoot_down.png",
        "resources/player/stay_shoot_left.png",
        "resources/player/stay_shoot_right.png",
        "resources/player/stay_shoot_up.png"
    };
    for (int i = 0; i < 12; ++i) {
        shootSprites[i] = LoadTexture(shootPaths[i]);
    }
}

void Player::Update(float deltaTime) {
    float bgScale = 3.8f;
    float bgWidth = 860 * bgScale;
    float bgHeight = 860 * bgScale;
    float bgX = (1920 - bgWidth) / 2;
    float bgY = (1080 - bgHeight) / 2;

    bool moving = false;
    if (IsKeyDown(KEY_A) && position.x > bgX) {
        position.x -= speed * deltaTime;
        movingLeft = true;
        movingRight = false;
        moving = true;
    }
    if (IsKeyDown(KEY_D) && position.x + sprites[0].width * scale < bgX + bgWidth) {
        position.x += speed * deltaTime;
        movingRight = true;
        movingLeft = false;
        moving = true;
    }
    if (IsKeyDown(KEY_W) && position.y > bgY) {
        position.y -= speed * deltaTime;
        moving = true;
    }
    if (IsKeyDown(KEY_S) && position.y + sprites[0].height * scale < bgY + bgHeight) {
        position.y += speed * deltaTime;
        moving = true;
    }

    if (moving) {
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
    }
    else {
        currentFrame = 1;
        movingLeft = false;
        movingRight = false;
    }

    if (shootAnimationTimer > 0.0f) {
        shootAnimationTimer -= deltaTime;
        if (shootAnimationTimer < 0.0f)
            shootAnimationTimer = 0.0f;
    }

    if (IsKeyPressed(KEY_UP)) { shootDir = UP;    shootAnimationTimer = shootAnimationDuration; }
    if (IsKeyPressed(KEY_DOWN)) { shootDir = DOWN;  shootAnimationTimer = shootAnimationDuration; }
    if (IsKeyPressed(KEY_LEFT)) { shootDir = LEFT;  shootAnimationTimer = shootAnimationDuration; }
    if (IsKeyPressed(KEY_RIGHT)) { shootDir = RIGHT; shootAnimationTimer = shootAnimationDuration; }
}

void Player::Draw() {
    DrawTextureEx(GetCurrentTexture(), position, 0.0f, scale, WHITE);
}

Texture2D Player::GetCurrentTexture() const {
    if (shootAnimationTimer > 0.0f) {
        if (movingLeft) {
            switch (shootDir) {
            case UP: return shootSprites[3];
            case DOWN: return shootSprites[0];
            case LEFT: return shootSprites[1];
            case RIGHT: return shootSprites[2];
            default: return sprites[currentFrame];
            }
        }
        else if (movingRight) {
            switch (shootDir) {
            case UP: return shootSprites[7];
            case DOWN: return shootSprites[4];
            case LEFT: return shootSprites[5];
            case RIGHT: return shootSprites[6];
            default: return sprites[currentFrame];
            }
        }
        else {
            switch (shootDir) {
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

Vector2 Player::GetPosition() const {
    return position;
}

void Player::SetPosition(Vector2 pos) {
    position = pos;
}

Rectangle Player::GetBounds() const {
    Texture2D tex = GetCurrentTexture();
    return { position.x, position.y, tex.width * scale, tex.height * scale };
}

int Player::GetLives() const {
    return lives;
}

void Player::DecreaseLife() {
    lives--;
    if (lives < 0) lives = 0;
}

bool Player::IsAlive() const {
    return lives > 0;
}

void Player::Reset() {
    position = { 1920.0f / 2, 1080.0f / 2 };
    currentFrame = 1;
    lives = 3;
    shootAnimationTimer = 0.0f;
    shootDir = NONE;
}

void Player::Unload() {
    for (int i = 0; i < 4; ++i) UnloadTexture(sprites[i]);
    for (int i = 0; i < 12; ++i) UnloadTexture(shootSprites[i]);
}
