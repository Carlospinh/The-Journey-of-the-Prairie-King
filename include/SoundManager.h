#pragma once

#include "raylib.h"

class SoundManager {
public:
    void LoadSounds();
    void PlayMusic();
    void UpdateMusic();
    void RestartMusic();
    void Cleanup();

private:
    Sound startSound;
    Sound footstepSound;
    Sound gunshotSound;
    Sound hitSound;
    Sound coinSound;
    Sound pickupPowerUpSound;

    Music music;
    bool isPlaying;
};  