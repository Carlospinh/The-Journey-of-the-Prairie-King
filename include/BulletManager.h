#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include <raylib.h>

class BulletManager {
public:
    void Init();
    void Update(float deltaTime);
    void Draw();
    void Shoot(Vector2 origin, Vector2 direction);
    void ShootCircle(Vector2 origin);
    bool CheckCollision(Rectangle target);
    void Reset();
    void Unload();

private:
    static const int MAX_BULLETS = 50;
    Vector2 bullets[MAX_BULLETS];
    Vector2 directions[MAX_BULLETS];
    bool active[MAX_BULLETS];
    Texture2D texture;
    float speed;
    float scale;
    int count;
};

#endif
