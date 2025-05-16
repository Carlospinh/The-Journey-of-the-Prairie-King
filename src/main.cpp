#include "../include/GameManager.h"

int main() {
    GameManager game;
    
    game.InitGame();
    
    while (!game.ShouldClose()) {
        game.Update();
        game.Draw();
    }
    
    return 0;
}
