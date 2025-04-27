#include "SoundManager.h"

void SoundManager::LoadSounds() {
    startSound = LoadSound("resources/Sounds/start.wav");
    footstepSound = LoadSound("resources/Sounds/Footstep.wav");
    gunshotSound = LoadSound("resources/Sounds/Gunshot.wav");
    hitSound = LoadSound("resources/Sounds/Hit.wav");
    coinSound = LoadSound("resources/Sounds/Coin.wav");
    pickupPowerUpSound = LoadSound("resources/Sounds/PickUpPowerUp.wav");

    SetSoundVolume(footstepSound, 0.5f);
    SetSoundVolume(gunshotSound, 0.7f);
    SetSoundVolume(hitSound, 0.6f);
    SetSoundVolume(coinSound, 0.5f);
    SetSoundVolume(pickupPowerUpSound, 0.6f);

    music = LoadMusicStream("resources/BKMusic/TheOutlaw.mp3");
    music.looping = true;
    isPlaying = false;
}

void SoundManager::PlayMusic() {
    PlayMusicStream(music);
    SetMusicVolume(music, 0.5f);
    isPlaying = true;
}

void SoundManager::RestartMusic() {
    StopMusicStream(music);
    PlayMusic();
}

void SoundManager::UpdateMusic() {
    if (isPlaying) UpdateMusicStream(music);
    if (!IsMusicStreamPlaying(music)) PlayMusicStream(music);
}

void SoundManager::Cleanup() {
    UnloadSound(startSound);
    UnloadSound(footstepSound);
    UnloadSound(gunshotSound);
    UnloadSound(hitSound);
    UnloadSound(coinSound);
    UnloadSound(pickupPowerUpSound);

    StopMusicStream(music);
    UnloadMusicStream(music);
}
