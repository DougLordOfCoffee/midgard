#ifndef TILEMAP_H
#define TILEMAP_H

#include "Chunk.h"
#include "WorldLoader.h"
#include "TextureManager.h"
#include <map>

class TileMap {
public:
    TileMap(const std::string& worldFile, TextureManager& textureManager);
    
    void setTile(int worldX, int worldY, TileType type);
    Tile getTile(int worldX, int worldY) const;
    void render(SDL_Renderer* renderer, int cameraX, int cameraY, int viewWidth, int viewHeight);
    void updateLoadedChunks(int cameraX, int cameraY);  // Load/unload based on camera
    
private:
    std::map<std::pair<int, int>, Chunk> loadedChunks;  // Only chunks near camera
    WorldLoader worldLoader;
    TextureManager& textureManager;
    int lastLoadedCameraX = -99999;
    int lastLoadedCameraY = -99999;
    
    Chunk& getOrCreateChunk(int chunkX, int chunkY);
};

#endif
