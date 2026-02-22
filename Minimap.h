#ifndef MINIMAP_H
#define MINIMAP_H

#include <SDL.h>
#include "WorldLoader.h"
#include <string>

class Minimap {
public:
    Minimap(const std::string& worldFile, SDL_Renderer* renderer);
    ~Minimap();
    
    void render(SDL_Renderer* renderer, int playerChunkX, int playerChunkY);
    
private:
    SDL_Texture* minimapTexture;
    int worldWidth, worldHeight;
    const int MINIMAP_SIZE = 100;  // 100x100 pixels
    
    SDL_Color getBiomeColor(ChunkType biome) const;
};

#endif
