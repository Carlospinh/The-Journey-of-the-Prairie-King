#include "raylib.h"

// Definir estados del juego
typedef enum GameScreen { MENU, GAMEPLAY } GameScreen;

int main() {
    // Inicializar ventana
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Journey of the Prairie King");

    // Cargar imagen del logo
    Texture2D logo = LoadTexture("resources/JOPK_logo.png");
    
    // Estado del juego
    GameScreen currentScreen = MENU;
    
    while (!WindowShouldClose()) {
        // Manejo de entrada
        if (currentScreen == MENU) {
            if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = GAMEPLAY;  // Ir al juego
            }
        }
        
        // Dibujar
        BeginDrawing();
        ClearBackground(BLACK);

        if (currentScreen == MENU) {
            // Dibujar logo
            DrawTexture(logo, (screenWidth - logo.width) / 2, 150, WHITE);

            // Dibujar botón de inicio
            DrawText("Press ENTER to Start", 280, 450, 20, WHITE);
        }
        
        EndDrawing();
    }
    
    // Descargar recursos
    UnloadTexture(logo);
    
    // Cerrar ventana
    CloseWindow();
    return 0;
}