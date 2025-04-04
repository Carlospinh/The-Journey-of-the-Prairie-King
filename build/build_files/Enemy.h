#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "Player.h"

enum class EnemyType {
    BASIC,
    FAST,
    TANK
};

class Enemy : public Entity {
private:
    EnemyType type;
    float damage;
    float attackCooldown;
    float currentCooldown;
    Vector2 direction;
    Player* target;
    Color color;

public:
    Enemy(Vector2 pos, EnemyType enemyType, Player* target);

    void Update(float deltaTime) override;
    void Draw() const override;

    void UpdateDirection();
    void Attack();
    EnemyType GetType() const;

    static Texture2D basicTexture;
    static Texture2D fastTexture;
    static Texture2D tankTexture;
    static void LoadTextures();
    static void UnloadTextures();
};

#endif