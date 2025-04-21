#include "Game.h"

int main() {
    Game game;
    game.Init();

    while (!WindowShouldClose()) {
        game.Update();
        game.Draw();
    }

    game.Close();
    return 0;
}
