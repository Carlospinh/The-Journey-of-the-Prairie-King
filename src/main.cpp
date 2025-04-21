<<<<<<< Updated upstream
#include "raylib.h"
#include "Player/Player.h"
#include "Bullet/Bullet.h"
#include "Enemy/Enemy.h"
#include "Enemy/EnemyManager.h"
#include "Environment/Background.h"
#include "Environment/ObstacleManager.h"
#include "UI/Menu.h"
#include "GameState.h"
#include "Constants.h"
#include <vector>

int main() {
    InitWindow(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, "Journey of the Prairie King");
    ToggleFullscreen();
    SetTargetFPS(60);

    Enemy::LoadTextures();

    Player player({ Constants::SCREEN_WIDTH / 2.0f - 100, Constants::SCREEN_HEIGHT / 2.0f }, Constants::PLAYER_SPEED);
    EnemyManager enemyManager(&player);

    Background background;
    background.LoadTextures("levels/Level_1.png", "levels/Level_1_2.png");

    ObstacleManager obstacleManager;
    obstacleManager.InitializeObstacles(background.GetBounds());

    Menu menu;
    menu.LoadResources();

    std::vector<Bullet*> bullets;
    for (int i = 0; i < Constants::MAX_BULLETS; i++) {
        bullets.push_back(nullptr);
    }

    GameState currentGameState = GameState::MENU;
    float timeRemaining = 60.0f;

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        if (currentGameState == GameState::MENU) {
            menu.Update();
            if (menu.ShouldStartGame()) {
                currentGameState = GameState::PLAYING;
            }
        }
        else {
            timeRemaining -= deltaTime;
            if (timeRemaining <= 0) timeRemaining = 0;

            background.Update(deltaTime);
            player.HandleInput(deltaTime);
            player.Update(deltaTime);
            enemyManager.Update(deltaTime);

            if (player.CanShoot()) {
                Vector2 bulletDir = { 0, 0 };
                bool shouldShoot = false;

                if (IsKeyPressed(KEY_UP)) {
                    bulletDir = { 0, -1 };
                    shouldShoot = true;
                }
                else if (IsKeyPressed(KEY_DOWN)) {
                    bulletDir = { 0, 1 };
                    shouldShoot = true;
                }
                else if (IsKeyPressed(KEY_LEFT)) {
                    bulletDir = { -1, 0 };
                    shouldShoot = true;
                }
                else if (IsKeyPressed(KEY_RIGHT)) {
                    bulletDir = { 1, 0 };
                    shouldShoot = true;
                }

                if (shouldShoot) {
                    for (int i = 0; i < Constants::MAX_BULLETS; i++) {
                        if (bullets[i] == nullptr || !bullets[i]->IsActive()) {
                            Vector2 playerCenter = {
                                player.GetPosition().x + player.GetCurrentTexture().width / 2,
                                player.GetPosition().y + player.GetCurrentTexture().height / 2
                            };

                            if (bullets[i] == nullptr) {
                                bullets[i] = new Bullet(playerCenter, bulletDir, Constants::BULLET_SPEED);
                            }
                            else {
                                bullets[i]->SetPosition(playerCenter);
                                bullets[i]->SetActive(true);
                            }

                            player.ResetShootTimer();
                            break;
                        }
                    }
                }
            }

            for (auto& bullet : bullets) {
                if (bullet && bullet->IsActive()) {
                    bullet->Update(deltaTime);
                }
            }

            enemyManager.CheckCollisionsWithBullets(bullets);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (currentGameState == GameState::MENU) {
            menu.Draw();
        }
        else {
            background.Draw();
            obstacleManager.DrawDebug();
            enemyManager.Draw();

            for (auto& bullet : bullets) {
                if (bullet && bullet->IsActive()) {
                    bullet->Draw();
                }
            }

            player.Draw();

            DrawText(TextFormat("Time: %.1f", timeRemaining), 20, 20, 30, WHITE);
            DrawText(TextFormat("Enemies: %d", (int)enemyManager.GetEnemyCount()), 20, 60, 30, WHITE);
        }

        EndDrawing();
    }

    for (auto& bullet : bullets) {
        delete bullet;
    }

    Enemy::UnloadTextures();
    CloseWindow();
    return 0;
}
=======
// main.cpp
#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

// --- DEFINES GENERALES ---
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define MAX_BULLETS 100
#define MAX_ENEMIES 20
#define MAX_COINS 50
#define MAX_POWERUPS 10

// --- ENUMS ---
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

// --- ESTRUCTURAS ---
typedef struct {
    Vector2 position;
    int currentFrame;
    float frameTimer;
    float speed;
    bool active;
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

// --- VARIABLES GLOBALES ---
GameState currentGameState = INTRO_1;
float introTimer = 0.0f, introDuration = 5.0f, fadeAlpha = 0.0f;
bool fadingIn = true, fadingOut = false;

Texture2D playerSprites[4], shootSprites[12], playerHiTexture;
Texture2D levelBackgrounds[2], levelCompletedBackgrounds[2];
Texture2D level2Texture, level2Texture2;
Texture2D menuLogo, clockTexture, powerUpCartelTexture, bulletTexture, coinTexture, wheelTexture, liveTexture;
Texture2D introScreen1, introScreen2, introScreen3, arrowTexture;

Sound startSound, footstepSound, gunshotSound, hitSound, coinSound, pickupPowerUpSound;
Music backgroundMusic;
bool isMusicPlaying = false;

Vector2 playerPosition = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
int currentFrame = 1;
float frameTime = 0.2f, frameCounter = 0.0f;
float playerSpeed = 200.0f;
bool movingLeft = false, movingRight = false;
float shootAnimationTimer = 0.0f;
float shootAnimationDuration = 0.3f;
ShootingDirection currentShootDirection = NONE;

Vector2 bullets[MAX_BULLETS], bulletDirections[MAX_BULLETS];
bool bulletActive[MAX_BULLETS];
int bulletCount = 0;
float lastBulletTime = 0.0f, bulletCooldown = 0.2f;
float bulletSpeed = 300.0f;

Enemy enemies[MAX_ENEMIES];
Coin coins[MAX_COINS];
PowerUp powerUps[MAX_POWERUPS];
float enemySpawnTimer = 0.0f, enemySpawnInterval = 2.0f;

int enemiesKilled = 0, coinsCollected = 0, playerLives = 3;

float timeRemaining = 60.0f, timeElapsed = 0.0f;
bool hasWheelPowerUp = false, wheelPowerUpActive = false;
float wheelPowerUpDuration = 12.0f, wheelPowerUpTimer = 0.0f;
float wheelShootCooldown = 0.5f, wheelShootTimer = 0.0f;
bool wheelPowerUpOnField = false;
float wheelDropCooldown = 0.0f;

Rectangle startButton = { SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 100, 200, 50 };
Rectangle sideObstacles[9];
float playerScale = 3.0f, bulletScale = 3.5f;

float backgroundFrameTime = 0.5f;
float backgroundFrameCounter = 0.0f;
int currentBackground = 0;
bool levelTransition = false;
float transitionY = 0.0f;
float transitionSpeed = 500.0f;
float transitionDuration = 1.5f;
float transitionProgress = 0.0f;

float arrowBlinkTimer = 0.0f;
float arrowBlinkInterval = 0.5f;
bool arrowVisible = false;

float hiTimer = 0.0f;
const float hiDuration = 1.0f;
>>>>>>> Stashed changes
