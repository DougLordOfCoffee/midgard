#ifndef NOISEGENERATOR_H
#define NOISEGENERATOR_H

#include <random>

class NoiseGenerator {
public:
    NoiseGenerator(int seed = 42);
    
    // Simple noise value 0-1
    float getNoise(int x, int y);
    
private:
    std::mt19937 generator;
    int seed;
    
    // Simple hash function
    int hash(int x, int y);
};

#endif
