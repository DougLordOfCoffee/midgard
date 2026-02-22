#include "world/Chunk.h"
#include "world/WorldLoader.h"

Chunk::Chunk(int chunkX, int chunkY, ChunkType biomeType) : chunkX(chunkX), chunkY(chunkY) {
    generateFromBiome(biomeType);
}

void Chunk::generateFromBiome(ChunkType biomeType) {
    // Fill ALL tiles in chunk with the same biome type
    TileType tileType = WorldLoader::getTileTypeForBiome(biomeType);
    std::string textureName = WorldLoader::getTextureNameForBiome(biomeType);
    
    for (int y = 0; y < CHUNK_SIZE; y++) {
        for (int x = 0; x < CHUNK_SIZE; x++) {
            tiles[x][y] = Tile(tileType);
            tiles[x][y].textureName = textureName;
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

void Chunk::render(SDL_Renderer* renderer, int cameraX, int cameraY, int viewWidth, int viewHeight, TextureManager& textureManager) {
    // Get this chunk's world position in tiles
    int chunkWorldTileX = chunkX * CHUNK_SIZE;
    int chunkWorldTileY = chunkY * CHUNK_SIZE;
    int chunkWorldPixelX = chunkWorldTileX * TILE_SIZE;
    int chunkWorldPixelY = chunkWorldTileY * TILE_SIZE;
    
    // Calculate which world tiles are visible on screen
    int startWorldTileX = cameraX / TILE_SIZE;
    int startWorldTileY = cameraY / TILE_SIZE;
    int endWorldTileX = (cameraX + viewWidth) / TILE_SIZE + 1;
    int endWorldTileY = (cameraY + viewHeight) / TILE_SIZE + 1;
    
    // Convert to local chunk coordinates
    int startTileX = startWorldTileX - chunkWorldTileX;
    int startTileY = startWorldTileY - chunkWorldTileY;
    int endTileX = endWorldTileX - chunkWorldTileX;
    int endTileY = endWorldTileY - chunkWorldTileY;
    
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
            
            // Calculate world pixel position of this tile
            int worldPixelX = (chunkWorldTileX + x) * TILE_SIZE;
            int worldPixelY = (chunkWorldTileY + y) * TILE_SIZE;
            
            // Convert to screen position (relative to camera)
            int screenX = worldPixelX - cameraX;
            int screenY = worldPixelY - cameraY;
            
            // Try to render texture, fall back to color if texture not found
            SDL_Texture* texture = textureManager.getTexture(tile.textureName);
            if (texture) {
                // Texture found - render it
                textureManager.drawTexture(renderer, tile.textureName, screenX, screenY, TILE_SIZE, TILE_SIZE);
            } else {
                // No texture found - render solid color as fallback
                SDL_FRect tileRect = { (float)screenX, (float)screenY, (float)TILE_SIZE, (float)TILE_SIZE };
                SDL_Color color = tile.getColor();
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_RenderFillRect(renderer, &tileRect);
                
                // Draw tile border in darker color so you can see individual tiles
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black border
                SDL_RenderRect(renderer, &tileRect);
                
                // Draw a thick border so fallback tiles are obvious
                SDL_FRect borderRect = { (float)(screenX + 1), (float)(screenY + 1), (float)(TILE_SIZE - 2), (float)(TILE_SIZE - 2) };
                SDL_RenderRect(renderer, &borderRect);
            }
        }
    }
}

int Chunk::getWorldX() const {
    return chunkX * CHUNK_SIZE * TILE_SIZE;
}

int Chunk::getWorldY() const {
    return chunkY * CHUNK_SIZE * TILE_SIZE;
}
