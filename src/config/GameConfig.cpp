#include "config/GameConfig.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <stdio.h>

namespace {

std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end == std::string::npos ? std::string::npos : end - start + 1);
}

}

GameConfig GameConfig::load(const std::string& filename) {
    GameConfig cfg;
    cfg.playerMaxSpeed = 200.0f;
    cfg.playerAngularSpeed = 3.0f;

    std::ifstream file(filename);
    if (!file.is_open()) {
        printf("Config: could not open '%s', using defaults (max_speed=200, angular_speed=3)\n", filename.c_str());
        return cfg;
    }

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;

        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;

        std::string key = trim(line.substr(0, eq));
        std::string value = trim(line.substr(eq + 1));
        if (key.empty() || value.empty()) continue;

        if (key == "player_max_speed") {
            float v = 0.0f;
            if (std::sscanf(value.c_str(), "%f", &v) == 1 && v > 0.0f)
                cfg.playerMaxSpeed = v;
        } else if (key == "player_angular_speed") {
            float v = 0.0f;
            if (std::sscanf(value.c_str(), "%f", &v) == 1 && v >= 0.0f)
                cfg.playerAngularSpeed = v;
        }
    }

    return cfg;
}
