#include "TileMap.h"
#include <stdio.h>

TileMap::TileMap(const std::string& worldFile, TextureManager& textureManager)
    : textureManager(textureManager) {
    // Load world from file
    if (!worldLoader.loadWorld(worldFile)) {
        printf("Warning: Could not load world file '%s'\n", worldFile.c_str());
    }
}

Chunk& TileMap::getOrCreateChunk(int chunkX, int chunkY) {
    auto key = std::make_pair(chunkX, chunkY);
    if (loadedChunks.find(key) == loadedChunks.end()) {
        // Get biome type from world loader
        ChunkType biomeType = worldLoader.getChunkType(chunkX, chunkY);
        loadedChunks[key] = Chunk(chunkX, chunkY, biomeType);
    }
    return loadedChunks[key];
}

void TileMap::updateLoadedChunks(int cameraX, int cameraY) {
    // Only update if camera moved significantly
    if (abs(cameraX - lastLoadedCameraX) < CHUNK_SIZE * TILE_SIZE && 
        abs(cameraY - lastLoadedCameraY) < CHUNK_SIZE * TILE_SIZE) {
        return;
    }
    
    lastLoadedCameraX = cameraX;
    lastLoadedCameraY = cameraY;
    
    // Calculate which chunks are visible (with buffer)
    int chunkX = cameraX / (CHUNK_SIZE * TILE_SIZE);
    int chunkY = cameraY / (CHUNK_SIZE * TILE_SIZE);
    
    // Load nearby chunks (with 2-chunk buffer for smooth loading)
    int buffer = 2;
    for (int y = chunkY - buffer; y <= chunkY + buffer; y++) {
        for (int x = chunkX - buffer; x <= chunkX + buffer; x++) {
            getOrCreateChunk(x, y);
        }
    }
    
    // Unload chunks that are too far away
    auto it = loadedChunks.begin();
    while (it != loadedChunks.end()) {
        int cx = it->first.first;
        int cy = it->first.second;
        
        // If chunk is more than buffer*2 away, unload it
        if (abs(cx - chunkX) > buffer * 2 || abs(cy - chunkY) > buffer * 2) {
            it = loadedChunks.erase(it);
        } else {
            ++it;
        }
    }
    
    // Debug: show how many chunks are loaded
    printf("Camera at chunk (%d, %d) - Loaded chunks: %zu\n", chunkX, chunkY, loadedChunks.size());
}

void TileMap::setTile(int worldX, int worldY, TileType type) {
    int chunkX = worldX / (CHUNK_SIZE * TILE_SIZE);
    int chunkY = worldY / (CHUNK_SIZE * TILE_SIZE);
    int localX = (worldX / TILE_SIZE) % CHUNK_SIZE;
    int localY = (worldY / TILE_SIZE) % CHUNK_SIZE;
    
    if (localX < 0) localX += CHUNK_SIZE;
    if (localY < 0) localY += CHUNK_SIZE;
    
    Chunk& chunk = getOrCreateChunk(chunkX, chunkY);
    chunk.setTile(localX, localY, type);
}

Tile TileMap::getTile(int worldX, int worldY) const {
    int chunkX = worldX / (CHUNK_SIZE * TILE_SIZE);
    int chunkY = worldY / (CHUNK_SIZE * TILE_SIZE);
    int localX = (worldX / TILE_SIZE) % CHUNK_SIZE;
    int localY = (worldY / TILE_SIZE) % CHUNK_SIZE;
    
    if (localX < 0) localX += CHUNK_SIZE;
    if (localY < 0) localY += CHUNK_SIZE;
    
    auto key = std::make_pair(chunkX, chunkY);
    auto it = loadedChunks.find(key);
    if (it != loadedChunks.end()) {
        return it->second.getTile(localX, localY);
    }
    return Tile(TileType::EMPTY);
}

void TileMap::render(SDL_Renderer* renderer, int cameraX, int cameraY, int viewWidth, int viewHeight) {
    // Render only loaded chunks
    for (auto& pair : loadedChunks) {
        pair.second.render(renderer, cameraX, cameraY, viewWidth, viewHeight, textureManager);
    }
}
