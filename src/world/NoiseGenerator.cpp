#include "world/NoiseGenerator.h"

NoiseGenerator::NoiseGenerator(int seed) : seed(seed) {
    generator.seed(seed);
}

int NoiseGenerator::hash(int x, int y) {
    // Simple hash function
    int h = seed;
    h ^= 2654435761U + (h << 6) + (h >> 2);
    h ^= x * 2246822519;
    h ^= y * 3266489917U;
    return h;
}

float NoiseGenerator::getNoise(int x, int y) {
    // Get pseudo-random value based on coordinates
    int h = hash(x, y);
    return (float)(h & 0x7FFFFFFF) / (float)0x7FFFFFFF;
}
