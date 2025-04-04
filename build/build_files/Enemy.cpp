#include "Enemy.h"
#include "Constants.h"

Texture2D Enemy::basicTexture = { 0 };
Texture2D Enemy::fastTexture = { 0 };
Texture2D Enemy::tankTexture = { 0 };

Enemy::Enemy(Vector2 pos, EnemyType enemyType, Player* target) :
    Entity(pos, 0, 0), type(enemyType), damage(10.0f),
    attackCooldown(2.0f), currentCooldown(0.0f),
    direction({ 0,0 }), target(target), color(WHITE) {

    switch (type) {
    case EnemyType::BASIC:
        speed = 100.0f;
        health = 30.0f;
        SetTexture(basicTexture);
        color = RED;
        break;
    case EnemyType::FAST:
        speed = 150.0f;
        health = 20.0f;
        SetTexture(fastTexture);
        color = SKYBLUE;
        break;
    case EnemyType::TANK:
        speed = 70.0f;
        health = 50.0f;
        SetTexture(tankTexture);
        color = GREEN;
        break;
    }

    initialHealth = health;
}

void Enemy::Update(float deltaTime) {
    if (!active || !target) return;

    UpdateDirection();

    // Movimiento
    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;

    // Actualizar caja de colisión
    collisionBox.x = position.x;
    collisionBox.y = position.y;

    // Enfriamiento de ataque
    if (currentCooldown > 0) {
        currentCooldown -= deltaTime;
    }

    // Atacar si está en rango
    if (CheckCollision(target->GetCollisionBox()) && currentCooldown <= 0) {
        Attack();
    }
}

void Enemy::Draw() const {
    if (!active) return;

    // Dibujar enemigo
    DrawTextureV(texture, position, color);

    // Barra de vida
    float healthPercentage = health / initialHealth;
    DrawRectangle(position.x, position.y - 10,
        texture.width * healthPercentage, 5, RED);
}

void Enemy::UpdateDirection() {
    if (!target) return;

    Vector2 targetPos = target->GetPosition();
    direction = Vector2Normalize(Vector2Subtract(targetPos, position));
}

void Enemy::Attack() {
    if (target && currentCooldown <= 0) {
        target->TakeDamage(damage);
        currentCooldown = attackCooldown;
    }
}

EnemyType Enemy::GetType() const {
    return type;
}

void Enemy::LoadTextures() {
    basicTexture = LoadTexture("resources/enemies/basic_enemy.png");
    fastTexture = LoadTexture("resources/enemies/fast_enemy.png");
    tankTexture = LoadTexture("resources/enemies/tank_enemy.png");
}

void Enemy::UnloadTextures() {
    UnloadTexture(basicTexture);
    UnloadTexture(fastTexture);
    UnloadTexture(tankTexture);
}