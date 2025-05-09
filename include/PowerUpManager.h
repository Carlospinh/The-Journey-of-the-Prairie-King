#ifndef POWERUP_MANAGER_H
#define POWERUP_MANAGER_H

#include <raylib.h>

class PowerUpManager {
public:
    void Init();
    void Update(float deltaTime, Vector2 playerPos);
    void Draw();
    void TrySpawn(Vector2 pos);
    void Activate();
    bool HasPowerUp() const;
    bool IsActive() const;
    float GetTimer() const;
    void Reset();
    void Unload();

private:
    static const int MAX_POWERUPS = 5;
    struct PowerUp {
        Vector2 position;
        float scale;
        bool active;
    } powerUps[MAX_POWERUPS];

    Texture2D texture;
    float scale;
    bool hasPowerUp;
    bool activePowerUp;
    float timer;
    float duration;
    float shootCooldown;
    float shootTimer;
};

#endif
