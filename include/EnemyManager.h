#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <raylib.h>

class Enemy {
public:
    void Init(Texture2D frame1, Texture2D frame2);
    void Update(float deltaTime, Vector2 playerPos);
    void Draw();
    void Spawn(Vector2 position, float speed);
    void Deactivate();

    bool IsActive() const;
    Rectangle GetBounds() const;
    Vector2 GetPosition() const;

private:
    Vector2 position;
    float speed;
    int currentFrame;
    float frameTimer;
    Texture2D frames[2];
    float scale;
    bool active;
};

class EnemyManager {
public:
    void Init();
    void Update(float deltaTime, Vector2 playerPos);
    void Draw();
    void Spawn();
    void Reset();
    void Unload();

    static const int MAX_ENEMIES = 20;
    Enemy& Get(int index) { return enemies[index]; }

private:
    Enemy enemies[MAX_ENEMIES];
    float spawnTimer;
    float spawnInterval;
    Texture2D frame1;
    Texture2D frame2;
};

#endif
