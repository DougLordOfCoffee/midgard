#ifndef WORLDIMPORTER_H
#define WORLDIMPORTER_H

#include <string>

class WorldImporter {
public:
    // Import a world file and save as clean version
    static bool importWorld(const std::string& inputFile, const std::string& outputFile);
    
    // Get world dimensions after import
    static void getWorldDimensions(const std::string& filename, int& width, int& height);
};

#endif
