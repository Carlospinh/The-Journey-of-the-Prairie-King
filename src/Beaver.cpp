#include "../include/Beaver.h"
#include <cmath>

// Static variables - now we need more textures for animation frames
static Texture2D beaverTextures[11]; // Friend_1 through Friend_11
static bool resourcesLoaded = false;

Beaver::Beaver()
    : position({0, 0}),
      active(false),
      speed(200.0f), 
      scale(0.20f),
      frameWidth(16.0f),
      frameHeight(16.0f),
      currentFrame(0),
      framesCounter(0),
      framesSpeed(12),
      facingLeft(false),
      currentPathPoint(0),
      appearanceTimer(0),
      disappearanceTimer(0),
      isDisappearing(false),
      friendId(1),
      animationFrames(11) {
}

Beaver::~Beaver() {
    // Individual beaver instances don't need to unload shared textures
}

void Beaver::LoadTexture(int selectedFriendId) {
    if (selectedFriendId < 1 || selectedFriendId > 11) selectedFriendId = 1;
    friendId = selectedFriendId;
    
    // Start with the selected friend texture
    texture = beaverTextures[friendId - 1];
    
    // Since each file is a single sprite, frame dimensions are the full texture
    frameWidth = static_cast<float>(texture.width);
    frameHeight = static_cast<float>(texture.height);
    
    // Use all 11 friends as animation frames
    animationFrames = 11;
}

void Beaver::LoadSharedResources() {
    if (!resourcesLoaded) {
        beaverTextures[0] = ::LoadTexture("resources/Friends/Friend_1.png");
        beaverTextures[1] = ::LoadTexture("resources/Friends/Friend_2.png");
        beaverTextures[2] = ::LoadTexture("resources/Friends/Friend_3.png");
        beaverTextures[3] = ::LoadTexture("resources/Friends/Friend_4.png");
        beaverTextures[4] = ::LoadTexture("resources/Friends/Friend_5.png");
        beaverTextures[5] = ::LoadTexture("resources/Friends/Friend_6.png");
        beaverTextures[6] = ::LoadTexture("resources/Friends/Friend_7.png");
        beaverTextures[7] = ::LoadTexture("resources/Friends/Friend_8.png");
        beaverTextures[8] = ::LoadTexture("resources/Friends/Friend_9.png");
        beaverTextures[9] = ::LoadTexture("resources/Friends/Friend_10.png");
        beaverTextures[10] = ::LoadTexture("resources/Friends/Friend_11.png");
        resourcesLoaded = true;
    }
}

void Beaver::UnloadSharedResources() {
    if (resourcesLoaded) {
        for (int i = 0; i < 11; i++) {
            ::UnloadTexture(beaverTextures[i]);
        }
        resourcesLoaded = false;
    }
}

void Beaver::Activate(const std::vector<Vector2>& newPath) {
    if (newPath.size() < 2) return;
    
    active = true;
    path = newPath;
    currentPathPoint = 0;
    position = path[0];
    isDisappearing = false;
    appearanceTimer = 0;
    disappearanceTimer = 3.0f;
    
    // Random friend variant (1-11)
    int selectedFriendId = GetRandomValue(1, 11);
    LoadTexture(selectedFriendId);
}

void Beaver::Deactivate() {
    active = false;
    currentFrame = 0;
    framesCounter = 0;
    currentPathPoint = 0;
    isDisappearing = false;
    path.clear();
}

void Beaver::Update(float deltaTime) {
    if (!active) return;
    
    // Update animation - cycle through all 11 friend textures
    framesCounter++;
    if (framesCounter >= 60/framesSpeed) {
        framesCounter = 0;
        currentFrame++;
        if (currentFrame >= animationFrames) currentFrame = 0;
        
        // Switch to the next friend texture for animation
        int currentTextureIndex = currentFrame;
        texture = beaverTextures[currentTextureIndex];
        
        // Update frame dimensions in case friend sprites have different sizes
        frameWidth = static_cast<float>(texture.width);
        frameHeight = static_cast<float>(texture.height);
    }
    
    // Path following logic - deactivate immediately when reaching the end
    if (currentPathPoint >= path.size() - 1) {
        // Beaver has reached its destination, deactivate immediately
        Deactivate();
        return;
    }
    
    Vector2 targetPoint = path[currentPathPoint + 1];
    Vector2 direction = {
        targetPoint.x - position.x,
        targetPoint.y - position.y
    };
    
    float distance = sqrtf(direction.x * direction.x + direction.y * direction.y);
    
    if (direction.x < 0) facingLeft = true;
    else if (direction.x > 0) facingLeft = false;
    
    if (distance < speed * deltaTime) {
        position = targetPoint;
        currentPathPoint++;
    } else {
        direction.x /= distance;
        direction.y /= distance;
        position.x += direction.x * speed * deltaTime;
        position.y += direction.y * speed * deltaTime;
    }
}

void Beaver::Draw() {
    if (!active) return;
    
    // Source rectangle covers the entire texture (single sprite)
    Rectangle source = {
        0,
        0,
        frameWidth,
        frameHeight
    };
    
    Rectangle dest = {
        position.x,
        position.y,
        frameWidth * scale,
        frameHeight * scale
    };
    
    Vector2 origin = {0, 0};
    
    // Handle horizontal flipping
    if (facingLeft) {
        dest.width = -(frameWidth * scale);
    }
    
    DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
}

Rectangle Beaver::GetCollisionRect() const {
    return Rectangle{
        position.x + frameWidth * scale * 0.25f,
        position.y + frameHeight * scale * 0.25f,
        frameWidth * scale * 0.5f,
        frameHeight * scale * 0.5f
    };
}