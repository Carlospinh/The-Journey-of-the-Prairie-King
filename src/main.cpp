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