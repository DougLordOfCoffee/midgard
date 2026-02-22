#ifndef CHUNK_H
#define CHUNK_H

#include "Tile.h"
#include "NoiseGenerator.h"
#include <SDL2/SDL.h>

const int CHUNK_SIZE = 64;      // 64x64 tiles per chunk
const int TILE_SIZE = 32;       // 32x32 pixels per tile

class Chunk {
public:
    Chunk() = default;
    Chunk(int chunkX, int chunkY, NoiseGenerator& noise);
    
    void setTile(int x, int y, TileType type);
    Tile getTile(int x, int y) const;
    void render(SDL_Renderer* renderer, int cameraX, int cameraY, int viewWidth, int viewHeight);
    
    int getWorldX() const;
    int getWorldY() const;
    
private:
    Tile tiles[CHUNK_SIZE][CHUNK_SIZE];
    int chunkX, chunkY;
    
    void generateDefault(NoiseGenerator& noise);
};

#endif
