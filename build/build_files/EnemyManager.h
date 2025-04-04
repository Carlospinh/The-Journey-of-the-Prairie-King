#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "Enemy.h"
#include <vector>

class EnemyManager {
private:
    std::vector<Enemy*> enemies;
    float spawnTimer;
    float spawnInterval;
    Player* playerRef;
    float gameTime;
    float difficultyScale;

public:
    EnemyManager(Player* player);
    ~EnemyManager();

    void Update(float deltaTime);
    void Draw() const;
    void ClearEnemies();

    void SpawnEnemy();
    void SpawnWave(int waveSize);
    void CheckCollisionsWithBullets(std::vector<Bullet*>& bullets);

    const std::vector<Enemy*>& GetEnemies() const;
    int GetEnemyCount() const;
};

#endif