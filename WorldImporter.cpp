#include "WorldImporter.h"
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <cctype>

bool WorldImporter::importWorld(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        printf("Failed to open import file: %s\n", inputFile.c_str());
        return false;
    }
    
    std::vector<std::string> cleanLines;
    std::string line;
    
    // Read input file and remove all spaces
    while (std::getline(inFile, line)) {
        std::string cleanLine;
        for (char c : line) {
            // Skip spaces and tabs, keep only valid characters
            if (c != ' ' && c != '\t' && c != '\r') {
                // Convert to lowercase for case-insensitive handling
                c = std::tolower(c);
                cleanLine += c;
            }
        }
        // Only add non-empty lines
        if (!cleanLine.empty()) {
            cleanLines.push_back(cleanLine);
        }
    }
    inFile.close();
    
    // Write cleaned data to output file
    std::ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        printf("Failed to open output file: %s\n", outputFile.c_str());
        return false;
    }
    
    for (const auto& cleanLine : cleanLines) {
        outFile << cleanLine << "\n";
    }
    outFile.close();
    
    printf("Imported world from '%s' -> '%s'\n", inputFile.c_str(), outputFile.c_str());
    printf("World size: %lu x %d chunks\n", cleanLines[0].size(), (int)cleanLines.size());
    return true;
}

void WorldImporter::getWorldDimensions(const std::string& filename, int& width, int& height) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        width = 0;
        height = 0;
        return;
    }
    
    std::string line;
    height = 0;
    width = 0;
    
    while (std::getline(file, line)) {
        if (!line.empty()) {
            width = line.length();
            height++;
        }
    }
    file.close();
}
