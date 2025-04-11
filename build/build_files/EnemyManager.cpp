#include "EnemyManager.h"
#include "Bullet.h"
#include "Constants.h"
#include <algorithm>

EnemyManager::EnemyManager(Player* player) :
    playerRef(player), spawnTimer(0.0f), spawnInterval(Constants::ENEMY_SPAWN_INTERVAL),
    gameTime(0.0f), difficultyScale(1.0f) {}

EnemyManager::~EnemyManager() {
    ClearEnemies();
}

void EnemyManager::Update(float deltaTime) {
    gameTime += deltaTime;
    difficultyScale = 1.0f + gameTime / 120.0f;

    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval) {
        SpawnWave(1 + static_cast<int>(gameTime / 30.0f));
        spawnTimer = 0.0f;
        spawnInterval = std::max(1.0f, Constants::ENEMY_SPAWN_INTERVAL / difficultyScale);
    }

    for (auto& enemy : enemies) {
        if (enemy->IsActive()) {
            enemy->Update(deltaTime);
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](Enemy* e) {
            if (!e->IsActive()) {
                delete e;
                return true;
            }
            return false;
        }), enemies.end());
}

void EnemyManager::Draw() const {
    for (const auto& enemy : enemies) {
        enemy->Draw();
    }
}

void EnemyManager::SpawnEnemy() {
    if (!playerRef) return;

    Vector2 spawnPos;
    int side = GetRandomValue(0, 3);
    int padding = 50;

    switch (side) {
    case 0:
        spawnPos = {
            static_cast<float>(GetRandomValue(padding, Constants::SCREEN_WIDTH - padding)),
            static_cast<float>(-padding)
        };
        break;
    case 1:
        spawnPos = {
            static_cast<float>(Constants::SCREEN_WIDTH + padding),
            static_cast<float>(GetRandomValue(padding, Constants::SCREEN_HEIGHT - padding))
        };
        break;
    case 2:
        spawnPos = {
            static_cast<float>(GetRandomValue(padding, Constants::SCREEN_WIDTH - padding)),
            static_cast<float>(Constants::SCREEN_HEIGHT + padding)
        };
        break;
    case 3:
        spawnPos = {
            static_cast<float>(-padding),
            static_cast<float>(GetRandomValue(padding, Constants::SCREEN_HEIGHT - padding))
        };
        break;
    }

    EnemyType type;
    float rand = GetRandomValue(0, 100);
    if (rand < 60) type = EnemyType::BASIC;
    else if (rand < 90) type = EnemyType::FAST;
    else type = EnemyType::TANK;

    enemies.push_back(new Enemy(spawnPos, type, playerRef));
}

void EnemyManager::SpawnWave(int waveSize) {
    for (int i = 0; i < waveSize; i++) {
        SpawnEnemy();
    }
}

void EnemyManager::CheckCollisionsWithBullets(const std::vector<Bullet*>& bullets) {
    for (auto& bullet : bullets) {
        if (bullet && bullet->IsActive()) {
            for (auto& enemy : enemies) {
                if (enemy->IsActive() && enemy->CheckCollision(bullet->GetCollisionBox())) {
                    enemy->TakeDamage(bullet->GetDamage());
                    bullet->SetActive(false);
                    break;
                }
            }
        }
    }
}

void EnemyManager::ClearEnemies() {
    for (auto& enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
}

const std::vector<Enemy*>& EnemyManager::GetEnemies() const {
    return enemies;
}

size_t EnemyManager::GetEnemyCount() const {
    return enemies.size();
}