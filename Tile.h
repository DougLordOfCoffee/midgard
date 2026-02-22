#ifndef TILE_H
#define TILE_H

#include <SDL2/SDL.h>

enum class TileType {
    EMPTY = 0,
    GROUND = 1,
    WALL = 2,
    WATER = 3,
    GRASS = 4,
    TRAP = 5
};

struct Tile {
    TileType type;
    
    Tile() : type(TileType::EMPTY) {}
    Tile(TileType t) : type(t) {}
    
    SDL_Color getColor() const {
        switch (type) {
            case TileType::GROUND: return {139, 69, 19, 255};    // Brown
            case TileType::WALL: return {128, 128, 128, 255};    // Gray
            case TileType::WATER: return {0, 100, 255, 255};     // Blue
            case TileType::GRASS: return {34, 139, 34, 255};     // Green
            case TileType::TRAP: return {255, 100, 0, 255};      // Orange
            default: return {0, 0, 0, 0};                         // Transparent
        }
    }
};

#endif
