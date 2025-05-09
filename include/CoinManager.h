#ifndef COIN_MANAGER_H
#define COIN_MANAGER_H

#include <raylib.h>

class CoinManager {
public:
    void Init();
    void Update(Vector2 playerPos);
    void Draw();
    void Spawn(Vector2 position);
    void Reset();
    void Unload();

private:
    static const int MAX_COINS = 20;
    struct Coin {
        Vector2 position;
        float scale;
        bool active;
    } coins[MAX_COINS];

    Texture2D texture;
    float scale;
};

#endif
