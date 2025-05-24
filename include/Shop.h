#ifndef SHOP_H
#define SHOP_H

#include "Common.h"
#include <raylib.h>

// Forward declaration
class Player;

enum ShopState {
    SHOP_ENTERING,
    SHOP_INTERACTIVE,
    SHOP_LEAVING,
    SHOP_INACTIVE,
    SHOP_AUTO_OPENING  // New state for automatic opening
};

enum ShopItemType {
    SHOP_GUN = 0,
    SHOP_BOOT = 1,
    SHOP_BOX = 2
};

struct ShopItem {
    ShopItemType type;
    Texture2D texture;
    const char* name;
    const char* description;
    int price;
    int upgradeLevel; // 1, 2, or 3
    bool purchased;
    Vector2 position;  // Position for collision detection
    Rectangle collisionRect;  // Collision rectangle for item pickup
};

class Shop {
private:
    ShopState state;
    bool isActive;
    bool isInteractable;
    Vector2 position;
    Vector2 targetPosition;
    Vector2 startPosition;
    float moveSpeed;
    
    Texture2D npcSprites[5]; // Walking and static sprites
    Texture2D backgroundTexture;
    bool showBackground;
    bool showMenu;
    
    int currentFrame;
    float frameTime;
    float frameCounter;
    
    bool showInteractPrompt;
    bool itemsPurchasable;  // Flag to control when items can be picked up
    bool singleItemPicked;  // Track if an item has been picked up
    
    // Track which levels have already had shop visits
    bool levelShopCompleted[10];  // Index 0-9 for levels 1-10 (level 10 doesn't exist but keeping array consistent)
    
    // Shop items
    ShopItem items[3]; // Gun, Boot, Box
    int selectedItem;
    int playerCoins;
    
    // Player reference for permanent upgrades
    Player* player;
    
    // Item textures for different upgrade levels
    Texture2D gunTextures[3];
    Texture2D bootTextures[2]; // Only 2 boot levels available
    Texture2D boxTextures[3];
    
    void LoadItemTextures();
    void UnloadItemTextures();
    void InitializeItems();
    void UpdateItems();
    void DrawShopMenu();
    void HandleMenuInput();
    bool PurchaseItem(int itemIndex);
    void PositionItemsOnMap();  // New method to position items for pickup
    bool CheckPlayerItemCollision(const Rectangle& playerRect);  // Check collision with items
    
public:
    Shop();
    ~Shop();
    
    void LoadResources();
    void UnloadResources();
    
    void Activate(int playerCoins, Player* playerRef); // Pass player reference
    void Update(float deltaTime);
    void Draw();
    
    bool IsActive() const { return isActive; }
    bool IsInteractable() const { return isInteractable; }
    bool HandleInteraction(); // Returns true if interaction is complete
    
    // Get purchased items info for UI display
    bool HasGunUpgrade() const;
    bool HasBootUpgrade() const;
    bool HasBoxUpgrade() const;
    
    int GetGunUpgradeLevel() const;
    int GetBootUpgradeLevel() const;
    int GetBoxUpgradeLevel() const;
    
    int GetCoinsSpent() const; // Return coins spent in this transaction
    int GetRemainingCoins() const { return playerCoins; } // Get remaining coins after purchases
    
    // New method for player collision detection
    bool CheckItemPickup(const Rectangle& playerRect);
    
    // Methods for tracking shop visits per level
    bool IsShopAvailableForLevel(int level) const;
    void MarkShopCompletedForLevel(int level);
    void ResetShopProgress(); // For game restart
};

#endif // SHOP_H