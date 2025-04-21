#pragma once

#include "raylib.h"
#include <string>
#include <unordered_map>

class ResourceManager {
public:
    void LoadAll();
    Texture2D GetTexture(const std::string& name) const;
    void UnloadAll();

private:
    std::unordered_map<std::string, Texture2D> textures;
}; 