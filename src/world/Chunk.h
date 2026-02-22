#ifndef CHUNK_H
#define CHUNK_H

#include "world/Tile.h"
#include "render/TextureManager.h"
#include <SDL3/SDL.h>

const int CHUNK_SIZE = 64;      // 64x64 tiles per chunk
const int TILE_SIZE = 32;       // 32x32 pixels per tile

// Forward declaration
enum class ChunkType;

class Chunk {
public:
    Chunk() = default;
    Chunk(int chunkX, int chunkY, ChunkType biomeType);  // Load from biome type
    
    void setTile(int x, int y, TileType type);
    Tile getTile(int x, int y) const;
    void render(SDL_Renderer* renderer, int cameraX, int cameraY, int viewWidth, int viewHeight, TextureManager& textureManager);
    
    int getWorldX() const;
    int getWorldY() const;
    
private:
    Tile tiles[CHUNK_SIZE][CHUNK_SIZE];
    int chunkX, chunkY;
    
    void generateFromBiome(ChunkType biomeType);
};

#endif
