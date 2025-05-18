#include "../include/UI.h"
#include "../include/PowerUp.h" // Add include for PowerUp class
#include <cstdio>  // Added for sprintf

UI::UI()
    : startButton({ SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 100, 200, 50 }),
    fadeAlpha(0.0f),
    fadingIn(true),
    fadingOut(false),
    introTimer(0.0f),
    introDuration(5.0f),
    currentGameState(INTRO_1) {
}

UI::~UI() {
    UnloadResources();
}

void UI::LoadResources() {
    // Load menu resources
    menuLogo = LoadTexture("resources/JOPK_logo.png");
    
    // Load intro screens
    introScreen1 = LoadTexture("resources/StartScreens/screen1.png");
    introScreen2 = LoadTexture("resources/StartScreens/screen2.png");
    introScreen3 = LoadTexture("resources/StartScreens/screen3.png");
    
    // Load HUD resources
    liveTexture = LoadTexture("resources/UI/Live.png");
    coinTexture = LoadTexture("resources/Coin/Coin.png");
    powerUpCartelTexture = LoadTexture("resources/UI/PowerUpCartel.png");
    clockTexture = LoadTexture("resources/UI/Clock.png");
}

void UI::UnloadResources() {
    // Unload menu resources
    UnloadTexture(menuLogo);
    
    // Unload intro screens
    UnloadTexture(introScreen1);
    UnloadTexture(introScreen2);
    UnloadTexture(introScreen3);
    
    // Unload HUD resources
    UnloadTexture(liveTexture);
    UnloadTexture(coinTexture);
    UnloadTexture(powerUpCartelTexture);
    UnloadTexture(clockTexture);
}

void UI::Update(float deltaTime) {
    switch (currentGameState) {
    case INTRO_1:
    case INTRO_2:
    case INTRO_3:
        // Update intro animations
        if (fadingIn) {
            fadeAlpha += deltaTime * 2.0f;
            if (fadeAlpha >= 1.0f) {
                fadeAlpha = 1.0f;
                fadingIn = false;
            }
        }

        introTimer += deltaTime;

        if (introTimer >= introDuration - 1.0f && !fadingOut) {
            fadingOut = true;
        }

        if (fadingOut) {
            fadeAlpha -= deltaTime * 2.0f;
            if (fadeAlpha <= 0.0f) {
                fadeAlpha = 0.0f;
                fadingOut = false;

                if (currentGameState == INTRO_1) {
                    currentGameState = INTRO_2;
                }
                else if (currentGameState == INTRO_2) {
                    currentGameState = INTRO_3;
                }
                else if (currentGameState == INTRO_3) {
                    currentGameState = MENU;
                }

                introTimer = 0.0f;
                fadingIn = true;
            }
        }
        break;
        
    default:
        break;
    }
}

void UI::DrawIntroScreen1() {
    const char* introText = "Hi! This is our remake of Journey of the Prairie King, the arcade minigame\n"
        "featured in Stardew Valley. We've recreated it from scratch using Raylib\n"
        "as a standalone project to learn more about game development and pay\n"
        "tribute to this fun classic. Enjoy!";

    Vector2 textSize = MeasureTextEx(GetFontDefault(), introText, 30, 1);
    DrawText(introText, (SCREEN_WIDTH - textSize.x) / 2 - 100, SCREEN_HEIGHT / 4, 30, Fade(WHITE, fadeAlpha));

    float imageScale = 0.7f;
    float imageWidth = introScreen1.width * imageScale;
    float imageHeight = introScreen1.height * imageScale;
    DrawTextureEx(introScreen1,
        Vector2 {
        (SCREEN_WIDTH - imageWidth) / 2 + 20, SCREEN_HEIGHT / 2 - 200
    },
        0.0f, imageScale, Fade(WHITE, fadeAlpha));
}

void UI::DrawIntroScreen2() {
    const char* title = "Our Team and Mentor";
    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, 40, 1);
    DrawText(title, (SCREEN_WIDTH - titleSize.x) / 2, SCREEN_HEIGHT / 8, 40, Fade(WHITE, fadeAlpha));

    float imageScale = 0.7f;
    float imageWidth = introScreen2.width * imageScale;
    float imageHeight = introScreen2.height * imageScale;
    DrawTextureEx(introScreen2,
        Vector2 {
        (SCREEN_WIDTH - imageWidth) / 2, SCREEN_HEIGHT / 2 - 300
    },
        0.0f, imageScale, Fade(WHITE, fadeAlpha));
}

void UI::DrawIntroScreen3() {
    const char* title = "Follow Our Journey On...";
    const char* githubText = "github.com/Carlospinh/The-Journey-of-the-Prairie-King";

    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, 40, 1);
    DrawText(title, (SCREEN_WIDTH - titleSize.x) / 2 - 100, SCREEN_HEIGHT / 4, 40, Fade(WHITE, fadeAlpha));

    Vector2 textSize = MeasureTextEx(GetFontDefault(), githubText, 30, 1);
    DrawText(githubText, (SCREEN_WIDTH - textSize.x) / 2, SCREEN_HEIGHT / 3 + 50, 30, Fade(WHITE, fadeAlpha));

    float imageScale = 0.7f;
    float imageWidth = introScreen3.width * imageScale;
    float imageHeight = introScreen3.height * imageScale;
    DrawTextureEx(introScreen3,
        Vector2 {
        (SCREEN_WIDTH - imageWidth) / 2, SCREEN_HEIGHT / 2 - 200
    },
        0.0f, imageScale, Fade(WHITE, fadeAlpha));
}

void UI::DrawMenu() {
    DrawTexture(menuLogo, (SCREEN_WIDTH - menuLogo.width) / 2, (SCREEN_HEIGHT - menuLogo.height) / 3, WHITE);
    DrawRectangleRec(startButton, WHITE);
    DrawText("START", startButton.x + 60, startButton.y + 15, 20, BLACK);
}

void UI::DrawGameOver() {
    ClearBackground(BLACK);

    const char* gameOverText = "GAME OVER";
    int gameOverFontSize = 100;

    Vector2 gameOverSize = MeasureTextEx(GetFontDefault(), gameOverText, gameOverFontSize, 2);
    DrawText(gameOverText,
        (SCREEN_WIDTH - gameOverSize.x) / 2 + 5,
        SCREEN_HEIGHT / 2 - 100 + 5,
        gameOverFontSize,
        Color {
        50, 50, 50, 255
    });

    DrawText(gameOverText,
        (SCREEN_WIDTH - gameOverSize.x) / 2,
        SCREEN_HEIGHT / 2 - 100,
        gameOverFontSize,
        RED);

    const char* instructionText = "Press SPACE To Go Back To Menu";
    int instructionFontSize = 40;
    Vector2 instructionSize = MeasureTextEx(GetFontDefault(), instructionText, instructionFontSize, 1);

    if ((int)(GetTime() * 2) % 2 == 0) {
        DrawText(instructionText,
            (SCREEN_WIDTH - instructionSize.x) / 2,
            SCREEN_HEIGHT / 2 + 50,
            instructionFontSize,
            WHITE);
    }
}

void UI::DrawLevelCompleted() {
    ClearBackground(BLACK);

    const char* winText = "YOU WIN";
    int winFontSize = 100;

    Vector2 winSize = MeasureTextEx(GetFontDefault(), winText, winFontSize, 2);
    DrawText(winText,
        (SCREEN_WIDTH - winSize.x) / 2 + 5,
        SCREEN_HEIGHT / 2 - 100 + 5,
        winFontSize,
        Color {
        50, 50, 0, 255
    });

    DrawText(winText,
        (SCREEN_WIDTH - winSize.x) / 2,
        SCREEN_HEIGHT / 2 - 100,
        winFontSize,
        YELLOW);

    const char* instructionText = "Press SPACE To Go Back To Menu";
    int instructionFontSize = 40;
    Vector2 instructionSize = MeasureTextEx(GetFontDefault(), instructionText, instructionFontSize, 1);

    if ((int)(GetTime() * 2) % 2 == 0) {
        DrawText(instructionText,
            (SCREEN_WIDTH - instructionSize.x) / 2,
            SCREEN_HEIGHT / 2 + 50,
            instructionFontSize,
            WHITE);
    }
}

void UI::DrawHUD(int playerLives, int coinsCollected, int enemiesKilled, float timeRemaining, bool hasWheelPowerUp, bool wheelPowerUpActive, float wheelPowerUpTimer, int currentLevel) {
    // Draw clock and time bar
    float clockScale = 0.18f;
    float clockWidth = clockTexture.width * clockScale;
    float clockHeight = clockTexture.height * clockScale;
    float progressBarWidth = 900.0f;
    float progressBarHeight = 20.0f;
    float spacing = 10.0f;
    
    float clockX = (SCREEN_WIDTH - (clockWidth + spacing + progressBarWidth)) / 2;
    float clockY = 20.0f + (progressBarHeight - clockHeight) / 2;
    float progressBarX = clockX + clockWidth + spacing;
    float progressBarY = 20.0f;
    
    DrawTextureEx(clockTexture, Vector2 { clockX, clockY }, 0.0f, clockScale, WHITE);
    
    float remainingWidth = (timeRemaining / 60.0f) * progressBarWidth;
    DrawRectangle(progressBarX, progressBarY, progressBarWidth, progressBarHeight, BLACK);
    
    Color timeBarColor = GREEN;
    if (timeRemaining <= 10.0f) timeBarColor = RED;
    if (timeRemaining <= 3.0f && (int)(timeRemaining * 2) % 2 == 0) timeBarColor = ORANGE;
    
    DrawRectangle(progressBarX, progressBarY, remainingWidth, progressBarHeight, timeBarColor);
    
    // Display current level
    char levelText[50];
    sprintf(levelText, "Level: %d/9", currentLevel);
    DrawText(levelText, 20, 20, 25, WHITE);
    
    // Display enemies killed
    char killsText[50];
    sprintf(killsText, "Enemies Killed: %d", enemiesKilled); 
    DrawText(killsText, 20, 50, 20, WHITE);
    
    if (liveTexture.id != 0) {
        float liveIconScale = 0.15f;
        Vector2 liveIconPos = { 340, 150 };
        DrawTextureEx(liveTexture, liveIconPos, 0.0f, liveIconScale, WHITE);
        DrawText(TextFormat("x %d", playerLives), liveIconPos.x + liveTexture.width * liveIconScale + 10, liveIconPos.y, 30, WHITE);
    }
    else {
        DrawText(TextFormat("Lives: %d", playerLives), 20, 80, 30, WHITE);
    }
    
    float coinIconScale = 0.4f;
    Vector2 coinIconPos = { 350, 200 };
    if (coinTexture.id != 0) {
        DrawTextureEx(coinTexture, coinIconPos, 0.0f, coinIconScale, WHITE);
        DrawText(TextFormat("x %d", coinsCollected), coinIconPos.x + coinTexture.width * coinIconScale + 10, coinIconPos.y, 30, WHITE);
    }
    else {
        DrawText(TextFormat("Coins: %d", coinsCollected), 20, 110, 30, WHITE);
    }
    
    float cartelScale = 0.2f;
    Vector2 cartelPos = { 350, 30 };
    DrawTextureEx(powerUpCartelTexture, cartelPos, 0.0f, cartelScale, WHITE);
    
    Texture2D wheelTexture = PowerUp::GetWheelTexture();
    
    if (hasWheelPowerUp && !wheelPowerUpActive) {
        float powerUpIconScale = 0.15f;
        Vector2 wheelIconPos = {
            cartelPos.x + (powerUpCartelTexture.width * cartelScale / 2) - (wheelTexture.width * powerUpIconScale / 2),
            cartelPos.y + (powerUpCartelTexture.height * cartelScale / 2) - (wheelTexture.height * powerUpIconScale / 2)
        };
        DrawTextureEx(wheelTexture, wheelIconPos, 0.0f, powerUpIconScale, WHITE);
        
        const char* text = "Press SPACE";
        Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 20, 1);
        DrawText(text,
            cartelPos.x + (powerUpCartelTexture.width * cartelScale / 2) - (textSize.x / 2),
            cartelPos.y + powerUpCartelTexture.height * cartelScale + 5,
            20, WHITE);
    }
    
    if (wheelPowerUpActive) {
        char timerText[20];
        sprintf(timerText, "%.1f", wheelPowerUpTimer);
        Vector2 textSize = MeasureTextEx(GetFontDefault(), timerText, 20, 1);
        DrawText(timerText,
            cartelPos.x + (powerUpCartelTexture.width * cartelScale / 2) - (textSize.x / 2),
            cartelPos.y + (powerUpCartelTexture.height * cartelScale / 2) - (textSize.y / 2),
            20, RED);
    }
}

GameState UI::GetGameState() const {
    return currentGameState;
}

void UI::SetGameState(GameState state) {
    currentGameState = state;
    
    if (state == INTRO_1 || state == INTRO_2 || state == INTRO_3) {
        introTimer = 0.0f;
        fadeAlpha = 0.0f;
        fadingIn = true;
        fadingOut = false;
    }
}

bool UI::IsMouseOverStart() const {
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, startButton);
}
