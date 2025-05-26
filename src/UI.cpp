#include "../include/UI.h"
#include "../include/PowerUp.h" // Add include for PowerUp class
#include <cstdio>  // Added for sprintf

UI::UI()
    : startButton({ SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 100, 200, 50 }),
    fadeAlpha(0.0f),
    fadingIn(true),
    fadingOut(false),
    introTimer(0.0f),
    introDuration(6.0f),
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
    levelBallTexture = LoadTexture("resources/levelball/Level_Ball.png");
    
    // Load shop item textures for UI display
    shopGunTextures[0] = LoadTexture("resources/Store/Gun/Store_Gun_1.png");
    shopGunTextures[1] = LoadTexture("resources/Store/Gun/Store_Gun_2.png");
    shopGunTextures[2] = LoadTexture("resources/Store/Gun/Store_Gun_3.png");
    
    shopBootTextures[0] = LoadTexture("resources/Store/Bota/Store_Bota_1.png");
    shopBootTextures[1] = LoadTexture("resources/Store/Bota/Store_Bota_2.png");
    
    shopBoxTextures[0] = LoadTexture("resources/Store/Box/Store_Box_1.png");
    shopBoxTextures[1] = LoadTexture("resources/Store/Box/Store_Box_2.png");
    shopBoxTextures[2] = LoadTexture("resources/Store/Box/Store_Box_3.png");
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
    UnloadTexture(levelBallTexture);  // Added this line to unload level ball texture
    
    // Unload shop item textures
    for (int i = 0; i < 3; i++) {
        UnloadTexture(shopGunTextures[i]);
        UnloadTexture(shopBoxTextures[i]);
    }
    for (int i = 0; i < 2; i++) {
        UnloadTexture(shopBootTextures[i]);
    }
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
        {
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
        {
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
        {
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
        {
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
        {
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

void UI::DrawHUD(int playerLives, int coinsCollected, int enemiesKilled, float timeRemaining, 
                 bool hasPowerUp, bool powerUpActive, float powerUpTimer, int currentLevel,
                 bool hasWheelPowerUp, bool hasShotgunPowerUp, bool hasCoffeePowerUp, bool hasNukePowerUp,
                 bool wheelPowerUpActive, bool shotgunPowerUpActive, bool coffeePowerUpActive,
                 int gunUpgradeLevel, int bootUpgradeLevel, int boxUpgradeLevel) {
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
    
    DrawTextureEx(clockTexture, { clockX, clockY }, 0.0f, clockScale, WHITE);
    
    float remainingWidth = (timeRemaining / 60.0f) * progressBarWidth;
    DrawRectangle(progressBarX, progressBarY, progressBarWidth, progressBarHeight, BLACK);
    
    Color timeBarColor = GREEN;
    if (timeRemaining <= 10.0f) timeBarColor = RED;
    if (timeRemaining <= 3.0f && (int)(timeRemaining * 2) % 2 == 0) timeBarColor = ORANGE;
    
    DrawRectangle(progressBarX, progressBarY, remainingWidth, progressBarHeight, timeBarColor);
    
    // Display level using multiple level balls
    float levelBallScale = 0.18f;
    float levelBallSpacing = 5.0f;  // Space between balls
    float levelBallHeight = levelBallTexture.height * levelBallScale;
    float startX = GetScreenWidth() / 1.32f ;  
    float startY = 20;

    // Draw level balls up to current level
    for (int i = 0; i < currentLevel; i++) {
        Vector2 ballPos = { startX, startY + (levelBallHeight + levelBallSpacing) * i };
        DrawTextureEx(levelBallTexture, ballPos, 0.0f, levelBallScale, WHITE);
    }
    // Display enemies killed
    char killsText[50];
    sprintf(killsText, "Enemies Killed: %d", enemiesKilled);
    DrawText(killsText, 20, 50, 20, WHITE);
    
    // Draw lives
    if (liveTexture.id != 0) {
        float liveIconScale = 0.15f;
        Vector2 liveIconPos = { 340, 150 };
        DrawTextureEx(liveTexture, liveIconPos, 0.0f, liveIconScale, WHITE);
        DrawText(TextFormat("x %d", playerLives), liveIconPos.x + liveTexture.width * liveIconScale + 10, liveIconPos.y, 30, WHITE);
    }
    else {
        DrawText(TextFormat("Lives: %d", playerLives), 20, 80, 30, WHITE);
    }
    
    // Draw coins
    float coinIconScale = 0.4f;
    Vector2 coinIconPos = { 350, 200 };
    if (coinTexture.id != 0) {
        DrawTextureEx(coinTexture, coinIconPos, 0.0f, coinIconScale, WHITE);
        DrawText(TextFormat("x %d", coinsCollected), coinIconPos.x + coinTexture.width * coinIconScale + 10, coinIconPos.y, 30, WHITE);
    }
    else {
        DrawText(TextFormat("Coins: %d", coinsCollected), 20, 110, 30, WHITE);
    }
    
    // Draw power-up info
    float cartelScale = 0.2f;
    Vector2 cartelPos = { 350, 30 };
    DrawTextureEx(powerUpCartelTexture, cartelPos, 0.0f, cartelScale, WHITE);
    
    // Get all power-up textures
    Texture2D wheelTexture = PowerUp::GetWheelTexture();
    Texture2D shotgunTexture = PowerUp::GetShotgunTexture();
    Texture2D coffeeTexture = PowerUp::GetCoffeeTexture();
    Texture2D nukeTexture = PowerUp::GetNukeTexture();
    
    // Show power-up if available or active
    float powerUpIconScale = 0.15f;
    
    if (hasPowerUp && !powerUpActive) {
        // Determine which powerup to display based on what player has
        Texture2D iconTexture = wheelTexture; // Default fallback
        
        if (hasWheelPowerUp) {
            iconTexture = wheelTexture;
        } else if (hasShotgunPowerUp) {
            iconTexture = shotgunTexture;
        } else if (hasCoffeePowerUp) {
            iconTexture = coffeeTexture;
            powerUpIconScale = 0.55f; // Larger scale for coffee cup
        } else if (hasNukePowerUp) {
            iconTexture = nukeTexture;
        }
        
        Vector2 iconPos = {
            cartelPos.x + (powerUpCartelTexture.width * cartelScale / 2) - (iconTexture.width * powerUpIconScale / 2),
            cartelPos.y + (powerUpCartelTexture.height * cartelScale / 2) - (iconTexture.height * powerUpIconScale / 2)
        };
        DrawTextureEx(iconTexture, iconPos, 0.0f, powerUpIconScale, WHITE);
        
        const char* text = "Press SPACE";
        Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 20, 1);
        DrawText(text,
            cartelPos.x + (powerUpCartelTexture.width * cartelScale / 2) - (textSize.x / 2),
            cartelPos.y + powerUpCartelTexture.height * cartelScale + 5,
            20, WHITE);
    }
    else if (powerUpActive) {
        // Show active power-up with timer
        char timerText[20];
        sprintf(timerText, "%.1f", powerUpTimer);
        Vector2 textSize = MeasureTextEx(GetFontDefault(), timerText, 20, 1);
        
        // Determine which active powerup to display
        Texture2D iconTexture = wheelTexture; // Default fallback
        
        if (wheelPowerUpActive) {
            iconTexture = wheelTexture;
        } else if (shotgunPowerUpActive) {
            iconTexture = shotgunTexture;
        } else if (coffeePowerUpActive) {
            iconTexture = coffeeTexture;
            powerUpIconScale = 0.55f; // Larger scale for coffee cup when active
        }
        
        Vector2 iconPos = {
            cartelPos.x + (powerUpCartelTexture.width * cartelScale / 2) - (iconTexture.width * powerUpIconScale / 2),
            cartelPos.y + (powerUpCartelTexture.height * cartelScale / 2) - (iconTexture.height * powerUpIconScale / 2) - 10
        };
        DrawTextureEx(iconTexture, iconPos, 0.0f, powerUpIconScale, WHITE);
        
        DrawText(timerText,
            cartelPos.x + (powerUpCartelTexture.width * cartelScale / 2) - (textSize.x / 2),
            cartelPos.y + (powerUpCartelTexture.height * cartelScale / 2) + 15,
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

void UI::DrawShopItems(int gunUpgradeLevel, int bootUpgradeLevel, int boxUpgradeLevel) {
    float shopItemScale = 0.15f; // Scale factor for shop item textures
    float verticalSpacing = 5.0f; // Space between vertically stacked items
    
    // Start from the bottom and calculate positions for stacking upward
    Vector2 basePosition = {400, GetScreenHeight() - 40.0f}; // Bottom position
    Vector2 currentPosition = basePosition;
    
    // Draw box upgrade if purchased (bottom item)
    if (boxUpgradeLevel > 0) {
        float boxHeight = shopBoxTextures[boxUpgradeLevel - 1].height * shopItemScale;
        currentPosition.y -= boxHeight;
        DrawTextureEx(shopBoxTextures[boxUpgradeLevel - 1], currentPosition, 0.0f, shopItemScale, WHITE);
        currentPosition.y -= verticalSpacing;
    }
    
    // Draw boot upgrade if purchased (middle item)
    if (bootUpgradeLevel > 0) {
        float bootHeight = shopBootTextures[bootUpgradeLevel - 1].height * shopItemScale;
        currentPosition.y -= bootHeight;
        DrawTextureEx(shopBootTextures[bootUpgradeLevel - 1], currentPosition, 0.0f, shopItemScale, WHITE);
        currentPosition.y -= verticalSpacing;
    }
    
    // Draw gun upgrade if purchased (top item)
    if (gunUpgradeLevel > 0) {
        float gunHeight = shopGunTextures[gunUpgradeLevel - 1].height * shopItemScale;
        currentPosition.y -= gunHeight;
        DrawTextureEx(shopGunTextures[gunUpgradeLevel - 1], currentPosition, 0.0f, shopItemScale, WHITE);
    }
}
