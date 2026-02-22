#include "ui/Minimap.h"
#include <stdio.h>
#include <cctype>

Minimap::Minimap(const std::string& worldFile, SDL_Renderer* renderer) : minimapTexture(nullptr) {
    // Load world to get data
    WorldLoader loader;
    loader.loadWorld(worldFile);
    
    // Get world size by reading file
    FILE* file = fopen(worldFile.c_str(), "r");
    if (file) {
        worldHeight = 0;
        worldWidth = 0;
        char line[500];
        while (fgets(line, sizeof(line), file)) {
            // Count non-empty lines and their width
            int lineLen = 0;
            for (int i = 0; line[i] != '\0' && line[i] != '\n'; i++) {
                if (line[i] != ' ' && line[i] != '\t') {
                    lineLen++;
                }
            }
            if (lineLen > 0) {
                worldHeight++;
                if (lineLen > worldWidth) {
                    worldWidth = lineLen;
                }
            }
        }
        fclose(file);
    }
    
    printf("Minimap initialized: %d x %d chunks\n", worldWidth, worldHeight);
    
    // Generate minimap texture (SDL3: SDL_CreateSurface with pixel format)
    SDL_Surface* surface = SDL_CreateSurface(MINIMAP_SIZE, MINIMAP_SIZE, SDL_PIXELFORMAT_RGBA8888);
    if (!surface) {
        printf("Failed to create minimap surface\n");
        return;
    }
    
    // Fill with world data
    float scaleX = (float)MINIMAP_SIZE / worldWidth;
    float scaleY = (float)MINIMAP_SIZE / worldHeight;
    
    for (int y = 0; y < worldHeight; y++) {
        for (int x = 0; x < worldWidth; x++) {
            ChunkType biome = loader.getChunkType(x, y);
            SDL_Color color = getBiomeColor(biome);
            
            // Get pixel position
            int pixX = (int)(x * scaleX);
            int pixY = (int)(y * scaleY);
            
            // Fill a small rectangle for this chunk (SDL3: MapSurfaceRGBA, FillSurfaceRect)
            int w = (int)scaleX > 0 ? (int)scaleX : 1;
            int h = (int)scaleY > 0 ? (int)scaleY : 1;
            SDL_Rect pixelRect = {pixX, pixY, w, h};
            Uint32 pixel = SDL_MapSurfaceRGBA(surface, color.r, color.g, color.b, color.a);
            SDL_FillSurfaceRect(surface, &pixelRect, pixel);
        }
    }
    
    // Convert surface to texture
    minimapTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    
    if (minimapTexture) {
        printf("Minimap texture created successfully\n");
    }
}

Minimap::~Minimap() {
    if (minimapTexture) {
        SDL_DestroyTexture(minimapTexture);
    }
}

SDL_Color Minimap::getBiomeColor(ChunkType biome) const {
    switch (biome) {
        case ChunkType::GRASSLAND: return {34, 139, 34, 255};      // Green
        case ChunkType::FOREST: return {0, 100, 0, 255};           // Dark green
        case ChunkType::MOUNTAIN: return {128, 128, 128, 255};     // Gray
        case ChunkType::WATER: return {0, 100, 255, 255};          // Blue
        case ChunkType::EMPTY: return {20, 20, 40, 255};           // Dark (background)
        default: return {0, 0, 0, 255};
    }
}

void Minimap::render(SDL_Renderer* renderer, int playerChunkX, int playerChunkY) {
    if (!minimapTexture) return;
    
    // Draw minimap in top left (SDL3: use SDL_FRect)
    SDL_FRect minimapRect = {5, 5, (float)MINIMAP_SIZE, (float)MINIMAP_SIZE};
    SDL_RenderTexture(renderer, minimapTexture, nullptr, &minimapRect);
    
    // Draw black border around minimap
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderRect(renderer, &minimapRect);
    
    // Draw thicker border
    SDL_FRect borderRect = {4, 4, (float)(MINIMAP_SIZE + 2), (float)(MINIMAP_SIZE + 2)};
    SDL_RenderRect(renderer, &borderRect);
    
    // Draw player position indicator (yellow dot)
    float scaleX = (float)MINIMAP_SIZE / worldWidth;
    float scaleY = (float)MINIMAP_SIZE / worldHeight;
    
    int playerPixX = 5 + (int)(playerChunkX * scaleX);
    int playerPixY = 5 + (int)(playerChunkY * scaleY);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Yellow
    SDL_FRect playerDot = {(float)(playerPixX - 2), (float)(playerPixY - 2), 4, 4};
    SDL_RenderFillRect(renderer, &playerDot);
    
    // Draw chunk coordinates display box below minimap
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_FRect coordBox = {5, (float)(MINIMAP_SIZE + 15), 150, 30};
    SDL_RenderRect(renderer, &coordBox);
    
    // Print chunk coordinates to console
    printf("Player at Chunk X: %d, Chunk Y: %d\n", playerChunkX, playerChunkY);
}
