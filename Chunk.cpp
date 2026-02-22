#include "Chunk.h"

Chunk::Chunk(int chunkX, int chunkY, NoiseGenerator& noise) : chunkX(chunkX), chunkY(chunkY) {
    generateDefault(noise);
}

void Chunk::generateDefault(NoiseGenerator& noise) {
    // Generate terrain based on noise
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            // Get world coordinates
            int worldX = chunkX * CHUNK_SIZE + x;
            int worldY = chunkY * CHUNK_SIZE + y;
            
            // Get noise value (0.0 to 1.0)
            float noiseValue = noise.getNoise(worldX, worldY);
            
            // Determine tile type based on noise
            if (noiseValue < 0.3f) {
                tiles[x][y] = Tile(TileType::WATER);
            } else if (noiseValue < 0.4f) {
                tiles[x][y] = Tile(TileType::GRASS);
            } else if (noiseValue < 0.7f) {
                tiles[x][y] = Tile(TileType::GRASS);
            } else if (noiseValue < 0.85f) {
                tiles[x][y] = Tile(TileType::WALL);
            } else {
                tiles[x][y] = Tile(TileType::TRAP);
            }
        }
    }
}

void Chunk::setTile(int x, int y, TileType type) {
    if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE) {
        tiles[x][y] = Tile(type);
    }
}

Tile Chunk::getTile(int x, int y) const {
    if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE) {
        return tiles[x][y];
    }
    return Tile(TileType::EMPTY);
}

void Chunk::render(SDL_Renderer* renderer, int cameraX, int cameraY, int viewWidth, int viewHeight) {
    // Calculate which tiles are visible on screen
    int startTileX = cameraX / TILE_SIZE;
    int startTileY = cameraY / TILE_SIZE;
    int endTileX = (cameraX + viewWidth) / TILE_SIZE + 1;
    int endTileY = (cameraY + viewHeight) / TILE_SIZE + 1;
    
    // Clamp to chunk bounds
    startTileX = (startTileX < 0) ? 0 : startTileX;
    startTileY = (startTileY < 0) ? 0 : startTileY;
    endTileX = (endTileX > CHUNK_SIZE) ? CHUNK_SIZE : endTileX;
    endTileY = (endTileY > CHUNK_SIZE) ? CHUNK_SIZE : endTileY;
    
    // Render only visible tiles
    for (int y = startTileY; y < endTileY; y++) {
        for (int x = startTileX; x < endTileX; x++) {
            Tile tile = tiles[x][y];
            
            if (tile.type == TileType::EMPTY) continue;
            
            SDL_Rect tileRect = {
                x * TILE_SIZE - cameraX,
                y * TILE_SIZE - cameraY,
                TILE_SIZE,
                TILE_SIZE
            };
            
            SDL_Color color = tile.getColor();
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &tileRect);
            
            // Draw tile border for visibility
            SDL_SetRenderDrawColor(renderer, color.r / 2, color.g / 2, color.b / 2, 255);
            SDL_RenderDrawRect(renderer, &tileRect);
        }
    }
}

int Chunk::getWorldX() const {
    return chunkX * CHUNK_SIZE * TILE_SIZE;
}

int Chunk::getWorldY() const {
    return chunkY * CHUNK_SIZE * TILE_SIZE;
}
