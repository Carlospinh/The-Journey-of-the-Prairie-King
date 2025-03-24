#include "raylib.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

typedef enum GameState {
    MENU,
    PLAYING
} GameState;

GameState currentGameState = MENU;

Texture2D playerSprites[4];      // Sprites del jugador
Texture2D levelBackgrounds[2];  // Fondos del nivel
int currentBackground = 0;
float backgroundFrameTime = 0.5f;  // Tiempo entre cambios de fondo
float backgroundFrameCounter = 0.0f;

Vector2 playerPosition = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 };
int currentFrame = 1;            // Empieza con player2.png (posición quieta)
float frameTime = 0.2f;          // Tiempo entre cambios de sprite
float frameCounter = 0.0f;
float playerSpeed = 200.0f;
float timeRemaining = 60.0f;     // Tiempo total en segundos para la barra (ejemplo: 60 segundos)
float timeElapsed = 0.0f;       // Contador de tiempo transcurrido
bool movingLeft = false;
bool movingRight = false;
float lastBulletTime = 0.0f;
float bulletCooldown = 0.2f;     // 1 segundo de delay entre disparos

Texture2D menuLogo;
Rectangle startButton = { SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 100, 200, 50 };

// Variables para las balas
#define MAX_BULLETS 100
Texture2D bulletTexture;
Vector2 bullets[MAX_BULLETS];
Vector2 bulletDirections[MAX_BULLETS];  // Dirección de cada bala
bool bulletActive[MAX_BULLETS];         // Para rastrear si una bala está activa
int bulletCount = 0;
float bulletSpeed = 300.0f;

// Definir los rectángulos de colisión de los obstáculos laterales
Rectangle sideObstacles[8];

void InitGame() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Journey of the Prairie King");
    ToggleFullscreen();  // Activa el modo pantalla completa
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

    float bgX = (SCREEN_WIDTH - 860) / 2 - 100;
    float bgY = (SCREEN_HEIGHT - 860) / 2;
    float bgWidth = 860;
    float bgHeight = 860;
    float thickness = 50;
    float gapWidth = 50; // Espacio entre los rectángulos superiores

    // Laterales y esquinas
    sideObstacles[1] = (Rectangle){ bgX - thickness, bgY + bgHeight, bgWidth + 2 * thickness, thickness }; // Abajo
    sideObstacles[2] = (Rectangle){ bgX - thickness, bgY, thickness, bgHeight }; // Izquierda
    sideObstacles[3] = (Rectangle){ bgX + bgWidth, bgY, thickness, bgHeight }; // Derecha
    sideObstacles[4] = (Rectangle){ bgX - thickness, bgY - thickness, thickness, thickness }; // Esquina superior izquierda
    sideObstacles[5] = (Rectangle){ bgX + bgWidth, bgY - thickness, thickness, thickness }; // Esquina superior derecha
    sideObstacles[6] = (Rectangle){ bgX - thickness, bgY + bgHeight, thickness, thickness }; // Esquina inferior izquierda
    sideObstacles[7] = (Rectangle){ bgX + bgWidth, bgY + bgHeight, thickness, thickness }; // Esquina inferior derecha

    // División superior en dos rectángulos dejando un espacio
    float halfWidth = (bgWidth + 2 * thickness - gapWidth) / 2;

    sideObstacles[0] = (Rectangle){ bgX - thickness, bgY - thickness, halfWidth, thickness }; // Superior izquierda
    sideObstacles[8] = (Rectangle){ bgX - thickness + halfWidth + gapWidth, bgY - thickness, halfWidth, thickness }; // Superior derecha


}

void UpdateGame() {
    // Actualizar el tiempo transcurrido
    timeElapsed += GetFrameTime();  // Se suma el tiempo de cada frame

    // Reducir el tiempo restante
    if (timeRemaining > 0) {
        timeRemaining -= GetFrameTime();
    }

    // Si el tiempo se acaba, puedes agregar alguna acción aquí (por ejemplo, reiniciar el juego o terminar el nivel)
    if (timeRemaining <= 0) {
        timeRemaining = 0;  // Asegura que no pase de 0
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

        float bgX = (SCREEN_WIDTH - bgWidth) / 2 - 100;// Centrar horizontalmente
        float bgY = (SCREEN_HEIGHT - bgHeight) / 2;// Centrar verticalmente

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

        // Verificar colisión con los obstáculos laterales
        for (int i = 0; i < 8; i++) {
            if (CheckCollisionRecs((Rectangle) { playerPosition.x, playerPosition.y, playerSprites[currentFrame].width, playerSprites[currentFrame].height }, sideObstacles[i])) {
                // Aquí aplicamos la corrección para evitar que el jugador atraviese los obstáculos
                // Detenemos el jugador dependiendo de la dirección de la colisión

                // Verificar colisión por la izquierda
                if (playerPosition.x + playerSprites[currentFrame].width > sideObstacles[i].x &&
                    playerPosition.x < sideObstacles[i].x) {
                    playerPosition.x = sideObstacles[i].x - playerSprites[currentFrame].width; // Empujar hacia la izquierda
                }

                // Verificar colisión por la derecha
                if (playerPosition.x < sideObstacles[i].x + sideObstacles[i].width &&
                    playerPosition.x + playerSprites[currentFrame].width > sideObstacles[i].x + sideObstacles[i].width) {
                    playerPosition.x = sideObstacles[i].x + sideObstacles[i].width; // Empujar hacia la derecha
                }

                // Verificar colisión por arriba
                if (playerPosition.y + playerSprites[currentFrame].height > sideObstacles[i].y &&
                    playerPosition.y < sideObstacles[i].y) {
                    playerPosition.y = sideObstacles[i].y - playerSprites[currentFrame].height; // Empujar hacia arriba
                }

                // Verificar colisión por abajo
                if (playerPosition.y < sideObstacles[i].y + sideObstacles[i].height &&
                    playerPosition.y + playerSprites[currentFrame].height > sideObstacles[i].y + sideObstacles[i].height) {
                    playerPosition.y = sideObstacles[i].y + sideObstacles[i].height; // Empujar hacia abajo
                }

                break;  // Detener la comprobación de colisiones adicionales para este obstáculo
            }
        }

        // Disparar balas con las flechas de dirección
        if (GetTime() - lastBulletTime >= bulletCooldown) {
            if (bulletCount < MAX_BULLETS) {
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!bulletActive[i]) {
                        Vector2 bulletDir = { 0, 0 };
                        bool shouldShoot = false;

                        if (IsKeyPressed(KEY_UP)) {
                            bulletDir = (Vector2){ 0, -1 };
                            shouldShoot = true;
                        }
                        if (IsKeyPressed(KEY_DOWN)) {
                            bulletDir = (Vector2){ 0, 1 };
                            shouldShoot = true;
                        }
                        if (IsKeyPressed(KEY_LEFT)) {
                            bulletDir = (Vector2){ -1, 0 };
                            shouldShoot = true;
                        }
                        if (IsKeyPressed(KEY_RIGHT)) {
                            bulletDir = (Vector2){ 1, 0 };
                            shouldShoot = true;
                        }

                        if (shouldShoot) {
                            float playerCenterX = playerPosition.x + playerSprites[currentFrame].width / 2;
                            float playerCenterY = playerPosition.y + playerSprites[currentFrame].height / 2;

                            bullets[i] = (Vector2){
                                playerCenterX - bulletTexture.width / 2 + bulletDir.x * 10,
                                playerCenterY - bulletTexture.height / 2 + bulletDir.y * 10
                            };

                            bulletDirections[i] = bulletDir;
                            bulletActive[i] = true;
                            bulletCount++;
                            lastBulletTime = GetTime();  // Se actualiza solo después de disparar  
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
        DrawTexture(menuLogo, (SCREEN_WIDTH - menuLogo.width) / 2 - 100, (SCREEN_HEIGHT - menuLogo.height) / 3, WHITE);
        DrawRectangleRec(startButton, WHITE);
        DrawText("START", startButton.x + 60, startButton.y + 15, 20, BLACK);
    }
    else if (currentGameState == PLAYING) {
        // Barra de tiempo
        float progressBarWidth = 500.0f;  // Ancho de la barra más pequeño (ajustado)
        float progressBarHeight = 10.0f;  // Altura de la barra más pequeña
        float timeProgress = (timeRemaining / 60.0f) * progressBarWidth;

        // Dibujar el fondo
// Dibujar el fondo
        float scale = 3.8f;  // Mantener la escala para que el fondo sea más grande
        Rectangle sourceRec = { 0, 0, levelBackgrounds[currentBackground].width, levelBackgrounds[currentBackground].height };
        Rectangle destRec = {
            (SCREEN_WIDTH - levelBackgrounds[currentBackground].width * scale) / 2 - 100, 
            (SCREEN_HEIGHT - levelBackgrounds[currentBackground].height * scale) / 2, // Centrar verticalmente
            levelBackgrounds[currentBackground].width * scale,
            levelBackgrounds[currentBackground].height * scale
        };
        Vector2 origin = { 0, 0 };
        DrawTexturePro(levelBackgrounds[currentBackground], sourceRec, destRec, origin, 0.0f, WHITE);


        // Dibujar al jugador
        DrawTextureEx(playerSprites[currentFrame], playerPosition, 0.0f, 1.5f, WHITE);

        // Dibujar las balas
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bulletActive[i]) {
                DrawTextureEx(bulletTexture, bullets[i], 0.0f, 3.0f, WHITE);
            }
        }

        // Dibujar los rectángulos de colisión (solo para debug, puedes eliminarlos si no es necesario)
        for (int i = 0; i < 8; i++) {
            DrawRectangleLinesEx(sideObstacles[i], 2, DARKGRAY);  // Dibujar bordes de los rectángulos de colisión
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
