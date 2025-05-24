#ifndef BEAVER_H
#define BEAVER_H

#include "raylib.h"
#include <vector>

class Beaver {
private:
int friendId;           // Which friend sprite we're using
int animationFrames;
    Vector2 position;
    Texture2D texture;
    bool active;
    float speed;
    float scale;
    float frameWidth;
    float frameHeight;
    
    // Animation variables
    int currentFrame;
    int framesCounter;
    int framesSpeed;
    bool facingLeft;
    
    // Path variables
    std::vector<Vector2> path;
    int currentPathPoint;
    float appearanceTimer;
    float disappearanceTimer;
    bool isDisappearing;
    
public:
    Beaver();
    ~Beaver();
    
    void LoadTexture(int friendId);
    void UnloadTexture();
    
    void Activate(const std::vector<Vector2>& newPath);
    void Deactivate();
    
    void Update(float deltaTime);
    void Draw();
    
    bool IsActive() const { return active; }
    Vector2 GetPosition() const { return position; }
    Rectangle GetCollisionRect() const;
    
    static void LoadSharedResources();
    static void UnloadSharedResources();
};

#endif // BEAVER_H