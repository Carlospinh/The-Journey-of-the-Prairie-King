#include "raylib.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include <sstream>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

typedef enum GameState {
    INTRO_1,
    INTRO_2,
    INTRO_3,
    MENU,
    PLAYING,
    GAME_OVER,
    LEVEL_COMPLETED
} GameState;

typedef enum ShootingDirection {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
} ShootingDirection;

GameState currentGameState = INTRO_1;
float introTimer = 0.0f;
float introDuration = 5.0f;
float fadeAlpha = 0.0f;
bool fadingIn = true;
bool fadingOut = false;

Texture2D playerSprites[4];
Texture2D shootSprites[12];
Texture2D levelBackgrounds[2];
int currentBackground = 0;
float backgroundFrameTime = 0.5f;
float backgroundFrameCounter = 0.0f;
Vector2 playerPosition = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
int currentFrame = 1;
float frameTime = 0.2f;
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
float shootAnimationDuration = 0.3f;
Texture2D menuLogo;
Rectangle startButton = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 100, 200, 50 };
#define MAX_BULLETS 100
Texture2D bulletTexture;
Vector2 bullets[MAX_BULLETS];
Vector2 bulletDirections[MAX_BULLETS];
bool bulletActive[MAX_BULLETS];
int bulletCount = 0;
float bulletSpeed = 300.0f;
Rectangle sideObstacles[9];
float playerScale = 3.0f;
float bulletScale = 3.5f;
#define MAX_ENEMIES 20
#define MAX_COINS 50
#define MAX_POWERUPS 10
bool hasWheelPowerUp = false;  // Añade esto con las otras variables globales
bool wheelPowerUpActive = false;
float wheelPowerUpDuration = 12.0f;
float wheelPowerUpTimer = 0.0f;
float wheelShootCooldown = 0.5f;
float wheelShootTimer = 0.0f;
bool wheelPowerUpOnField = false; // Añadir con las otras variables globales
float wheelDropCooldown = 0.0f; // Añadir con las otras variables globales
Texture2D powerUpCartelTexture; // Textura del fondo de power-ups
Texture2D clockTexture;
Texture2D levelCompletedBackgrounds[2]; // Añade esto con las otras variables de textura
Texture2D arrowTexture; // Añade esto con las otras variables de textura
float arrowBlinkTimer = 0.0f;
float arrowBlinkInterval = 0.5f; // Medio segundo
bool arrowVisible = false;
bool levelTransition = false;
float transitionY = 0.0f;
float transitionSpeed = 500.0f;
Texture2D level2Texture;
Texture2D level2Texture2;
float transitionDuration = 1.5f; // 1.5 segundos para la transición
float transitionProgress = 0.0f; // Progreso de la transición (0 a 1)
Texture2D playerHiTexture;
float hiTimer = 0.0f;
const float hiDuration = 1.0f; // 1 segundo

void ResetGame();
void DrawGameOver();

typedef struct {
    Vector2 position;
    float speed;
    bool active;
    int currentFrame;
    float frameTimer;
    Texture2D frames[2];
    float scale;
} Enemy;

typedef struct {
    Vector2 position;
    bool active;
    float scale;
} Coin;

typedef struct {
    Vector2 position;
    bool active;
    float scale;
    Texture2D texture;
} PowerUp;

Enemy enemies[MAX_ENEMIES];
Coin coins[MAX_COINS];
PowerUp powerUps[MAX_POWERUPS];
float enemySpawnTimer = 0.0f;
float enemySpawnInterval = 2.0f;
Texture2D orcFrames[2];
Texture2D coinTexture;
Texture2D wheelTexture;
int enemiesKilled = 0;
int coinsCollected = 0;
int playerLives = 3;
Texture2D liveTexture;

Texture2D introScreen1;
Texture2D introScreen2;
Texture2D introScreen3;
Sound startSound;
Sound footstepSound;
Sound gunshotSound;
Sound hitSound;
Sound coinSound;
Music backgroundMusic;
bool isMusicPlaying = false;
float footstepCooldown = 0.0f;
float footstepDelay = 0.3f;
Sound pickupPowerUpSound;  // Añade esto con las otras variables de sonido

void LoadIntroResources() {
    introScreen1 = LoadTexture("resources/StartScreens/screen1.png");
    introScreen2 = LoadTexture("resources/StartScreens/screen2.png");
    introScreen3 = LoadTexture("resources/StartScreens/screen3.png");
}

void UnloadIntroResources() {
    UnloadTexture(introScreen1);
    UnloadTexture(introScreen2);
    UnloadTexture(introScreen3);
}

void UpdateIntro() {
    float deltaTime = GetFrameTime();

    if (fadingIn) {
        fadeAlpha += deltaTime * 2.0f;
        if (fadeAlpha >= 1.0f) {
            fadeAlpha = 1.0f;
            fadingIn = false;
        }
    }

    introTimer += deltaTime;

    if (introTimer >= introDuration - 1.0f && !fadingOut) {
        fadingOut = true;
    }

    if (fadingOut) {
        fadeAlpha -= deltaTime * 2.0f;
        if (fadeAlpha <= 0.0f) {
            fadeAlpha = 0.0f;
            fadingOut = false;

            if (currentGameState == INTRO_1) {
                currentGameState = INTRO_2;
            }
            else if (currentGameState == INTRO_2) {
                currentGameState = INTRO_3;
            }
            else if (currentGameState == INTRO_3) {
                currentGameState = MENU;
                SetSoundVolume(startSound, 1.0f);
                PlaySound(startSound);
            }

            introTimer = 0.0f;
            fadingIn = true;
        }
    }
}

void DrawIntroScreen1() {
    const char* introText = "Hi! This is our remake of Journey of the Prairie King, the arcade minigame\n"
        "featured in Stardew Valley. We've recreated it from scratch using Raylib\n"
        "as a standalone project to learn more about game development and pay\n"
        "tribute to this fun classic. Enjoy!";

    Vector2 textSize = MeasureTextEx(GetFontDefault(), introText, 30, 1);
    DrawText(introText, (SCREEN_WIDTH - textSize.x) / 2 - 100, SCREEN_HEIGHT / 4, 30, Fade(WHITE, fadeAlpha));

    float imageScale = 0.7f;
    float imageWidth = introScreen1.width * imageScale;
    float imageHeight = introScreen1.height * imageScale;
    DrawTextureEx(introScreen1,
        Vector2 {
        (SCREEN_WIDTH - imageWidth) / 2 + 20, SCREEN_HEIGHT / 2 - 200
    },
        0.0f, imageScale, Fade(WHITE, fadeAlpha));
}

void DrawIntroScreen2() {
    const char* title = "Our Team and Mentor";
    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, 40, 1);
    DrawText(title, (SCREEN_WIDTH - titleSize.x) / 2, SCREEN_HEIGHT / 8, 40, Fade(WHITE, fadeAlpha));

    float imageScale = 0.7f;
    float imageWidth = introScreen2.width * imageScale;
    float imageHeight = introScreen2.height * imageScale;
    DrawTextureEx(introScreen2,
        Vector2 {
        (SCREEN_WIDTH - imageWidth) / 2, SCREEN_HEIGHT / 2 - 300
    },
        0.0f, imageScale, Fade(WHITE, fadeAlpha));
}

void DrawIntroScreen3() {
    const char* title = "Follow Our Journey On...";
    const char* githubText = "github.com/Carlospinh/The-Journey-of-the-Prairie-King";

    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, 40, 1);
    DrawText(title, (SCREEN_WIDTH - titleSize.x) / 2 - 100, SCREEN_HEIGHT / 4, 40, Fade(WHITE, fadeAlpha));

    Vector2 textSize = MeasureTextEx(GetFontDefault(), githubText, 30, 1);
    DrawText(githubText, (SCREEN_WIDTH - textSize.x) / 2, SCREEN_HEIGHT / 3 + 50, 30, Fade(WHITE, fadeAlpha));

    float imageScale = 0.7f;
    float imageWidth = introScreen3.width * imageScale;
    float imageHeight = introScreen3.height * imageScale;
    DrawTextureEx(introScreen3,
        Vector2 {
        (SCREEN_WIDTH - imageWidth) / 2, SCREEN_HEIGHT / 2 - 200
    },
        0.0f, imageScale, Fade(WHITE, fadeAlpha));
}

void ShootCircle() {
    if (bulletCount >= MAX_BULLETS - 8) return; // No disparar si no hay espacio para 8 balas

    float playerCenterX = playerPosition.x + (playerSprites[0].width * playerScale) / 2;
    float playerCenterY = playerPosition.y + (playerSprites[0].height * playerScale) / 2;

    // Disparar 8 balas en direcciones diferentes (45 grados entre cada una)
    for (int i = 0; i < 8; i++) {
        float angle = i * 45 * DEG2RAD;
        Vector2 direction = { cosf(angle), sinf(angle) };

        for (int j = 0; j < MAX_BULLETS; j++) {
            if (!bulletActive[j]) {
                bullets[j] = Vector2{
                    playerCenterX - bulletTexture.width * bulletScale / 2,
                    playerCenterY - bulletTexture.height * bulletScale / 2
                };
                bulletDirections[j] = direction;
                bulletActive[j] = true;
                bulletCount++;
                break;
            }
        }
    }
    PlaySound(gunshotSound);
}

bool CheckBulletEnemyCollision(Vector2 bulletPos, Enemy enemy) {
    float bulletWidth = bulletTexture.width * bulletScale;
    float bulletHeight = bulletTexture.height * bulletScale;

    float enemyWidth = enemy.frames[0].width * enemy.scale;
    float enemyHeight = enemy.frames[0].height * enemy.scale;

    Rectangle bulletRect = { bulletPos.x, bulletPos.y, bulletWidth, bulletHeight };
    Rectangle enemyRect = { enemy.position.x, enemy.position.y, enemyWidth, enemyHeight };

    return CheckCollisionRecs(bulletRect, enemyRect);
}

bool CheckPlayerCoinCollision(Vector2 playerPos, Coin coin) {
    float playerWidth = playerSprites[0].width * playerScale;
    float playerHeight = playerSprites[0].height * playerScale;
    float coinWidth = coinTexture.width * coin.scale;
    float coinHeight = coinTexture.height * coin.scale;

    Rectangle playerRect = { playerPos.x, playerPos.y, playerWidth, playerHeight };
    Rectangle coinRect = { coin.position.x, coin.position.y, coinWidth, coinHeight };

    return CheckCollisionRecs(playerRect, coinRect);
}

bool CheckPlayerPowerUpCollision(Vector2 playerPos, PowerUp powerUp) {
    float playerWidth = playerSprites[0].width * playerScale;
    float playerHeight = playerSprites[0].height * playerScale;
    float powerUpWidth = powerUp.texture.width * powerUp.scale;
    float powerUpHeight = powerUp.texture.height * powerUp.scale;

    Rectangle playerRect = { playerPos.x, playerPos.y, playerWidth, playerHeight };
    Rectangle powerUpRect = { powerUp.position.x, powerUp.position.y, powerUpWidth, powerUpHeight };

    return CheckCollisionRecs(playerRect, powerUpRect);
}

void SpawnEnemy() {
    float scale = 3.8f;
    float bgWidth = levelBackgrounds[currentBackground].width * scale;
    float bgHeight = levelBackgrounds[currentBackground].height * scale;
    float bgX = (SCREEN_WIDTH - bgWidth) / 2;
    float bgY = (SCREEN_HEIGHT - bgHeight) / 2;

    // Zonas de spawn ligeramente amplias (40%-60% del área)
    Vector2 spawnPositions[] = {
        // Izquierda (zona vertical: 40% a 60% de altura)
        { bgX + 15, bgY + bgHeight * 0.4f + (float)(rand() % (int)(bgHeight * 0.2f)) },
        // Derecha (zona vertical: 40% a 60% de altura)
        { bgX + bgWidth - 15, bgY + bgHeight * 0.4f + (float)(rand() % (int)(bgHeight * 0.2f)) },
        // Arriba (zona horizontal: 40% a 60% de ancho)
        { bgX + bgWidth * 0.4f + (float)(rand() % (int)(bgWidth * 0.2f)), bgY + 15 },
        // Abajo (zona horizontal: 40% a 60% de ancho)
        { bgX + bgWidth * 0.4f + (float)(rand() % (int)(bgWidth * 0.2f)), bgY + bgHeight - 15 }
    };

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            int spawnIndex = rand() % 4;
            enemies[i].position = spawnPositions[spawnIndex];

            enemies[i].active = true;
            enemies[i].currentFrame = 0;
            enemies[i].frameTimer = 0.0f;
            enemies[i].scale = 0.2f;
            enemies[i].speed = 100.0f + (rand() % 50);
            break;
        }
    }
}

void SpawnCoin(Vector2 position) {
    for (int i = 0; i < MAX_COINS; i++) {
        if (!coins[i].active) {
            coins[i].position = position;
            coins[i].active = true;
            coins[i].scale = 0.4f;
            break;
        }
    }
}

void SpawnPowerUp(Vector2 position) {
    // No spawnear si ya hay uno en campo, o el jugador tiene uno, o está activo el efecto
    if (wheelPowerUpOnField || hasWheelPowerUp || wheelPowerUpActive) return;

    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!powerUps[i].active) {
            powerUps[i].position = position;
            powerUps[i].active = true;
            powerUps[i].scale = 0.2f;
            powerUps[i].texture = wheelTexture;
            wheelPowerUpOnField = true;
            break;
        }
    }
}

Texture2D GetCurrentPlayerTexture() {

    if (hiTimer > 0) {
        return playerHiTexture;
    }

    // Si hay animación de disparo activa, usar las texturas de disparo
    if (shootAnimationTimer > 0) {
        if (movingLeft) {
            switch (currentShootDirection) {
            case UP: return shootSprites[3];    // move_left_shoot_up
            case DOWN: return shootSprites[0];  // move_left_shoot_down
            case LEFT: return shootSprites[1];  // move_left_shoot_left
            case RIGHT: return shootSprites[2]; // move_left_shoot_right
            default: return playerSprites[currentFrame];
            }
        }
        else if (movingRight) {
            switch (currentShootDirection) {
            case UP: return shootSprites[7];    // move_right_shoot_up
            case DOWN: return shootSprites[4];  // move_right_shoot_down
            case LEFT: return shootSprites[5];  // move_right_shoot_left
            case RIGHT: return shootSprites[6]; // move_right_shoot_right
            default: return playerSprites[currentFrame];
            }
        }
        else {
            // Cuando está quieto y disparando
            switch (currentShootDirection) {
            case UP: return shootSprites[11];   // stay_shoot_up
            case DOWN: return shootSprites[8];  // stay_shoot_down
            case LEFT: return shootSprites[9];  // stay_shoot_left
            case RIGHT: return shootSprites[10]; // stay_shoot_right
            default: return playerSprites[currentFrame];
            }
        }
    }

    // Si no está disparando, usar las texturas originales de caminar
    return playerSprites[currentFrame];
}

void UpdateEnemies() {
    float scale = 3.8f;
    float bgWidth = levelBackgrounds[currentBackground].width * scale;
    float bgHeight = levelBackgrounds[currentBackground].height * scale;
    float bgX = (SCREEN_WIDTH - bgWidth) / 2;
    float bgY = (SCREEN_HEIGHT - bgHeight) / 2;

    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            // Calcular dirección hacia el jugador en cada frame
            Vector2 direction = {
                playerPosition.x - enemies[i].position.x,
                playerPosition.y - enemies[i].position.y
            };

            // Normalizar la dirección
            float length = sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length > 0) {
                direction.x /= length;
                direction.y /= length;
            }

            // Mover al enemigo
            enemies[i].position.x += direction.x * enemies[i].speed * GetFrameTime();
            enemies[i].position.y += direction.y * enemies[i].speed * GetFrameTime();

            enemies[i].frameTimer += GetFrameTime();
            if (enemies[i].frameTimer >= 0.2f) {
                enemies[i].frameTimer = 0.0f;
                enemies[i].currentFrame = (enemies[i].currentFrame + 1) % 2;
            }

            // Desactivar si salen del área de juego
            if (enemies[i].position.x < bgX - 150 || enemies[i].position.x > bgX + bgWidth + 150 ||
                enemies[i].position.y < bgY - 150 || enemies[i].position.y > bgY + bgHeight + 150) {
                enemies[i].active = false;
            }
        }
    }
}

void UpdateCoins() {
    float scale = 3.8f;
    float bgWidth = levelBackgrounds[currentBackground].width * scale;
    float bgHeight = levelBackgrounds[currentBackground].height * scale;
    float bgX = (SCREEN_WIDTH - bgWidth) / 2;
    float bgY = (SCREEN_HEIGHT - bgHeight) / 2;

    for (int i = 0; i < MAX_COINS; i++) {
        if (coins[i].active) {
            // Verificar colisión con el jugador
            if (CheckPlayerCoinCollision(playerPosition, coins[i])) {
                coins[i].active = false;
                coinsCollected++;
                PlaySound(coinSound);
            }

            // Verificar si la moneda está fuera del área de juego
            if (coins[i].position.x < bgX || coins[i].position.x > bgX + bgWidth ||
                coins[i].position.y < bgY || coins[i].position.y > bgY + bgHeight) {
                coins[i].active = false;
            }
        }
    }
}

void UpdatePowerUps() {
    float scale = 3.8f;
    float bgWidth = levelBackgrounds[currentBackground].width * scale;
    float bgHeight = levelBackgrounds[currentBackground].height * scale;
    float bgX = (SCREEN_WIDTH - bgWidth) / 2;
    float bgY = (SCREEN_HEIGHT - bgHeight) / 2;

    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (powerUps[i].active) {
            if (CheckPlayerPowerUpCollision(playerPosition, powerUps[i])) {
                powerUps[i].active = false;
                hasWheelPowerUp = true;
                wheelPowerUpOnField = false;
                PlaySound(pickupPowerUpSound);  // Cambiado de coinSound a pickupPowerUpSound
            }
            else if (powerUps[i].position.x < bgX || powerUps[i].position.x > bgX + bgWidth ||
                powerUps[i].position.y < bgY || powerUps[i].position.y > bgY + bgHeight) {
                powerUps[i].active = false;
                wheelPowerUpOnField = false;
            }
        }
    }
}

void UpdateGame() {
    float deltaTime = GetFrameTime();

    // Actualizar temporizador de animación "hi"
    if (currentGameState == PLAYING && hiTimer > 0) {
        hiTimer -= deltaTime;
    }

    switch (currentGameState) {
    case INTRO_1:
    case INTRO_2:
    case INTRO_3:
        UpdateIntro();
        break;

    case MENU:
        if (IsKeyPressed(KEY_ENTER)) {
            currentGameState = PLAYING;
            PlayMusicStream(backgroundMusic);
            SetMusicVolume(backgroundMusic, 0.5f);
            isMusicPlaying = true;
        }
        break;

    case PLAYING:
        if (isMusicPlaying) {
            UpdateMusicStream(backgroundMusic);
        }

        // Verificar si el jugador perdió todas las vidas
        if (playerLives <= 0) {
            currentGameState = GAME_OVER;
            break;
        }

        // Actualización del tiempo del juego
        timeElapsed += deltaTime;
        if (timeRemaining > 0) {
            timeRemaining -= deltaTime;
        }
        else {
            timeRemaining = 0;

            // Verificar si estamos en el nivel 2 (completado)
            if (levelBackgrounds[0].id == level2Texture.id) {
                currentGameState = LEVEL_COMPLETED;
                break;
            }


            // Cambiar a los fondos de nivel completado solo si no estamos en transición
            if (!levelTransition) {
                levelBackgrounds[0] = levelCompletedBackgrounds[0];
                levelBackgrounds[1] = levelCompletedBackgrounds[1];
            }

            // Verificar si el jugador está en la zona de spawn inferior para iniciar transición
            float bgScale = 3.8f;
            float bgWidth = levelBackgrounds[currentBackground].width * bgScale;
            float bgHeight = levelBackgrounds[currentBackground].height * bgScale;
            float bgX = (SCREEN_WIDTH - bgWidth) / 2;
            float bgY = (SCREEN_HEIGHT - bgHeight) / 2;

            // Verificar si el jugador está en la zona de spawn inferior (40%-60% del ancho, parte inferior)
            float playerWidth = playerSprites[0].width * playerScale;
            float playerHeight = playerSprites[0].height * playerScale;
            Rectangle playerRect = { playerPosition.x, playerPosition.y, playerWidth, playerHeight };
            Rectangle spawnZone = {
                bgX + bgWidth * 0.4f,
                bgY + bgHeight - 50,
                bgWidth * 0.2f,
                50
            };

            if (CheckCollisionRecs(playerRect, spawnZone)) {
                levelTransition = true;
                transitionY = bgY + bgHeight; // Comenzar desde abajo del fondo (cambio aquí)
            }

            // Actualizar el parpadeo de la flecha
            arrowBlinkTimer += deltaTime;
            if (arrowBlinkTimer >= arrowBlinkInterval) {
                arrowBlinkTimer = 0.0f;
                arrowVisible = !arrowVisible;
            }
        }

        // Actualizar la animación de transición si está activa
        if (levelTransition) {
            transitionProgress += deltaTime / transitionDuration;

            // Suavizar el movimiento con una curva de ease-out
            float easedProgress = 1.0f - powf(1.0f - transitionProgress, 3);

            // Calcular la posición Y basada en el progreso suavizado
            float bgHeight = levelBackgrounds[currentBackground].height * 3.8f;
            float bgY = (SCREEN_HEIGHT - bgHeight) / 2;
            transitionY = bgY + bgHeight - (bgHeight * easedProgress);

            // Cuando la animación termina
            if (transitionProgress >= 1.0f) {
                levelTransition = false;
                transitionProgress = 0.0f;
                // Cambiar definitivamente al nivel 2
                levelBackgrounds[0] = level2Texture;
                levelBackgrounds[1] = level2Texture2;
                // Resetear otros valores del juego para el nuevo nivel
                timeRemaining = 60.0f;
                // Limpiar enemigos, balas, etc.
                for (int i = 0; i < MAX_ENEMIES; i++) enemies[i].active = false;
                for (int i = 0; i < MAX_BULLETS; i++) bulletActive[i] = false;
                bulletCount = 0;
                // Resetear posición del jugador
                playerPosition = Vector2{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
            }
        }

        // Manejo del power-up Wheel
        if (hasWheelPowerUp && IsKeyPressed(KEY_SPACE) && !wheelPowerUpActive && !levelTransition) {
            wheelPowerUpActive = true;
            wheelPowerUpTimer = wheelPowerUpDuration;
            wheelShootTimer = 0.0f;
            hasWheelPowerUp = false;
        }

        // Actualización del power-up activo
        if (wheelPowerUpActive) {
            wheelPowerUpTimer -= deltaTime;
            wheelShootTimer -= deltaTime;

            // Disparar en círculo cada segundo
            if (wheelShootTimer <= 0) {
                ShootCircle();
                wheelShootTimer = wheelShootCooldown;
            }

            // Desactivar cuando termine el tiempo
            if (wheelPowerUpTimer <= 0) {
                wheelPowerUpActive = false;
            }
        }

        // Actualización de enemigos (solo si no estamos en transición)
        if (timeRemaining > 0 && !levelTransition) {
            enemySpawnTimer += deltaTime;
            if (enemySpawnTimer >= enemySpawnInterval) {
                enemySpawnTimer = 0.0f;
                SpawnEnemy();
            }
        }

        // Actualizar entidades del juego (solo si no estamos en transición)
        if (!levelTransition) {
            UpdateEnemies();
            UpdateCoins();
            UpdatePowerUps();
        }

        // Verificación de colisión jugador-enemigo (solo si no estamos en transición)
        if (!levelTransition) {
            Texture2D currentTex = GetCurrentPlayerTexture();
            float playerWidth = currentTex.width * playerScale;
            float playerHeight = currentTex.height * playerScale;
            Rectangle playerRect = { playerPosition.x, playerPosition.y, playerWidth, playerHeight };

            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (enemies[i].active) {
                    float enemyWidth = enemies[i].frames[0].width * enemies[i].scale;
                    float enemyHeight = enemies[i].frames[0].height * enemies[i].scale;
                    Rectangle enemyRect = { enemies[i].position.x, enemies[i].position.y, enemyWidth, enemyHeight };

                    if (CheckCollisionRecs(playerRect, enemyRect)) {
                        enemies[i].active = false;
                        playerLives--;
                        PlaySound(hitSound);

                        if (playerLives <= 0) {
                            playerLives = 0;
                            currentGameState = GAME_OVER;
                        }
                    }
                }
            }
        }

        // --- MOVIMIENTO DEL JUGADOR (CORREGIDO) ---
        bool moving = false;
        float bgScale = 3.8f;
        float bgWidth = levelBackgrounds[currentBackground].width * bgScale;
        float bgHeight = levelBackgrounds[currentBackground].height * bgScale;
        float bgX = (SCREEN_WIDTH - bgWidth) / 2;
        float bgY = (SCREEN_HEIGHT - bgHeight) / 2;

        // Resetear estados de movimiento al inicio de cada frame
        movingLeft = false;
        movingRight = false;

        // Manejar entrada de teclado para movimiento
        if (IsKeyDown(KEY_A)) {
            movingLeft = true;
            moving = true;
        }
        if (IsKeyDown(KEY_D)) {
            movingRight = true;
            moving = true;
        }

        // Actualizar posición basada en teclas presionadas
        if (IsKeyDown(KEY_W) && playerPosition.y > bgY) {
            playerPosition.y -= playerSpeed * deltaTime;
            moving = true;
        }
        if (IsKeyDown(KEY_S) && playerPosition.y + playerSprites[0].height * playerScale < bgY + bgHeight) {
            playerPosition.y += playerSpeed * deltaTime;
            moving = true;
        }
        if (IsKeyDown(KEY_A) && playerPosition.x > bgX) {
            playerPosition.x -= playerSpeed * deltaTime;
            moving = true;
            movingLeft = true;
            movingRight = false;
        }
        if (IsKeyDown(KEY_D) && playerPosition.x + playerSprites[0].width * playerScale < bgX + bgWidth) {
            playerPosition.x += playerSpeed * deltaTime;
            moving = true;
            movingRight = true;
            movingLeft = false;
        }

        // Sonido de pasos (igual que antes)
        if (moving && !levelTransition) {
            footstepCooldown -= deltaTime;
            if (footstepCooldown <= 0.0f) {
                PlaySound(footstepSound);
                footstepCooldown = footstepDelay;
            }
        }
        else {
            footstepCooldown = 0.0f;
        }

        // Animación del jugador (CORREGIDO)
        if (!moving) {
            currentFrame = 1;  // Frame neutral (quieto)
            movingLeft = false;
            movingRight = false;
        }
        else {
            frameCounter += deltaTime;
            if (frameCounter >= frameTime) {
                frameCounter = 0.0f;

                if (movingLeft) {
                    currentFrame = (currentFrame == 0) ? 1 : 0; // Alterna entre player1 y player2
                }
                else if (movingRight) {
                    currentFrame = (currentFrame == 2) ? 3 : 2; // Alterna entre player3 y player4
                }
                else {
                    // Movimiento vertical (arriba/abajo) - alterna entre player2 y player3
                    currentFrame = (currentFrame == 1) ? 2 : 1;
                }
            }
        }


        // Actualizar temporizador de animación de disparo
        if (shootAnimationTimer > 0) {
            shootAnimationTimer -= deltaTime;
        }
        else {
            currentShootDirection = NONE;
        }

        // Animación del fondo (si no estamos en transición)
        if (!levelTransition) {
            backgroundFrameCounter += deltaTime;
            if (backgroundFrameCounter >= backgroundFrameTime) {
                backgroundFrameCounter = 0.0f;
                currentBackground = (currentBackground + 1) % 2;
            }
        }

        // Colisión con obstáculos
        Texture2D currentPlayerTex = GetCurrentPlayerTexture();
        float pWidth = currentPlayerTex.width * playerScale;
        float pHeight = currentPlayerTex.height * playerScale;
        Rectangle playerRect = { playerPosition.x, playerPosition.y, pWidth, pHeight };

        for (int i = 0; i < 9; i++) {
            if (CheckCollisionRecs(playerRect, sideObstacles[i])) {
                if (playerPosition.x + pWidth > sideObstacles[i].x &&
                    playerPosition.x < sideObstacles[i].x) {
                    playerPosition.x = sideObstacles[i].x - pWidth;
                }
                if (playerPosition.x < sideObstacles[i].x + sideObstacles[i].width &&
                    playerPosition.x + pWidth > sideObstacles[i].x + sideObstacles[i].width) {
                    playerPosition.x = sideObstacles[i].x + sideObstacles[i].width;
                }
                if (playerPosition.y + pHeight > sideObstacles[i].y &&
                    playerPosition.y < sideObstacles[i].y) {
                    playerPosition.y = sideObstacles[i].y - pHeight;
                }
                if (playerPosition.y < sideObstacles[i].y + sideObstacles[i].height &&
                    playerPosition.y + pHeight > sideObstacles[i].y + sideObstacles[i].height) {
                    playerPosition.y = sideObstacles[i].y + sideObstacles[i].height;
                }
                break;

    case LEVEL_COMPLETED:
        if (IsKeyPressed(KEY_SPACE)) {
            ResetGame();
            currentGameState = MENU;
        }
        break;

            }
        }

        // Disparo normal (solo si no estamos en transición)
        if (!levelTransition && GetTime() - lastBulletTime >= bulletCooldown) {
            if (bulletCount < MAX_BULLETS) {
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!bulletActive[i]) {
                        Vector2 bulletDir = { 0, 0 };
                        bool shouldShoot = false;

                        if (IsKeyPressed(KEY_UP)) {
                            bulletDir = Vector2{ 0, -1 };
                            shouldShoot = true;
                            currentShootDirection = UP;
                            shootAnimationTimer = shootAnimationDuration;
                        }
                        if (IsKeyPressed(KEY_DOWN)) {
                            bulletDir = Vector2{ 0, 1 };
                            shouldShoot = true;
                            currentShootDirection = DOWN;
                            shootAnimationTimer = shootAnimationDuration;
                        }
                        if (IsKeyPressed(KEY_LEFT)) {
                            bulletDir = Vector2{ -1, 0 };
                            shouldShoot = true;
                            currentShootDirection = LEFT;
                            shootAnimationTimer = shootAnimationDuration;
                        }
                        if (IsKeyPressed(KEY_RIGHT)) {
                            bulletDir = Vector2{ 1, 0 };
                            shouldShoot = true;
                            currentShootDirection = RIGHT;
                            shootAnimationTimer = shootAnimationDuration;
                        }

                        if (shouldShoot) {
                            float playerCenterX = playerPosition.x + pWidth / 2;
                            float playerCenterY = playerPosition.y + pHeight / 2;

                            bullets[i] = Vector2{
                                playerCenterX - bulletTexture.width * bulletScale / 2 + bulletDir.x * 20,
                                playerCenterY - bulletTexture.height * bulletScale / 2 + bulletDir.y * 20
                            };

                            bulletDirections[i] = bulletDir;
                            bulletActive[i] = true;
                            bulletCount++;
                            lastBulletTime = GetTime();

                            PlaySound(gunshotSound);
                            break;
                        }
                    }
                }
            }
        }

        // Actualización de balas (solo si no estamos en transición)
        if (!levelTransition) {
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bulletActive[i]) {
                    bullets[i].x += bulletDirections[i].x * bulletSpeed * deltaTime;
                    bullets[i].y += bulletDirections[i].y * bulletSpeed * deltaTime;

                    // Colisión bala-enemigo
                    for (int j = 0; j < MAX_ENEMIES; j++) {
                        if (enemies[j].active && CheckBulletEnemyCollision(bullets[i], enemies[j])) {
                            bulletActive[i] = false;
                            bulletCount--;
                            enemies[j].active = false;
                            enemiesKilled++;

                            // Decidir si dropear power-up o moneda
                            int dropChance = rand() % 100;

                            if (dropChance < 25 && !wheelPowerUpOnField && !hasWheelPowerUp && !wheelPowerUpActive) {
                                SpawnPowerUp(enemies[j].position);
                            }
                            else if (dropChance < 100) {
                                SpawnCoin(enemies[j].position);
                            }

                            PlaySound(hitSound);
                            break;
                        }
                    }

                    // Eliminar balas fuera de pantalla
                    if (bulletActive[i] &&
                        (bullets[i].x < bgX || bullets[i].x > bgX + bgWidth ||
                            bullets[i].y < bgY || bullets[i].y > bgY + bgHeight)) {
                        bulletActive[i] = false;
                        bulletCount--;
                    }
                }
            }
        }
        break;

    case GAME_OVER:
        if (IsKeyPressed(KEY_SPACE)) {
            ResetGame();
            currentGameState = MENU;
        }
        break;
    }
}

void DrawLevelCompleted() {
    // Fondo negro
    ClearBackground(BLACK);

    // Texto "YOU WIN" en amarillo grande
    const char* winText = "YOU WIN";
    int winFontSize = 100;

    // Sombra del texto
    Vector2 winSize = MeasureTextEx(GetFontDefault(), winText, winFontSize, 2);
    DrawText(winText,
        (SCREEN_WIDTH - winSize.x) / 2 + 5,
        SCREEN_HEIGHT / 2 - 100 + 5,
        winFontSize,
        Color {
        50, 50, 0, 255
    }); // Sombra amarilla oscura

    // Texto principal amarillo
    DrawText(winText,
        (SCREEN_WIDTH - winSize.x) / 2,
        SCREEN_HEIGHT / 2 - 100,
        winFontSize,
        YELLOW);

    // Texto de instrucción en blanco con efecto de parpadeo
    const char* instructionText = "Press SPACE To Go Back To Menu";
    int instructionFontSize = 40;
    Vector2 instructionSize = MeasureTextEx(GetFontDefault(), instructionText, instructionFontSize, 1);

    // Efecto de parpadeo (cada segundo)
    if ((int)(GetTime() * 2) % 2 == 0) {
        DrawText(instructionText,
            (SCREEN_WIDTH - instructionSize.x) / 2,
            SCREEN_HEIGHT / 2 + 50,
            instructionFontSize,
            WHITE);
    }
}

void ResetGame() {
    playerLives = 3;
    timeRemaining = 60.0f;
    timeElapsed = 0.0f;
    enemiesKilled = 0;
    coinsCollected = 0;
    hasWheelPowerUp = false;
    wheelPowerUpActive = false;
    wheelPowerUpOnField = false;
    levelTransition = false;
    transitionY = 0.0f;
    transitionProgress = 0.0f;
    hiTimer = hiDuration;

    // Restaurar los fondos originales del nivel 1
    levelBackgrounds[0] = LoadTexture("levels/Level_1.png");
    levelBackgrounds[1] = LoadTexture("levels/Level_1_2.png");
    levelCompletedBackgrounds[0] = LoadTexture("levels/Level1_Completed1.png");
    levelCompletedBackgrounds[1] = LoadTexture("levels/Level1_Completed2.png");

    // Desactivar todos los elementos del juego
    for (int i = 0; i < MAX_ENEMIES; i++) enemies[i].active = false;
    for (int i = 0; i < MAX_BULLETS; i++) bulletActive[i] = false;
    for (int i = 0; i < MAX_COINS; i++) coins[i].active = false;
    for (int i = 0; i < MAX_POWERUPS; i++) powerUps[i].active = false;

    bulletCount = 0;
    playerPosition = Vector2{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

    // Reiniciar la música
    StopMusicStream(backgroundMusic);
    PlayMusicStream(backgroundMusic);
    SetMusicVolume(backgroundMusic, 0.5f);
    isMusicPlaying = true;
    arrowBlinkTimer = 0.0f;
    arrowVisible = false;
}


void DrawGame() {
    BeginDrawing();
    ClearBackground(BLACK);

    switch (currentGameState) {
    case INTRO_1:
        DrawIntroScreen1();
        break;
    case INTRO_2:
        DrawIntroScreen2();
        break;
    case INTRO_3:
        DrawIntroScreen3();
        break;
    case MENU:
        DrawTexture(menuLogo, (SCREEN_WIDTH - menuLogo.width) / 2, (SCREEN_HEIGHT - menuLogo.height) / 3, WHITE);
        DrawRectangleRec(startButton, WHITE);
        DrawText("START", startButton.x + 60, startButton.y + 15, 20, BLACK);
        break;
    case PLAYING:
        if (isMusicPlaying) {
            UpdateMusicStream(backgroundMusic);

            if (IsMusicStreamPlaying(backgroundMusic) == false) {
                PlayMusicStream(backgroundMusic);
            }
        }

        // --- Barra de tiempo con reloj ---
        float clockScale = 0.18f;
        float clockWidth = clockTexture.width * clockScale;
        float clockHeight = clockTexture.height * clockScale;

        float progressBarWidth = 900.0f;
        float progressBarHeight = 20.0f;
        float spacing = 10.0f;

        float clockX = (SCREEN_WIDTH - (clockWidth + spacing + progressBarWidth)) / 2;
        float clockY = 20.0f + (progressBarHeight - clockHeight) / 2;
        float progressBarX = clockX + clockWidth + spacing;
        float progressBarY = 20.0f;

        DrawTextureEx(clockTexture, Vector2 { clockX, clockY }, 0.0f, clockScale, WHITE);

        float remainingWidth = (timeRemaining / 60.0f) * progressBarWidth;
        DrawRectangle(progressBarX, progressBarY, progressBarWidth, progressBarHeight, BLACK);

        Color timeBarColor = GREEN;
        if (timeRemaining <= 10.0f) {
            timeBarColor = RED;
            if (timeRemaining <= 3.0f) {
                if ((int)(timeRemaining * 2) % 2 == 0) {
                    timeBarColor = ORANGE;
                }
            }
        }
        DrawRectangle(progressBarX, progressBarY, remainingWidth, progressBarHeight, timeBarColor);

        // Dibujar el fondo actual
        float scale = 3.8f;
        Rectangle sourceRec = { 0, 0, levelBackgrounds[currentBackground].width, levelBackgrounds[currentBackground].height };
        Rectangle destRec = {
            (SCREEN_WIDTH - levelBackgrounds[currentBackground].width * scale) / 2,
            (SCREEN_HEIGHT - levelBackgrounds[currentBackground].height * scale) / 2,
            levelBackgrounds[currentBackground].width * scale,
            levelBackgrounds[currentBackground].height * scale
        };
        Vector2 origin = { 0, 0 };

        // Si estamos en transición, dibujar ambos fondos con efecto de desplazamiento
        if (levelTransition) {
            float bgHeight = levelBackgrounds[currentBackground].height * 3.8f;
            float bgY = (SCREEN_HEIGHT - bgHeight) / 2;

            // 1. Dibujar el fondo actual desplazándose hacia arriba
            Rectangle currentSource = {
                0,
                0,
                levelBackgrounds[currentBackground].width,
                levelBackgrounds[currentBackground].height
            };

            Rectangle currentDest = {
                destRec.x,
                bgY - (bgHeight * transitionProgress), // Se mueve hacia arriba
                destRec.width,
                destRec.height
            };

            DrawTexturePro(levelBackgrounds[currentBackground], currentSource, currentDest, origin, 0.0f, WHITE);

            // 2. Dibujar el nuevo fondo apareciendo desde abajo
            Rectangle newSource = {
                0,
                0,
                level2Texture.width,
                level2Texture.height
            };

            Rectangle newDest = {
                destRec.x,
                bgY + (bgHeight * (1.0f - transitionProgress)), // Aparece desde abajo
                destRec.width,
                destRec.height
            };

            DrawTexturePro(level2Texture, newSource, newDest, origin, 0.0f, WHITE);
        }
        else {
            // Dibujar el fondo normalmente si no hay transición
            DrawTexturePro(levelBackgrounds[currentBackground], sourceRec, destRec, origin, 0.0f, WHITE);
        }



        // Dibujar al jugador (siempre visible, incluso durante transición)
        DrawTextureEx(GetCurrentPlayerTexture(), playerPosition, 0.0f, playerScale, WHITE);

        // Dibujar entidades del juego (solo si no estamos en transición)
        if (!levelTransition) {
            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (enemies[i].active) {
                    Texture2D currentFrame = enemies[i].frames[enemies[i].currentFrame];
                    DrawTextureEx(currentFrame, enemies[i].position, 0.0f, enemies[i].scale, WHITE);
                }
            }

            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bulletActive[i]) {
                    DrawTextureEx(bulletTexture, bullets[i], 0.0f, bulletScale, WHITE);
                }
            }

            for (int i = 0; i < MAX_COINS; i++) {
                if (coins[i].active) {
                    DrawTextureEx(coinTexture, coins[i].position, 0.0f, coins[i].scale, WHITE);
                }
            }

            for (int i = 0; i < MAX_POWERUPS; i++) {
                if (powerUps[i].active) {
                    DrawTextureEx(powerUps[i].texture, powerUps[i].position, 0.0f, powerUps[i].scale, WHITE);
                }
            }
        }

        // Dibujar flecha indicadora (si el tiempo se acabó y no estamos en transición)
        if (timeRemaining <= 0 && arrowVisible && !levelTransition) {
            float bgWidth = levelBackgrounds[currentBackground].width * scale;
            float bgHeight = levelBackgrounds[currentBackground].height * scale;
            float bgX = (SCREEN_WIDTH - bgWidth) / 2;
            float bgY = (SCREEN_HEIGHT - bgHeight) / 2;

            float arrowScale = 0.18f;
            Vector2 arrowPos = {
                bgX + bgWidth / 2 - (arrowTexture.width * arrowScale / 2) + 30,
                bgY + bgHeight - (arrowTexture.height * arrowScale) - 20
            };

            DrawTextureEx(arrowTexture, arrowPos, 0.0f, arrowScale, WHITE);
        }

        // Dibujar obstáculos (siempre visibles)
        for (int i = 0; i < 9; i++) {
            DrawRectangleLinesEx(sideObstacles[i], 2, DARKGRAY);
        }

        // --- UI ---
        // Contador de enemigos eliminados
        char killsText[50];
        sprintf(killsText, "Enemies Killed: %d", enemiesKilled);
        DrawText(killsText, 20, 20, 20, WHITE);

        // Dibujar contador de vidas
        if (liveTexture.id != 0) {
            float liveIconScale = 0.15f;
            Vector2 liveIconPos = { 340, 150 };
            DrawTextureEx(liveTexture, liveIconPos, 0.0f, liveIconScale, WHITE);
            DrawText(TextFormat("x %d", playerLives), liveIconPos.x + liveTexture.width * liveIconScale + 10, liveIconPos.y, 30, WHITE);
        }
        else {
            DrawText(TextFormat("Lives: %d", playerLives), 20, 50, 30, WHITE);
        }

        // Dibujar icono de moneda y contador
        float coinIconScale = 0.4f;
        Vector2 coinIconPos = { 350, 200 };
        if (coinTexture.id != 0) {
            DrawTextureEx(coinTexture, coinIconPos, 0.0f, coinIconScale, WHITE);
            DrawText(TextFormat("x %d", coinsCollected), coinIconPos.x + coinTexture.width * coinIconScale + 10, coinIconPos.y, 30, WHITE);
        }
        else {
            DrawText(TextFormat("Coins: %d", coinsCollected), 20, 100, 30, WHITE);
        }

        // --- UI de Power-ups ---
        float cartelScale = 0.2f;
        Vector2 cartelPos = { 350, 30 };
        DrawTextureEx(powerUpCartelTexture, cartelPos, 0.0f, cartelScale, WHITE);

        if (hasWheelPowerUp && !wheelPowerUpActive && !levelTransition) {
            float powerUpIconScale = 0.15f;
            Vector2 wheelIconPos = {
                cartelPos.x + (powerUpCartelTexture.width * cartelScale / 2) - (wheelTexture.width * powerUpIconScale / 2),
                cartelPos.y + (powerUpCartelTexture.height * cartelScale / 2) - (wheelTexture.height * powerUpIconScale / 2)
            };
            DrawTextureEx(wheelTexture, wheelIconPos, 0.0f, powerUpIconScale, WHITE);

            const char* text = "Press SPACE";
            Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 20, 1);
            DrawText(text,
                cartelPos.x + (powerUpCartelTexture.width * cartelScale / 2) - (textSize.x / 2),
                cartelPos.y + powerUpCartelTexture.height * cartelScale + 5,
                20, WHITE);
        }

        if (wheelPowerUpActive) {
            char timerText[20];
            sprintf(timerText, "%.1f", wheelPowerUpTimer);
            Vector2 textSize = MeasureTextEx(GetFontDefault(), timerText, 20, 1);
            DrawText(timerText,
                cartelPos.x + (powerUpCartelTexture.width * cartelScale / 2) - (textSize.x / 2),
                cartelPos.y + (powerUpCartelTexture.height * cartelScale / 2) - (textSize.y / 2),
                20, RED);
        }

        break;
    case GAME_OVER:
        DrawGameOver();
        break;

    case LEVEL_COMPLETED:
        DrawLevelCompleted();
        break;


    }

    EndDrawing();
}

void DrawGameOver() {
    // Fondo negro
    ClearBackground(BLACK);

    // Texto "GAME OVER" en rojo grande con sombra para mejor visibilidad
    const char* gameOverText = "GAME OVER";
    int gameOverFontSize = 100;

    // Sombra del texto
    Vector2 gameOverSize = MeasureTextEx(GetFontDefault(), gameOverText, gameOverFontSize, 2);
    DrawText(gameOverText,
        (SCREEN_WIDTH - gameOverSize.x) / 2 + 5,
        SCREEN_HEIGHT / 2 - 100 + 5,
        gameOverFontSize,
        Color {
        50, 50, 50, 255
    });

    // Texto principal
    DrawText(gameOverText,
        (SCREEN_WIDTH - gameOverSize.x) / 2,
        SCREEN_HEIGHT / 2 - 100,
        gameOverFontSize,
        RED);

    // Texto de instrucción con efecto de parpadeo
    const char* instructionText = "Press SPACE To Go Back To Menu";
    int instructionFontSize = 40;
    Vector2 instructionSize = MeasureTextEx(GetFontDefault(), instructionText, instructionFontSize, 1);

    // Efecto de parpadeo (cada segundo)
    if ((int)(GetTime() * 2) % 2 == 0) {
        DrawText(instructionText,
            (SCREEN_WIDTH - instructionSize.x) / 2,
            SCREEN_HEIGHT / 2 + 50,
            instructionFontSize,
            WHITE);
    }
}

void InitGame() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Journey of the Prairie King");
    ToggleFullscreen();
    SetTargetFPS(60);

    InitAudioDevice();
    SetMasterVolume(1.0f);

    srand(time(NULL));

    LoadIntroResources();
    menuLogo = LoadTexture("resources/JOPK_logo.png");
    liveTexture = LoadTexture("resources/UI/Live.png");

    level2Texture = LoadTexture("levels/Level_2.png");
    level2Texture2 = LoadTexture("levels/Level_2_2.png");

    if (liveTexture.id == 0) {
        TraceLog(LOG_WARNING, "No se pudo cargar la textura de vida en resources/UI/Live.png");
    }

    // Cargar sonidos
    startSound = LoadSound("resources/Sounds/start.wav");
    footstepSound = LoadSound("resources/Sounds/Footstep.wav");
    gunshotSound = LoadSound("resources/Sounds/Gunshot.wav");
    hitSound = LoadSound("resources/Sounds/Hit.wav");
    coinSound = LoadSound("resources/Sounds/Coin.wav");
    pickupPowerUpSound = LoadSound("resources/Sounds/PickUpPowerUp.wav");

    // Ajustar volúmenes de los efectos de sonido
    SetSoundVolume(footstepSound, 0.5f);
    SetSoundVolume(gunshotSound, 0.7f);
    SetSoundVolume(hitSound, 0.6f);
    SetSoundVolume(coinSound, 0.5f);
    SetSoundVolume(pickupPowerUpSound, 0.6f);

    backgroundMusic = LoadMusicStream("resources/BKMusic/TheOutlaw.mp3");
    backgroundMusic.looping = true;

    // En InitGame(), cargar todas las texturas del jugador:
    playerSprites[0] = LoadTexture("resources/player/player1.png");    // Quieto frame 1
    playerSprites[1] = LoadTexture("resources/player/player2.png");    // Quieto frame 2
    playerSprites[2] = LoadTexture("resources/player/player3.png");    // Movimiento derecha frame 1
    playerSprites[3] = LoadTexture("resources/player/player4.png");    // Movimiento derecha frame 2

    playerHiTexture = LoadTexture("resources/player/hi.png");

    // Cargar todas las texturas de disparo
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

    orcFrames[0] = LoadTexture("resources/Enemies/Orc/Orc1.png");
    orcFrames[1] = LoadTexture("resources/Enemies/Orc/Orc2.png");
    levelBackgrounds[0] = LoadTexture("levels/Level_1.png");
    levelBackgrounds[1] = LoadTexture("levels/Level_1_2.png");
    bulletTexture = LoadTexture("resources/bullet/bullet.png");
    coinTexture = LoadTexture("resources/Coin/Coin.png");
    wheelTexture = LoadTexture("resources/PowerUps/Wheel.png");

    hasWheelPowerUp = false;  // Añade esto en InitGame()
    wheelPowerUpOnField = false;
    wheelDropCooldown = 0.0f;

    powerUpCartelTexture = LoadTexture("resources/UI/PowerUpCartel.png");

    clockTexture = LoadTexture("resources/UI/Clock.png");

    levelBackgrounds[0] = LoadTexture("levels/Level_1.png");
    levelBackgrounds[1] = LoadTexture("levels/Level_1_2.png");
    levelCompletedBackgrounds[0] = LoadTexture("levels/Level1_Completed1.png"); // Añade esto
    levelCompletedBackgrounds[1] = LoadTexture("levels/Level1_Completed2.png"); // Añade esto

    arrowTexture = LoadTexture("resources/UI/Arrow.png"); // Añade esto después de cargar las otras texturas

    for (int i = 0; i < MAX_BULLETS; i++) {
        bulletActive[i] = false;
    }

    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = false;
        enemies[i].frames[0] = orcFrames[0];
        enemies[i].frames[1] = orcFrames[1];
        enemies[i].scale = 0.2f;
        enemies[i].speed = 100.0f + (rand() % 50);
    }

    for (int i = 0; i < MAX_COINS; i++) {
        coins[i].active = false;
    }

    for (int i = 0; i < MAX_POWERUPS; i++) {
        powerUps[i].active = false;
    }

    float bgX = (SCREEN_WIDTH - 860) / 2;
    float bgY = (SCREEN_HEIGHT - 860) / 2;
    float bgWidth = 860;
    float bgHeight = 860;
    float thickness = 50;
    float gapWidth = 50;

    sideObstacles[1] = Rectangle{ bgX - thickness, bgY + bgHeight, 320 + 2 * thickness, thickness };
    sideObstacles[2] = Rectangle{ bgX - thickness, bgY, thickness, 370 };
    sideObstacles[3] = Rectangle{ bgX + bgWidth, bgY, thickness, 370 };
    sideObstacles[4] = Rectangle{ bgX - thickness, bgY - thickness, thickness, thickness };
    sideObstacles[5] = Rectangle{ bgX + bgWidth, bgY - thickness, thickness, thickness };
    sideObstacles[6] = Rectangle{ bgX - thickness, bgY + bgHeight, thickness, thickness };
    sideObstacles[7] = Rectangle{ bgX + bgWidth, bgY + bgHeight, thickness, thickness };
    sideObstacles[8] = Rectangle{ bgX - thickness, bgY + bgHeight, bgHeight + 2 * thickness, thickness };

    float halfWidth = (bgWidth + 2 * thickness - gapWidth) / 2;

    sideObstacles[0] = Rectangle{ bgX - thickness, bgY - thickness, halfWidth, thickness };
    sideObstacles[8] = Rectangle{ bgX - thickness + halfWidth + gapWidth, bgY - thickness, halfWidth, thickness };

    hasWheelPowerUp = false;
    wheelPowerUpActive = false;
    wheelPowerUpOnField = false;
    wheelDropCooldown = 0.0f;

}

void CloseGame() {
    StopMusicStream(backgroundMusic);
    UnloadMusicStream(backgroundMusic);

    UnloadIntroResources();
    UnloadTexture(menuLogo);
    UnloadTexture(liveTexture);
    for (int i = 0; i < 4; i++) UnloadTexture(playerSprites[i]);
    for (int i = 0; i < 12; i++) UnloadTexture(shootSprites[i]);
    for (int i = 0; i < 2; i++) UnloadTexture(orcFrames[i]);
    for (int i = 0; i < 2; i++) {
        UnloadTexture(levelBackgrounds[i]);
        UnloadTexture(levelCompletedBackgrounds[i]); // Añade esto
    }
    UnloadTexture(bulletTexture);
    UnloadTexture(coinTexture);
    UnloadTexture(wheelTexture);
    UnloadTexture(powerUpCartelTexture);
    UnloadTexture(clockTexture);
    UnloadTexture(arrowTexture); // Añade esto con las otras descargas de texturas
    UnloadTexture(level2Texture);
    UnloadTexture(level2Texture2);
    UnloadTexture(playerHiTexture);

    // Descargar sonidos
    UnloadSound(startSound);
    UnloadSound(footstepSound);
    UnloadSound(gunshotSound);
    UnloadSound(hitSound);
    UnloadSound(coinSound);
    UnloadSound(pickupPowerUpSound);

    CloseAudioDevice();

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