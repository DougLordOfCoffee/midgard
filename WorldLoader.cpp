#include "WorldLoader.h"
#include <fstream>
#include <stdio.h>
#include <cctype>

WorldLoader::WorldLoader() : worldWidth(0), worldHeight(0) {}

bool WorldLoader::loadWorld(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        printf("Failed to open world file: %s\n", filename.c_str());
        return false;
    }
    
    world.clear();
    std::string line;
    
    while (std::getline(file, line)) {
        std::vector<ChunkType> row;
        for (char c : line) {
            if (c != ' ' && c != '\t') {  // Skip whitespace
                // Convert to lowercase for case-insensitive loading
                c = std::tolower(c);
                row.push_back((ChunkType)c);
            }
        }
        if (!row.empty()) {
            world.push_back(row);
        }
    }
    
    file.close();
    
    worldHeight = world.size();
    worldWidth = worldHeight > 0 ? world[0].size() : 0;
    
    printf("World loaded: %d x %d chunks\n", worldWidth, worldHeight);
    return true;
}

bool WorldLoader::saveWorld(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        printf("Failed to open world file for writing: %s\n", filename.c_str());
        return false;
    }
    
    for (const auto& row : world) {
        for (ChunkType chunk : row) {
            file << (char)chunk;
        }
        file << "\n";
    }
    
    file.close();
    printf("World saved to %s\n", filename.c_str());
    return true;
}

ChunkType WorldLoader::getChunkType(int chunkX, int chunkY) const {
    if (chunkY < 0 || chunkY >= worldHeight || chunkX < 0 || chunkX >= worldWidth) {
        return ChunkType::EMPTY;
    }
    return world[chunkY][chunkX];
}

TileType WorldLoader::getTileTypeForBiome(ChunkType biome) {
    switch (biome) {
        case ChunkType::GRASSLAND: return TileType::GRASS;
        case ChunkType::FOREST: return TileType::GRASS;  // Forest tiles are grass-colored
        case ChunkType::MOUNTAIN: return TileType::WALL;
        case ChunkType::WATER: return TileType::WATER;
        default: return TileType::EMPTY;
    }
}

std::string WorldLoader::getTextureNameForBiome(ChunkType biome) {
    switch (biome) {
        case ChunkType::GRASSLAND: return "grass";
        case ChunkType::FOREST: return "grass";  // Use grass texture for now
        case ChunkType::MOUNTAIN: return "wall";
        case ChunkType::WATER: return "water";
        default: return "";
    }
}
