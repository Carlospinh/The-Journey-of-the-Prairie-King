#include "ObstacleManager.h"

ObstacleManager::ObstacleManager() {}

void ObstacleManager::InitializeObstacles(const Rectangle& bgBounds) {
    obstacles.clear();

    float thickness = 50;
    float gapWidth = 50;
    float halfWidth = (bgBounds.width + 2 * thickness - gapWidth) / 2;

    // Añadir todos los obstáculos
    obstacles.push_back({ bgBounds.x - thickness, bgBounds.y - thickness, halfWidth, thickness });
    obstacles.push_back({ bgBounds.x - thickness + halfWidth + gapWidth, bgBounds.y - thickness, halfWidth, thickness });
    obstacles.push_back({ bgBounds.x - thickness, bgBounds.y + bgBounds.height, bgBounds.width + 2 * thickness, thickness });
    obstacles.push_back({ bgBounds.x - thickness, bgBounds.y, thickness, bgBounds.height });
    obstacles.push_back({ bgBounds.x + bgBounds.width, bgBounds.y, thickness, bgBounds.height });
    // Añadir esquinas...
}

bool ObstacleManager::CheckCollision(const Rectangle& rect) const {
    for (const auto& obstacle : obstacles) {
        if (CheckCollisionRecs(rect, obstacle)) {
            return true;
        }
    }
    return false;
}

void ObstacleManager::DrawDebug() const {
    for (const auto& obstacle : obstacles) {
        DrawRectangleLinesEx(obstacle, 2, DARKGRAY);
    }
}