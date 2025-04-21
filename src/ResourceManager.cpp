#include "ResourceManager.h"

void ResourceManager::LoadAll() {
    textures["player1"] = LoadTexture("resources/player/player1.png");
    textures["player2"] = LoadTexture("resources/player/player2.png");
    textures["player3"] = LoadTexture("resources/player/player3.png");
    textures["player4"] = LoadTexture("resources/player/player4.png");
    textures["hi"] = LoadTexture("resources/player/hi.png");

    textures["shoot_down_left"] = LoadTexture("resources/player/move_left_shoot_down.png");
    textures["shoot_left_left"] = LoadTexture("resources/player/move_left_shoot_left.png");
    textures["shoot_right_left"] = LoadTexture("resources/player/move_left_shoot_right.png");
    textures["shoot_up_left"] = LoadTexture("resources/player/move_left_shoot_up.png");
    textures["shoot_down_right"] = LoadTexture("resources/player/move_right_shoot_down.png");
    textures["shoot_left_right"] = LoadTexture("resources/player/move_right_shoot_left.png");
    textures["shoot_right_right"] = LoadTexture("resources/player/move_right_shoot_right.png");
    textures["shoot_up_right"] = LoadTexture("resources/player/move_right_shoot_up.png");
    textures["shoot_down_idle"] = LoadTexture("resources/player/stay_shoot_down.png");
    textures["shoot_left_idle"] = LoadTexture("resources/player/stay_shoot_left.png");
    textures["shoot_right_idle"] = LoadTexture("resources/player/stay_shoot_right.png");
    textures["shoot_up_idle"] = LoadTexture("resources/player/stay_shoot_up.png");

    textures["orc1"] = LoadTexture("resources/Enemies/Orc/Orc1.png"); 
    textures["orc2"] = LoadTexture("resources/Enemies/Orc/Orc2.png");
    textures["bullet"] = LoadTexture("resources/bullet/bullet.png");
    textures["coin"] = LoadTexture("resources/Coin/Coin.png");
    textures["wheel"] = LoadTexture("resources/PowerUps/Wheel.png");
    textures["live"] = LoadTexture("resources/UI/Live.png");
    textures["clock"] = LoadTexture("resources/UI/Clock.png");
    textures["arrow"] = LoadTexture("resources/UI/Arrow.png");
    textures["powerUpCartel"] = LoadTexture("resources/UI/PowerUpCartel.png");

    textures["level1"] = LoadTexture("levels/Level_1.png");
    textures["level1_2"] = LoadTexture("levels/Level_1_2.png");
    textures["level1_completed1"] = LoadTexture("levels/Level1_Completed1.png");
    textures["level1_completed2"] = LoadTexture("levels/Level1_Completed2.png");
    textures["level2"] = LoadTexture("levels/Level_2.png");
    textures["level2_2"] = LoadTexture("levels/Level_2_2.png");
}

Texture2D ResourceManager::GetTexture(const std::string& name) const {
    auto it = textures.find(name);
    if (it != textures.end()) return it->second;
    return Texture2D{};
}

void ResourceManager::UnloadAll() {
    for (auto& [key, tex] : textures) {
        UnloadTexture(tex);
    }
    textures.clear();
}