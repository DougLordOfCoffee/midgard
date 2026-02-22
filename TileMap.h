#ifndef TILEMAP_H
#define TILEMAP_H

#include "Chunk.h"
#include "NoiseGenerator.h"
#include <map>

class TileMap {
public:
    TileMap(int chunkWidthCount, int chunkHeightCount);
    
    void setTile(int worldX, int worldY, TileType type);
    Tile getTile(int worldX, int worldY) const;
    void render(SDL_Renderer* renderer, int cameraX, int cameraY, int viewWidth, int viewHeight);
    
private:
    std::map<std::pair<int, int>, Chunk> chunks;
    NoiseGenerator noise;
    int chunkWidthCount, chunkHeightCount;
    
    Chunk& getOrCreateChunk(int chunkX, int chunkY);
};

#endif
