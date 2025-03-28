#include "raylib.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

typedef enum GameState {
    MENU,
    PLAYING
} GameState;

typedef enum ShootingDirection {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
} ShootingDirection;

GameState currentGameState = MENU;

// Texturas originales para caminar
Texture2D playerSprites[4];      // Sprites del jugador (player1.png, player2.png, etc.)

// Nuevas texturas para disparar
Texture2D moveLeftShootDown;
Texture2D moveLeftShootLeft;
Texture2D moveLeftShootRight;
Texture2D moveLeftShootUp;
Texture2D moveRightShootDown;
Texture2D moveRightShootLeft;
Texture2D moveRightShootRight;
Texture2D moveRightShootUp;
Texture2D stayShootDown;
Texture2D stayShootLeft;
Texture2D stayShootRight;
Texture2D stayShootUp;

Texture2D levelBackgrounds[2];
int currentBackground = 0;
float backgroundFrameTime = 0.5f;
float backgroundFrameCounter = 0.0f;

Vector2 playerPosition = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 };
int currentFrame = 1;            // Empieza con player2.png (posición quieta)
float frameTime = 0.2f;          // Tiempo entre cambios de sprite
float frameCounter = 0.0f;
float playerSpeed = 200.0f;
float timeRemaining = 60.0f;
float timeElapsed = 0.0f;
bool movingLeft = false;
bool movingRight = false;
float lastBulletTime = 0.0f;
float bulletCooldown = 0.2f;
ShootingDirection currentShootDirection = NONE;
float shootAnimationTimer = 0.0f;
float shootAnimationDuration = 0.3f; // Duración de la animación de disparo

Texture2D menuLogo;
Rectangle startButton = { SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 100, 200, 50 };

#define MAX_BULLETS 100
Texture2D bulletTexture;
Vector2 bullets[MAX_BULLETS];
Vector2 bulletDirections[MAX_BULLETS];
bool bulletActive[MAX_BULLETS];
int bulletCount = 0;
float bulletSpeed = 300.0f;

Rectangle sideObstacles[9];

void InitGame() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Journey of the Prairie King");
    ToggleFullscreen();
    SetTargetFPS(60);

    menuLogo = LoadTexture("resources/JOPK_logo.png");

    // Cargar las texturas originales de caminar
    playerSprites[0] = LoadTexture("resources/player1.png");
    playerSprites[1] = LoadTexture("resources/player2.png");
    playerSprites[2] = LoadTexture("resources/player3.png");
    playerSprites[3] = LoadTexture("resources/player4.png");

    // Cargar las nuevas texturas de disparo
    moveLeftShootDown = LoadTexture("resources/player/move_left_shoot_down.png");
    moveLeftShootLeft = LoadTexture("resources/player/move_left_shoot_left.png");
    moveLeftShootRight = LoadTexture("resources/player/move_left_shoot_right.png");
    moveLeftShootUp = LoadTexture("resources/player/move_left_shoot_up.png");
    moveRightShootDown = LoadTexture("resources/player/move_right_shoot_down.png");
    moveRightShootLeft = LoadTexture("resources/player/move_right_shoot_left.png");
    moveRightShootRight = LoadTexture("resources/player/move_right_shoot_right.png");
    moveRightShootUp = LoadTexture("resources/player/move_right_shoot_up.png");
    stayShootDown = LoadTexture("resources/player/stay_shoot_down.png");
    stayShootLeft = LoadTexture("resources/player/stay_shoot_left.png");
    stayShootRight = LoadTexture("resources/player/stay_shoot_right.png");
    stayShootUp = LoadTexture("resources/player/stay_shoot_up.png");

    levelBackgrounds[0] = LoadTexture("levels/Level_1.png");
    levelBackgrounds[1] = LoadTexture("levels/Level_1_2.png");

    bulletTexture = LoadTexture("resources/bullet/bullet.png");

    for (int i = 0; i < MAX_BULLETS; i++) {
        bulletActive[i] = false;
    }

    float bgX = (SCREEN_WIDTH - 860) / 2 - 100;
    float bgY = (SCREEN_HEIGHT - 860) / 2;
    float bgWidth = 860;
    float bgHeight = 860;
    float thickness = 50;
    float gapWidth = 50;

    sideObstacles[1] = (Rectangle){ bgX - thickness, bgY + bgHeight, bgWidth + 2 * thickness, thickness };
    sideObstacles[2] = (Rectangle){ bgX - thickness, bgY, thickness, bgHeight };
    sideObstacles[3] = (Rectangle){ bgX + bgWidth, bgY, thickness, bgHeight };
    sideObstacles[4] = (Rectangle){ bgX - thickness, bgY - thickness, thickness, thickness };
    sideObstacles[5] = (Rectangle){ bgX + bgWidth, bgY - thickness, thickness, thickness };
    sideObstacles[6] = (Rectangle){ bgX - thickness, bgY + bgHeight, thickness, thickness };
    sideObstacles[7] = (Rectangle){ bgX + bgWidth, bgY + bgHeight, thickness, thickness };

    float halfWidth = (bgWidth + 2 * thickness - gapWidth) / 2;

    sideObstacles[0] = (Rectangle){ bgX - thickness, bgY - thickness, halfWidth, thickness };
    sideObstacles[8] = (Rectangle){ bgX - thickness + halfWidth + gapWidth, bgY - thickness, halfWidth, thickness };
}

Texture2D GetCurrentPlayerTexture() {
    // Si hay animación de disparo activa, usar las nuevas texturas
    if (shootAnimationTimer > 0) {
        if (movingLeft) {
            switch (currentShootDirection) {
            case UP: return moveLeftShootUp;
            case DOWN: return moveLeftShootDown;
            case LEFT: return moveLeftShootLeft;
            case RIGHT: return moveLeftShootRight;
            default: return playerSprites[currentFrame]; // Por defecto, usar animación de caminar
            }
        }
        else if (movingRight) {
            switch (currentShootDirection) {
            case UP: return moveRightShootUp;
            case DOWN: return moveRightShootDown;
            case LEFT: return moveRightShootLeft;
            case RIGHT: return moveRightShootRight;
            default: return playerSprites[currentFrame]; // Por defecto, usar animación de caminar
            }
        }
        else {
            // Cuando está quieto y disparando
            switch (currentShootDirection) {
            case UP: return stayShootUp;
            case DOWN: return stayShootDown;
            case LEFT: return stayShootLeft;
            case RIGHT: return stayShootRight;
            default: return playerSprites[currentFrame]; // Por defecto, usar animación de caminar
            }
        }
    }

    // Si no está disparando, usar las texturas originales de caminar
    return playerSprites[currentFrame];
}

void UpdateGame() {
    timeElapsed += GetFrameTime();

    if (timeRemaining > 0) {
        timeRemaining -= GetFrameTime();
    }

    if (timeRemaining <= 0) {
        timeRemaining = 0;
    }

    // Actualizar temporizador de animación de disparo
    if (shootAnimationTimer > 0) {
        shootAnimationTimer -= GetFrameTime();
        if (shootAnimationTimer <= 0) {
            currentShootDirection = NONE;
        }
    }

    if (currentGameState == MENU) {
        if (IsKeyPressed(KEY_ENTER)) {
            currentGameState = PLAYING;
        }
    }
    else if (currentGameState == PLAYING) {
        bool moving = false;

        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_S)) {
            movingLeft = true;
            movingRight = false;
        }
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_W)) {
            movingRight = true;
            movingLeft = false;
        }

        float scale = 3.8f;
        float bgWidth = levelBackgrounds[currentBackground].width * scale;
        float bgHeight = levelBackgrounds[currentBackground].height * scale;

        float bgX = (SCREEN_WIDTH - bgWidth) / 2 - 100;
        float bgY = (SCREEN_HEIGHT - bgHeight) / 2;

        if (IsKeyDown(KEY_W) && playerPosition.y > bgY) {
            playerPosition.y -= playerSpeed * GetFrameTime();
            moving = true;
        }
        if (IsKeyDown(KEY_S) && playerPosition.y + GetCurrentPlayerTexture().height * 2.5f < bgY + bgHeight) {
            playerPosition.y += playerSpeed * GetFrameTime();
            moving = true;
        }
        if (IsKeyDown(KEY_A) && playerPosition.x > bgX) {
            playerPosition.x -= playerSpeed * GetFrameTime();
            moving = true;
        }
        if (IsKeyDown(KEY_D) && playerPosition.x + GetCurrentPlayerTexture().width * 2.5f < bgX + bgWidth) {
            playerPosition.x += playerSpeed * GetFrameTime();
            moving = true;
        }

        if (!moving) {
            currentFrame = 1;  // Si está quieto, usa player2.png
            movingLeft = false;
            movingRight = false;
        }
        else {
            frameCounter += GetFrameTime();
            if (frameCounter >= frameTime) {
                frameCounter = 0.0f;

                if (movingLeft) {
                    currentFrame = (currentFrame == 0) ? 1 : 0; // Alterna entre player1 y player2
                }
                else if (movingRight) {
                    currentFrame = (currentFrame == 2) ? 3 : 2; // Alterna entre player3 y player4
                }
            }
        }

        backgroundFrameCounter += GetFrameTime();
        if (backgroundFrameCounter >= backgroundFrameTime) {
            backgroundFrameCounter = 0.0f;
            currentBackground = (currentBackground + 1) % 2;
        }

        // Verificar colisión con los obstáculos
        Texture2D currentPlayerTex = GetCurrentPlayerTexture();
        float playerWidth = currentPlayerTex.width * 2.5f;
        float playerHeight = currentPlayerTex.height * 2.5f;

        for (int i = 0; i < 9; i++) {
            if (CheckCollisionRecs((Rectangle) { playerPosition.x, playerPosition.y, playerWidth, playerHeight }, sideObstacles[i])) {
                if (playerPosition.x + playerWidth > sideObstacles[i].x &&
                    playerPosition.x < sideObstacles[i].x) {
                    playerPosition.x = sideObstacles[i].x - playerWidth;
                }
                if (playerPosition.x < sideObstacles[i].x + sideObstacles[i].width &&
                    playerPosition.x + playerWidth > sideObstacles[i].x + sideObstacles[i].width) {
                    playerPosition.x = sideObstacles[i].x + sideObstacles[i].width;
                }
                if (playerPosition.y + playerHeight > sideObstacles[i].y &&
                    playerPosition.y < sideObstacles[i].y) {
                    playerPosition.y = sideObstacles[i].y - playerHeight;
                }
                if (playerPosition.y < sideObstacles[i].y + sideObstacles[i].height &&
                    playerPosition.y + playerHeight > sideObstacles[i].y + sideObstacles[i].height) {
                    playerPosition.y = sideObstacles[i].y + sideObstacles[i].height;
                }
                break;
            }
        }

        // Disparar balas
        if (GetTime() - lastBulletTime >= bulletCooldown) {
            if (bulletCount < MAX_BULLETS) {
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!bulletActive[i]) {
                        Vector2 bulletDir = { 0, 0 };
                        bool shouldShoot = false;

                        if (IsKeyPressed(KEY_UP)) {
                            bulletDir = (Vector2){ 0, -1 };
                            shouldShoot = true;
                            currentShootDirection = UP;
                            shootAnimationTimer = shootAnimationDuration;
                        }
                        if (IsKeyPressed(KEY_DOWN)) {
                            bulletDir = (Vector2){ 0, 1 };
                            shouldShoot = true;
                            currentShootDirection = DOWN;
                            shootAnimationTimer = shootAnimationDuration;
                        }
                        if (IsKeyPressed(KEY_LEFT)) {
                            bulletDir = (Vector2){ -1, 0 };
                            shouldShoot = true;
                            currentShootDirection = LEFT;
                            shootAnimationTimer = shootAnimationDuration;
                        }
                        if (IsKeyPressed(KEY_RIGHT)) {
                            bulletDir = (Vector2){ 1, 0 };
                            shouldShoot = true;
                            currentShootDirection = RIGHT;
                            shootAnimationTimer = shootAnimationDuration;
                        }

                        if (shouldShoot) {
                            float playerCenterX = playerPosition.x + GetCurrentPlayerTexture().width * 2.5f / 2;
                            float playerCenterY = playerPosition.y + GetCurrentPlayerTexture().height * 2.5f / 2;

                            bullets[i] = (Vector2){
                                playerCenterX - bulletTexture.width * 3.0f / 2 + bulletDir.x * 20,
                                playerCenterY - bulletTexture.height * 3.0f / 2 + bulletDir.y * 20
                            };

                            bulletDirections[i] = bulletDir;
                            bulletActive[i] = true;
                            bulletCount++;
                            lastBulletTime = GetTime();
                            break;
                        }
                    }
                }
            }
        }

        // Actualizar posición de las balas
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bulletActive[i]) {
                bullets[i].x += bulletDirections[i].x * bulletSpeed * GetFrameTime();
                bullets[i].y += bulletDirections[i].y * bulletSpeed * GetFrameTime();

                if (bullets[i].x < 0 || bullets[i].x > SCREEN_WIDTH || bullets[i].y < 0 || bullets[i].y > SCREEN_HEIGHT) {
                    bulletActive[i] = false;
                    bulletCount--;
                }
            }
        }
    }
}

void DrawGame() {
    BeginDrawing();
    ClearBackground(BLACK);

    if (currentGameState == MENU) {
        DrawTexture(menuLogo, (SCREEN_WIDTH - menuLogo.width) / 2 - 100, (SCREEN_HEIGHT - menuLogo.height) / 3, WHITE);
        DrawRectangleRec(startButton, WHITE);
        DrawText("START", startButton.x + 60, startButton.y + 15, 20, BLACK);
    }
    else if (currentGameState == PLAYING) {
        float progressBarWidth = 500.0f;
        float progressBarHeight = 10.0f;
        float timeProgress = (timeRemaining / 60.0f) * progressBarWidth;

        // Dibujar el fondo
        float scale = 3.8f;
        Rectangle sourceRec = { 0, 0, levelBackgrounds[currentBackground].width, levelBackgrounds[currentBackground].height };
        Rectangle destRec = {
            (SCREEN_WIDTH - levelBackgrounds[currentBackground].width * scale) / 2 - 100,
            (SCREEN_HEIGHT - levelBackgrounds[currentBackground].height * scale) / 2,
            levelBackgrounds[currentBackground].width * scale,
            levelBackgrounds[currentBackground].height * scale
        };
        Vector2 origin = { 0, 0 };
        DrawTexturePro(levelBackgrounds[currentBackground], sourceRec, destRec, origin, 0.0f, WHITE);

        // Dibujar al jugador
        DrawTextureEx(GetCurrentPlayerTexture(), playerPosition, 0.0f, 2.5f, WHITE);

        // Dibujar las balas
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bulletActive[i]) {
                DrawTextureEx(bulletTexture, bullets[i], 0.0f, 3.5f, WHITE);
            }
        }

        // Dibujar los rectángulos de colisión (debug)
        for (int i = 0; i < 9; i++) {
            DrawRectangleLinesEx(sideObstacles[i], 2, DARKGRAY);
        }
    }

    EndDrawing();
}

void CloseGame() {
    UnloadTexture(menuLogo);

    // Liberar texturas originales
    for (int i = 0; i < 4; i++) {
        UnloadTexture(playerSprites[i]);
    }

    // Liberar texturas de disparo
    UnloadTexture(moveLeftShootDown);
    UnloadTexture(moveLeftShootLeft);
    UnloadTexture(moveLeftShootRight);
    UnloadTexture(moveLeftShootUp);
    UnloadTexture(moveRightShootDown);
    UnloadTexture(moveRightShootLeft);
    UnloadTexture(moveRightShootRight);
    UnloadTexture(moveRightShootUp);
    UnloadTexture(stayShootDown);
    UnloadTexture(stayShootLeft);
    UnloadTexture(stayShootRight);
    UnloadTexture(stayShootUp);

    for (int i = 0; i < 2; i++) {
        UnloadTexture(levelBackgrounds[i]);
    }
    UnloadTexture(bulletTexture);
    CloseWindow();
}

int main() {
    InitGame();

    while (!WindowShouldClose()) {
        UpdateGame();
        DrawGame();
    }

    CloseGame();
    return 0;
}