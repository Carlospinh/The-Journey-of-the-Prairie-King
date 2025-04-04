#ifndef OBSTACLE_MANAGER_H
#define OBSTACLE_MANAGER_H

#include "raylib.h"
#include <vector>

class ObstacleManager {
private:
    std::vector<Rectangle> obstacles;

public:
    ObstacleManager();
    void InitializeObstacles(const Rectangle& backgroundBounds);
    bool CheckCollision(const Rectangle& rect) const;
    void DrawDebug() const; // Para visualización de obstáculos
};

#endif