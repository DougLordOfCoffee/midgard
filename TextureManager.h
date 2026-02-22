#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include <string>

class TextureManager {
public:
    TextureManager();
    ~TextureManager();
    
    // Load a PNG image
    bool loadTexture(const std::string& name, const std::string& filePath, SDL_Renderer* renderer);
    
    // Get a loaded texture
    SDL_Texture* getTexture(const std::string& name) const;
    
    // Draw a texture at position
    void drawTexture(SDL_Renderer* renderer, const std::string& name, int x, int y, int width, int height);
    
private:
    std::map<std::string, SDL_Texture*> textures;
};

#endif
