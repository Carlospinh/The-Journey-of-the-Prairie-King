#include "raylib.h"
#include "Player.h"
#include "Bullet.h"
#include "Background.h"
#include "ObstacleManager.h"
#include "Menu.h"
#include "GameState.h"
#include "Constants.h"

class GameManager {
private:
    GameState currentGameState;
    Player* player;
    Bullet* bullets[MAX_BULLETS];
    Background background;
    ObstacleManager obstacleManager;
    Menu menu;
    float timeRemaining;
    float timeElapsed;

public:
    GameManager();
    ~GameManager();

    void Init();
    void Update();
    void Draw();
    void Close();
};

GameManager::GameManager() : currentGameState(GameState::MENU),
player(nullptr), timeRemaining(60.0f), timeElapsed(0.0f) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i] = nullptr;
    }
}

void GameManager::Init() {
    InitWindow(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, "Journey of the Prairie King");
    ToggleFullscreen();
    SetTargetFPS(60);

    menu.LoadResources();

    background.LoadTextures("levels/Level_1.png", "levels/Level_1_2.png");
    obstacleManager.InitializeObstacles(background.GetBounds());

    player = new Player({ Constants::SCREEN_WIDTH / 2 - 100, Constants::SCREEN_HEIGHT / 2 }, 200.0f);
}

void GameManager::Update() {
    if (currentGameState == GameState::MENU) {
        menu.Update();
        if (menu.ShouldStartGame()) {
            currentGameState = GameState::PLAYING;
        }
    }
    else {
        // Actualización del juego...
        background.Update(GetFrameTime());
        player->Update(GetFrameTime());

        // Verificar colisiones con obstáculos
        if (obstacleManager.CheckCollision(player->GetCollisionBox())) {
            // Manejar colisión...
        }
    }
}

void GameManager::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    if (currentGameState == GameState::MENU) {
        menu.Draw();
    }
    else {
        background.Draw();
        player->Draw();
        obstacleManager.DrawDebug(); // Solo para debug
    }

    EndDrawing();
}

void GameManager::Close() {
    delete player;
    for (int i = 0; i < MAX_BULLETS; i++) {
        delete bullets[i];
    }
    CloseWindow();
}

int main() {
    GameManager game;
    game.Init();

    while (!WindowShouldClose()) {
        game.Update();
        game.Draw();
    }

    game.Close();
    return 0;
}