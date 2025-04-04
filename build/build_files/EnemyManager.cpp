#include "EnemyManager.h"
#include "Constants.h"
#include <algorithm>

EnemyManager::EnemyManager(Player* player) :
    playerRef(player), spawnTimer(0.0f), spawnInterval(3.0f),
    gameTime(0.0f), difficultyScale(1.0f) {}

EnemyManager::~EnemyManager() {
    ClearEnemies();
}

void EnemyManager::Update(float deltaTime) {
    gameTime += deltaTime;
    difficultyScale = 1.0f + gameTime / 120.0f; // Aumenta dificultad cada 2 minutos

    // Spawn lógico
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval) {
        SpawnWave(1 + static_cast<int>(gameTime / 30.0f)); // +1 enemigo cada 30 segundos
        spawnTimer = 0.0f;
        spawnInterval = std::max(1.0f, 3.0f / difficultyScale); // Reduce intervalo con dificultad
    }

    // Actualizar enemigos
    for (auto& enemy : enemies) {
        if (enemy->IsActive()) {
            enemy->Update(deltaTime);
        }
    }

    // Eliminar enemigos inactivos
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
    case 0: // Arriba
        spawnPos = {
            GetRandomValue(padding, Constants::SCREEN_WIDTH - padding),
            -padding
        };
        break;
    case 1: // Derecha
        spawnPos = {
            Constants::SCREEN_WIDTH + padding,
            GetRandomValue(padding, Constants::SCREEN_HEIGHT - padding)
        };
        break;
    case 2: // Abajo
        spawnPos = {
            GetRandomValue(padding, Constants::SCREEN_WIDTH - padding),
            Constants::SCREEN_HEIGHT + padding
        };
        break;
    case 3: // Izquierda
        spawnPos = {
            -padding,
            GetRandomValue(padding, Constants::SCREEN_HEIGHT - padding)
        };
        break;
    }

    // Tipo aleatorio con ponderación según dificultad
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

void EnemyManager::CheckCollisionsWithBullets(std::vector<Bullet*>& bullets) {
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

int EnemyManager::GetEnemyCount() const {
    return enemies.size();
}