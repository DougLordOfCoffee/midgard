#include "render/TextureManager.h"
#include <stdio.h>

TextureManager::TextureManager() {}

TextureManager::~TextureManager() {
    // Clean up all loaded textures
    for (auto& pair : textures) {
        if (pair.second) {
            SDL_DestroyTexture(pair.second);
        }
    }
    textures.clear();
}

bool TextureManager::loadTexture(const std::string& name, const std::string& filePath, SDL_Renderer* renderer) {
    // Load image from file
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        printf("Failed to load image '%s': %s\n", filePath.c_str(), SDL_GetError());
        return false;
    }
    
    // Convert surface to texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    
    if (!texture) {
        printf("Failed to create texture from '%s': %s\n", filePath.c_str(), SDL_GetError());
        return false;
    }
    
    textures[name] = texture;
    printf("Loaded texture '%s' from '%s'\n", name.c_str(), filePath.c_str());
    return true;
}

SDL_Texture* TextureManager::getTexture(const std::string& name) const {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second;
    }
    printf("Warning: Texture '%s' not found\n", name.c_str());
    return nullptr;
}

void TextureManager::drawTexture(SDL_Renderer* renderer, const std::string& name, int x, int y, int width, int height) {
    SDL_Texture* texture = getTexture(name);
    if (!texture) return;
    
    SDL_FRect dest = {(float)x, (float)y, (float)width, (float)height};
    SDL_RenderTexture(renderer, texture, nullptr, &dest);
}
