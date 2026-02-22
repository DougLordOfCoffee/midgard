#ifndef WORLDLOADER_H
#define WORLDLOADER_H

#include <string>
#include <vector>
#include "Tile.h"

enum class ChunkType {
    EMPTY = '.',
    GRASSLAND = 'g',
    FOREST = 'f',
    MOUNTAIN = 'm',
    WATER = 'w'
};

struct ChunkBiome {
    int x, y;
    ChunkType type;
};

class WorldLoader {
public:
    WorldLoader();
    
    // Load world from file
    bool loadWorld(const std::string& filename);
    
    // Save world to file
    bool saveWorld(const std::string& filename);
    
    // Get chunk type at position
    ChunkType getChunkType(int chunkX, int chunkY) const;
    
    // Get tile type for a biome
    static TileType getTileTypeForBiome(ChunkType biome);
    
    // Get texture name for biome
    static std::string getTextureNameForBiome(ChunkType biome);
    
private:
    std::vector<std::vector<ChunkType>> world;
    int worldWidth, worldHeight;
};

#endif
