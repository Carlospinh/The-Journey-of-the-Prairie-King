#include "LevelManager.h"

void LevelManager::Init(ResourceManager& resources) {
    backgrounds[0] = resources.GetTexture("level1");
    backgrounds[1] = resources.GetTexture("level1_2");
    completedBG[0] = resources.GetTexture("level1_completed1");
    completedBG[1] = resources.GetTexture("level1_completed2");
    level2[0] = resources.GetTexture("level2");
    level2[1] = resources.GetTexture("level2_2");

    scale = 3.8f;
    current = 0;
    frameTimer = 0.0f;
    frameTime = 0.5f;
    transitionProgress = 0.0f;
    transitioning = false;
}

void LevelManager::DrawBackground() {
    Rectangle src = { 0, 0, (float)backgrounds[current].width, (float)backgrounds[current].height };
    Rectangle dst = { (SCREEN_WIDTH - backgrounds[current].width * scale) / 2,
                      (SCREEN_HEIGHT - backgrounds[current].height * scale) / 2,
                      backgrounds[current].width * scale,
                      backgrounds[current].height * scale };

    if (transitioning) {
        float bgH = backgrounds[current].height * scale;
        float bgY = (SCREEN_HEIGHT - bgH) / 2;
        float eased = 1.0f - powf(1.0f - transitionProgress, 3);
        float offsetY = bgY + bgH - (bgH * eased);

        DrawTexturePro(backgrounds[current], src, { dst.x, offsetY - bgH, dst.width, dst.height }, { 0, 0 }, 0.0f, WHITE);
        DrawTexturePro(level2[0], src, { dst.x, offsetY, dst.width, dst.height }, { 0, 0 }, 0.0f, WHITE);
    }
    else {
        DrawTexturePro(backgrounds[current], src, dst, { 0, 0 }, 0.0f, WHITE);
    }
}

void LevelManager::UpdateTransition(float deltaTime) {
    if (!transitioning) return;

    transitionProgress += deltaTime / 1.5f;
    if (transitionProgress >= 1.0f) {
        transitionProgress = 0.0f;
        transitioning = false;
        backgrounds[0] = level2[0];
        backgrounds[1] = level2[1];
    }

    frameTimer += deltaTime;
    if (frameTimer >= frameTime) {
        frameTimer = 0.0f;
        current = (current + 1) % 2;
    }
}

bool LevelManager::ShouldTransition(const Player& player) {
    float bgW = backgrounds[0].width * scale;
    float bgH = backgrounds[0].height * scale;
    float bgX = (SCREEN_WIDTH - bgW) / 2;
    float bgY = (SCREEN_HEIGHT - bgH) / 2;

    Rectangle zone = { bgX + bgW * 0.4f, bgY + bgH - 50, bgW * 0.2f, 50 };
    Rectangle p = { player.GetPosition().x, player.GetPosition().y, 40, 40 };

    return !transitioning && zone.x < p.x + p.width && zone.x + zone.width > p.x &&
        zone.y < p.y + p.height && zone.y + zone.height > p.y;
}

void LevelManager::StartTransition() {
    transitioning = true;
    transitionProgress = 0.0f;
}

void LevelManager::DrawOverlay(const Player& player) {
    if (transitioning) return;

    float bgW = backgrounds[0].width * scale;
    float bgH = backgrounds[0].height * scale;
    float bgX = (SCREEN_WIDTH - bgW) / 2;
    float bgY = (SCREEN_HEIGHT - bgH) / 2;

    float arrowScale = 0.18f;
    Texture2D arrow = GetArrowTexture();

    if (((int)(GetTime() * 2) % 2) == 0) {
        Vector2 pos = {
            bgX + bgW / 2 - (arrow.width * arrowScale / 2) + 30,
            bgY + bgH - (arrow.height * arrowScale) - 20
        };
        DrawTextureEx(arrow, pos, 0.0f, arrowScale, WHITE);
    }
}

void LevelManager::Reset() {
    Init(*resourceManager);
}

Texture2D LevelManager::GetArrowTexture() const {
    return resourceManager->GetTexture("arrow");
}
