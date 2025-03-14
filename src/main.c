#include "raylib.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef enum GameState {
    MENU,
    PLAYING
} GameState;

GameState currentGameState = MENU;

Texture2D playerSprites[4];
Texture2D levelBackgrounds[2];
int currentBackground = 0;
float backgroundFrameTime = 0.5f;  // Tiempo entre cambios de fondo
float backgroundFrameCounter = 0.0f;

Vector2 playerPosition = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
int currentFrame = 1;  // Empieza con player2.png (posición quieta)
float frameTime = 0.2f;  // Tiempo entre cambios de sprite
float frameCounter = 0.0f;
float playerSpeed = 200.0f;
bool movingLeft = false;
bool movingRight = false;

Texture2D menuLogo;
Rectangle startButton = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 100, 200, 50 };

// Variables para las balas
#define MAX_BULLETS 100
Texture2D bulletTexture;
Vector2 bullets[MAX_BULLETS];
Vector2 bulletDirections[MAX_BULLETS]; // Dirección de cada bala
bool bulletActive[MAX_BULLETS]; // Para rastrear si una bala está activa
int bulletCount = 0;
float bulletSpeed = 400.0f;

void InitGame() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Journey of the Prairie King");
    SetTargetFPS(60);

    menuLogo = LoadTexture("resources/JOPK_logo.png");
    playerSprites[0] = LoadTexture("resources/player1.png");
    playerSprites[1] = LoadTexture("resources/player2.png");
    playerSprites[2] = LoadTexture("resources/player3.png");
    playerSprites[3] = LoadTexture("resources/player4.png");

    // Cargar las texturas del fondo del nivel 1
    levelBackgrounds[0] = LoadTexture("levels/Level_1.png");
    levelBackgrounds[1] = LoadTexture("levels/Level_1_2.png");

    // Cargar la textura de la bala
    bulletTexture = LoadTexture("resources/bullet/bullet.png");

    // Inicializar el estado de las balas
    for (int i = 0; i < MAX_BULLETS; i++) {
        bulletActive[i] = false;
    }
}

void UpdateGame() {
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

        float scale = 2.0f;
        float bgWidth = levelBackgrounds[currentBackground].width * scale;
        float bgHeight = levelBackgrounds[currentBackground].height * scale;
        float bgX = (SCREEN_WIDTH - bgWidth) / 2;
        float bgY = (SCREEN_HEIGHT - bgHeight) / 2;

        if (IsKeyDown(KEY_W) && playerPosition.y > bgY) {
            playerPosition.y -= playerSpeed * GetFrameTime();
            moving = true;
        }
        if (IsKeyDown(KEY_S) && playerPosition.y + playerSprites[currentFrame].height < bgY + bgHeight) {
            playerPosition.y += playerSpeed * GetFrameTime();
            moving = true;
        }
        if (IsKeyDown(KEY_A) && playerPosition.x > bgX) {
            playerPosition.x -= playerSpeed * GetFrameTime();
            moving = true;
        }
        if (IsKeyDown(KEY_D) && playerPosition.x + playerSprites[currentFrame].width < bgX + bgWidth) {
            playerPosition.x += playerSpeed * GetFrameTime();
            moving = true;
        }

        if (!moving) {
            currentFrame = 1;  // Si está quieto, usa player2.png
        }
        else {
            frameCounter += GetFrameTime();
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

        // Alternar entre los fondos para dar efecto de animación
        backgroundFrameCounter += GetFrameTime();
        if (backgroundFrameCounter >= backgroundFrameTime) {
            backgroundFrameCounter = 0.0f;
            currentBackground = (currentBackground + 1) % 2;
        }

        // Disparar balas con las flechas de dirección
        if (IsKeyPressed(KEY_UP)) {
            if (bulletCount < MAX_BULLETS) {
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!bulletActive[i]) {
                        bullets[i] = (Vector2){ playerPosition.x + playerSprites[currentFrame].width / 2 - bulletTexture.width / 2, playerPosition.y };
                        bulletDirections[i] = (Vector2){ 0, -1 }; // Disparar hacia arriba
                        bulletActive[i] = true;
                        bulletCount++;
                        break;
                    }
                }
            }
        }
        if (IsKeyPressed(KEY_DOWN)) {
            if (bulletCount < MAX_BULLETS) {
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!bulletActive[i]) {
                        bullets[i] = (Vector2){ playerPosition.x + playerSprites[currentFrame].width / 2 - bulletTexture.width / 2, playerPosition.y };
                        bulletDirections[i] = (Vector2){ 0, 1 }; // Disparar hacia abajo
                        bulletActive[i] = true;
                        bulletCount++;
                        break;
                    }
                }
            }
        }
        if (IsKeyPressed(KEY_LEFT)) {
            if (bulletCount < MAX_BULLETS) {
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!bulletActive[i]) {
                        bullets[i] = (Vector2){ playerPosition.x + playerSprites[currentFrame].width / 2 - bulletTexture.width / 2, playerPosition.y };
                        bulletDirections[i] = (Vector2){ -1, 0 }; // Disparar hacia la izquierda
                        bulletActive[i] = true;
                        bulletCount++;
                        break;
                    }
                }
            }
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            if (bulletCount < MAX_BULLETS) {
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!bulletActive[i]) {
                        bullets[i] = (Vector2){ playerPosition.x + playerSprites[currentFrame].width / 2 - bulletTexture.width / 2, playerPosition.y };
                        bulletDirections[i] = (Vector2){ 1, 0 }; // Disparar hacia la derecha
                        bulletActive[i] = true;
                        bulletCount++;
                        break;
                    }
                }
            }
        }

        // Actualizar posición de las balas
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bulletActive[i]) {
                bullets[i].x += bulletDirections[i].x * bulletSpeed * GetFrameTime();
                bullets[i].y += bulletDirections[i].y * bulletSpeed * GetFrameTime();

                // Eliminar balas que salen de la pantalla
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
        DrawTexture(menuLogo, (SCREEN_WIDTH - menuLogo.width) / 2, (SCREEN_HEIGHT - menuLogo.height) / 3, WHITE);
        DrawRectangleRec(startButton, WHITE);
        DrawText("START", startButton.x + 60, startButton.y + 15, 20, BLACK);
    }
    else if (currentGameState == PLAYING) {
        // Dibujar el fondo del nivel 1 animado y centrado
        float scale = 2.0f;
        Rectangle sourceRec = { 0, 0, levelBackgrounds[currentBackground].width, levelBackgrounds[currentBackground].height };
        Rectangle destRec = { (SCREEN_WIDTH - levelBackgrounds[currentBackground].width * scale) / 2,
                              (SCREEN_HEIGHT - levelBackgrounds[currentBackground].height * scale) / 2,
                              levelBackgrounds[currentBackground].width * scale,
                              levelBackgrounds[currentBackground].height * scale };
        Vector2 origin = { 0, 0 };
        DrawTexturePro(levelBackgrounds[currentBackground], sourceRec, destRec, origin, 0.0f, WHITE);

        // Dibujar al jugador sobre el fondo
        DrawTexture(playerSprites[currentFrame], playerPosition.x, playerPosition.y, WHITE);

        // Dibujar las balas
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bulletActive[i]) {
                DrawTexture(bulletTexture, bullets[i].x, bullets[i].y, WHITE);
            }
        }
    }

    EndDrawing();
}

void CloseGame() {
    UnloadTexture(menuLogo);
    for (int i = 0; i < 4; i++) {
        UnloadTexture(playerSprites[i]);
    }
    for (int i = 0; i < 2; i++) {
        UnloadTexture(levelBackgrounds[i]); // Liberar memoria de los fondos
    }
    UnloadTexture(bulletTexture); // Liberar memoria de la textura de la bala
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