#include "TileMap.h"

TileMap::TileMap(int chunkWidthCount, int chunkHeightCount)
    : chunkWidthCount(chunkWidthCount), chunkHeightCount(chunkHeightCount), noise(42) {
    // Create all chunks in the world based on dimensions
    for (int y = 0; y < chunkHeightCount; y++) {
        for (int x = 0; x < chunkWidthCount; x++) {
            getOrCreateChunk(x, y);
        }
    }
}

Chunk& TileMap::getOrCreateChunk(int chunkX, int chunkY) {
    auto key = std::make_pair(chunkX, chunkY);
    if (chunks.find(key) == chunks.end()) {
        chunks[key] = Chunk(chunkX, chunkY, noise);
    }
    return chunks[key];
}

void TileMap::setTile(int worldX, int worldY, TileType type) {
    // Convert world coordinates to chunk and local tile coordinates
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
    auto it = chunks.find(key);
    if (it != chunks.end()) {
        return it->second.getTile(localX, localY);
    }
    return Tile(TileType::EMPTY);
}

void TileMap::render(SDL_Renderer* renderer, int cameraX, int cameraY, int viewWidth, int viewHeight) {
    // For 1x1 chunk world, just render the one chunk
    // But we could expand this to render multiple chunks if needed
    for (auto& pair : chunks) {
        pair.second.render(renderer, cameraX, cameraY, viewWidth, viewHeight);
    }
}
