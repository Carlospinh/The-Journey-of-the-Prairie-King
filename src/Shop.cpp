#include "../include/Shop.h"
#include "../include/Player.h"
#include <iostream>

Shop::Shop() {
    state = SHOP_INACTIVE;
    isActive = false;
    isInteractable = false;
    position = {SCREEN_WIDTH / 2, -100};
    targetPosition = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150};
    startPosition = {SCREEN_WIDTH / 2, -100};
    moveSpeed = 150.0f;
    
    currentFrame = 0;
    frameTime = 0.2f;
    frameCounter = 0.0f;
    
    showInteractPrompt = false;
    showBackground = false;
    showMenu = false;
    selectedItem = 0;
    playerCoins = 0;
    player = nullptr;
    itemsPurchasable = false;
    singleItemPicked = false;
}

Shop::~Shop() {
    UnloadResources();
}

void Shop::LoadResources() {
    npcSprites[0] = LoadTexture("resources/Store/NPC/StoreNPC_Static_Left.png");
    npcSprites[1] = LoadTexture("resources/Store/NPC/StoreNPC_Static.png");
    npcSprites[2] = LoadTexture("resources/Store/NPC/StoreNPC_Static_Right.png");
    npcSprites[3] = LoadTexture("resources/Store/NPC/StoreNPC_Walking1.png");
    npcSprites[4] = LoadTexture("resources/Store/NPC/StoreNPC_Walking2.png");
    
    backgroundTexture = LoadTexture("resources/Store/Store BackGround.png");
    LoadItemTextures();
}

void Shop::LoadItemTextures() {
    gunTextures[0] = LoadTexture("resources/Store/Gun/Store_Gun_1.png");
    gunTextures[1] = LoadTexture("resources/Store/Gun/Store_Gun_2.png");
    gunTextures[2] = LoadTexture("resources/Store/Gun/Store_Gun_3.png");
    
    bootTextures[0] = LoadTexture("resources/Store/Bota/Store_Bota_1.png");
    bootTextures[1] = LoadTexture("resources/Store/Bota/Store_Bota_2.png");
    
    boxTextures[0] = LoadTexture("resources/Store/Box/Store_Box_1.png");
    boxTextures[1] = LoadTexture("resources/Store/Box/Store_Box_2.png");
    boxTextures[2] = LoadTexture("resources/Store/Box/Store_Box_3.png");
}

void Shop::UnloadResources() {
    for (int i = 0; i < 5; i++) {
        if (npcSprites[i].id != 0) {
            UnloadTexture(npcSprites[i]);
        }
    }
    
    if (backgroundTexture.id != 0) {
        UnloadTexture(backgroundTexture);
    }
    
    UnloadItemTextures();
}

void Shop::UnloadItemTextures() {
    for (int i = 0; i < 3; i++) {
        if (gunTextures[i].id != 0) UnloadTexture(gunTextures[i]);
        if (boxTextures[i].id != 0) UnloadTexture(boxTextures[i]);
    }
    for (int i = 0; i < 2; i++) {
        if (bootTextures[i].id != 0) UnloadTexture(bootTextures[i]);
    }
}

void Shop::Activate(int coins, Player* playerRef) {
    isActive = true;
    state = SHOP_ENTERING;
    playerCoins = coins;
    player = playerRef;
    position = startPosition;
    targetPosition = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150};
    moveSpeed = 150.0f;
    showBackground = false;
    showMenu = false;
    isInteractable = false;
    showInteractPrompt = false;
    selectedItem = 0;
    InitializeItems();
}

void Shop::InitializeItems() {
    if (!player) return;
    
    int gunLevel = player->GetGunUpgradeLevel();
    int bootLevel = player->getBootUpgradeLevel();
    int boxLevel = player->GetBoxUpgradeLevel();
    
    int gunPrice = (gunLevel == 0) ? 10 : ((gunLevel == 1) ? 20 : 30);
    int bootPrice = (bootLevel == 0) ? 8 : 20;
    int boxPrice = (boxLevel == 0) ? 15 : ((boxLevel == 1) ? 30 : 45);
    
    items[0] = {SHOP_GUN, gunTextures[gunLevel < 3 ? gunLevel : 2], 
                "Gun Upgrade", "Increases damage", gunPrice, gunLevel + 1, false};
    items[1] = {SHOP_BOOT, bootTextures[bootLevel < 2 ? bootLevel : 1], 
                "Boot Upgrade", "Increases speed", bootPrice, bootLevel + 1, false};
    items[2] = {SHOP_BOX, boxTextures[boxLevel < 3 ? boxLevel : 2], 
                "Penetration", "Bullet penetration+damage", boxPrice, boxLevel + 1, false};
                
    if (gunLevel >= 3) items[0].price = -1;
    if (bootLevel >= 2) items[1].price = -1;
    if (boxLevel >= 3) items[2].price = -1;
}

void Shop::Update(float deltaTime) {
    if (!isActive) return;
    
    switch (state) {
        case SHOP_ENTERING:
            if (position.y < targetPosition.y) {
                position.y += moveSpeed * deltaTime;
                
                frameCounter += deltaTime;
                if (frameCounter >= frameTime) {
                    frameCounter = 0.0f;
                    currentFrame = (currentFrame >= 3 && currentFrame <= 4) ? 
                                  ((currentFrame == 3) ? 4 : 3) : 3;
                }
                
                if (position.y >= targetPosition.y) {
                    position.y = targetPosition.y;
                    state = SHOP_AUTO_OPENING;  // Changed to auto-opening state
                    isInteractable = false;
                    showInteractPrompt = false;
                    showBackground = false;
                    currentFrame = 1;
                }
            }
            break;
            
        case SHOP_AUTO_OPENING:
            // Automatically open shop and position items after NPC stops
            showMenu = true;
            showBackground = true;
            PositionItemsOnMap();
            state = SHOP_INTERACTIVE;
            itemsPurchasable = true;
            singleItemPicked = false;
            break;
            
        case SHOP_INTERACTIVE:
            // No menu input needed - items are on the map for pickup
            if (singleItemPicked) {
                // Start leaving after an item is picked
                state = SHOP_LEAVING;
                targetPosition = startPosition;
                moveSpeed = 200.0f;
                showBackground = false;
                showMenu = false;
                isInteractable = false;
                itemsPurchasable = false;
            }
            break;
            
        case SHOP_LEAVING:
            if (position.y > startPosition.y) {
                position.y -= moveSpeed * deltaTime;
                
                frameCounter += deltaTime;
                if (frameCounter >= frameTime) {
                    frameCounter = 0.0f;
                    currentFrame = (currentFrame >= 3 && currentFrame <= 4) ? 
                                  ((currentFrame == 3) ? 4 : 3) : 3;
                }
                
                if (position.y <= startPosition.y) {
                    position.y = startPosition.y;
                    state = SHOP_INACTIVE;
                    isActive = false;
                }
            }
            break;
    }
}

void Shop::HandleMenuInput() {
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        selectedItem = (selectedItem - 1 + 3) % 3;
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        selectedItem = (selectedItem + 1) % 3;
    }
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        if (PurchaseItem(selectedItem)) {
            showMenu = false;
            state = SHOP_LEAVING;
            targetPosition = startPosition;
            moveSpeed = 200.0f;
            showBackground = false;
            isInteractable = false;
        }
    }
    if (IsKeyPressed(KEY_BACKSPACE)) {
        showMenu = false;
        state = SHOP_LEAVING;
        targetPosition = startPosition;
        moveSpeed = 200.0f;
        showBackground = false;
        isInteractable = false;
    }
}

bool Shop::PurchaseItem(int itemIndex) {
    if (itemIndex < 0 || itemIndex >= 3) return false;
    if (items[itemIndex].price == -1) return false;
    if (playerCoins < items[itemIndex].price) return false;
    if (!player) return false;
    
    playerCoins -= items[itemIndex].price;
    items[itemIndex].purchased = true;
    
    // Apply permanent upgrades to player
    switch (items[itemIndex].type) {
        case SHOP_GUN:
            player->UpgradeGun();
            break;
        case SHOP_BOOT:
            player->UpgradeBoot();
            break;
        case SHOP_BOX:
            player->UpgradeBox();
            break;
    }
    
    return true;
}


void Shop::Draw() {
    if (!isActive) return;
    
    // Draw smaller shop background when active
    if (showBackground) {
        float bgScale = 0.25f; // Further reduced from 0.4f to make shop much smaller
        float bgWidth = backgroundTexture.width * bgScale;
        float bgHeight = backgroundTexture.height * bgScale;
        
        Vector2 bgPos = {
            (SCREEN_WIDTH - bgWidth) / 2,
            (SCREEN_HEIGHT - bgHeight) / 2 - 20 // Reduced offset
        };
        
        DrawTextureEx(backgroundTexture, bgPos, 0.0f, bgScale, WHITE);
    }
    
    // Draw NPC sprite (smaller)
    float scale = 0.08f; // Further reduced NPC sprite size
    DrawTextureEx(npcSprites[currentFrame], position, 0.0f, scale, WHITE);
    
    // Draw items on the map for pickup (only when itemsPurchasable is true)
    if (itemsPurchasable && !singleItemPicked) {
        for (int i = 0; i < 3; i++) {
            if (items[i].price != -1 && playerCoins >= items[i].price && !items[i].purchased) {
                // Draw item on the map
                float itemScale = 0.15f; // Small scale for map items
                Vector2 itemPos = {
                    items[i].position.x - (items[i].texture.width * itemScale) / 2,
                    items[i].position.y - (items[i].texture.height * itemScale) / 2
                };
                
                // Draw a background circle for the item
                DrawCircle(items[i].position.x, items[i].position.y, 20, Fade(YELLOW, 0.3f));
                DrawCircleLines(items[i].position.x, items[i].position.y, 20, YELLOW);
                
                // Draw the item texture
                DrawTextureEx(items[i].texture, itemPos, 0.0f, itemScale, WHITE);
                
                // Draw price text above the item
                const char* priceText = TextFormat("%d", items[i].price);
                int fontSize = 16;
                Vector2 textSize = MeasureTextEx(GetFontDefault(), priceText, fontSize, 1);
                DrawText(priceText, items[i].position.x - textSize.x / 2, items[i].position.y - 35, fontSize, GOLD);
                
                // Draw item name below
                int nameSize = 12;
                Vector2 nameTextSize = MeasureTextEx(GetFontDefault(), items[i].name, nameSize, 1);
                DrawText(items[i].name, items[i].position.x - nameTextSize.x / 2, items[i].position.y + 25, nameSize, WHITE);
            }
        }
    }
}

void Shop::DrawShopMenu() {
    float menuX = SCREEN_WIDTH / 2 - 300; // Reduced from 400
    float menuY = SCREEN_HEIGHT / 2 - 80; // Reduced from 100
    
    DrawText("SHOP", menuX + 250, menuY - 40, 30, WHITE); // Reduced font size
    DrawText(TextFormat("Coins: %d", playerCoins), menuX + 200, menuY - 15, 20, GOLD); // Reduced font size
    
    for (int i = 0; i < 3; i++) {
        float itemX = menuX + i * 200; // Reduced from 250
        float itemY = menuY;
        
        Color bgColor = (i == selectedItem) ? DARKGRAY : BLACK;
        Color borderColor = (i == selectedItem) ? WHITE : GRAY;
        
        DrawRectangle(itemX - 8, itemY - 8, 180, 150, bgColor); // Reduced size
        DrawRectangleLines(itemX - 8, itemY - 8, 180, 150, borderColor);
        
        if (items[i].texture.id != 0) {
            float scale = 0.25f; // Reduced from 0.3f
            DrawTextureEx(items[i].texture, {itemX + 50, itemY + 5}, 0.0f, scale, WHITE);
        }
        
        DrawText(items[i].name, itemX, itemY + 65, 14, WHITE); // Reduced font size
        DrawText(items[i].description, itemX, itemY + 80, 10, LIGHTGRAY); // Reduced font size
        
        if (items[i].price == -1) {
            DrawText("MAX LEVEL", itemX + 20, itemY + 100, 12, RED); // Reduced font size
        } else {
            Color priceColor = (playerCoins >= items[i].price) ? GREEN : RED;
            DrawText(TextFormat("Price: %d", items[i].price), itemX, itemY + 100, 14, priceColor); // Reduced font size
        }
        
        DrawText(TextFormat("Level %d", items[i].upgradeLevel), itemX, itemY + 120, 10, YELLOW); // Reduced font size
    }
    
    DrawText("A/D to select, ENTER to buy, ESC to exit", 
             menuX - 30, menuY + 160, 16, LIGHTGRAY); // Reduced font size and repositioned
}

bool Shop::HandleInteraction() {
    if (!isInteractable || state != SHOP_INTERACTIVE) return false;
    
    if (IsKeyPressed(KEY_E)) {
        if (!showMenu) {
            showMenu = true;
            showInteractPrompt = false;
            return false;
        }
    }
    
    return false;
}

int Shop::GetCoinsSpent() const {
    int totalSpent = 0;
    for (int i = 0; i < 3; i++) {
        if (items[i].purchased) {
            totalSpent += items[i].price;
        }
    }
    return totalSpent;
}

bool Shop::HasGunUpgrade() const {
    return player ? player->GetGunUpgradeLevel() > 0 : false;
}

bool Shop::HasBootUpgrade() const {
    return player ? player->getBootUpgradeLevel() > 0 : false;
}

bool Shop::HasBoxUpgrade() const {
    return player ? player->GetBoxUpgradeLevel() > 0 : false;
}

int Shop::GetGunUpgradeLevel() const {
    return player ? player->GetGunUpgradeLevel() : 0;
}

int Shop::GetBootUpgradeLevel() const {
    return player ? player->getBootUpgradeLevel() : 0;
}

int Shop::GetBoxUpgradeLevel() const {
    return player ? player->GetBoxUpgradeLevel() : 0;
}
void Shop::PositionItemsOnMap() {
    // Position items around the shop area for pickup
    float centerX = SCREEN_WIDTH / 2;
    float centerY = SCREEN_HEIGHT / 2;
    
    // Position items in a horizontal line
    float spacing = 100.0f; // Distance between items
    float startX = centerX - spacing; // Start position for 3 items
    
    for (int i = 0; i < 3; i++) {
        if (items[i].price != -1 && playerCoins >= items[i].price) {
            items[i].position.x = startX + (i * spacing);
            items[i].position.y = centerY;
            
            // Set collision rectangle for each item (smaller size)
            float itemSize = 32.0f; // Smaller item size for pickup
            items[i].collisionRect = {
                items[i].position.x - itemSize/2,
                items[i].position.y - itemSize/2,
                itemSize,
                itemSize
            };
        }
    }
}

bool Shop::CheckItemPickup(const Rectangle& playerRect) {
    if (!itemsPurchasable || singleItemPicked) return false;
    
    for (int i = 0; i < 3; i++) {
        if (items[i].price != -1 && playerCoins >= items[i].price && !items[i].purchased) {
            if (CheckCollisionRecs(playerRect, items[i].collisionRect)) {
                // Purchase the item
                if (PurchaseItem(i)) {
                    singleItemPicked = true;
                    return true;
                }
            }
        }
    }
    return false;
}
bool Shop::IsShopAvailableForLevel(int level) const {
    // Shop is available on levels 1, 4, 6, 8 and only if not already completed
    if (/*level == 1 || */  level == 2 ||     level == 4||level == 6|| level == 8) {
        if (level >= 1 && level <= 9) {
            return !levelShopCompleted[level - 1]; // Return true if not completed yet
        }
    }
    return false;
}





void Shop::MarkShopCompletedForLevel(int level) {
    if (level >= 1 && level <= 9) {
        levelShopCompleted[level - 1] = true; // Convert to 0-based index
    }
}

void Shop::ResetShopProgress() {
    // Reset all shop completion flags - called when starting a new game
    for (int i = 0; i < 10; i++) {
        levelShopCompleted[i] = false;
    }
}
