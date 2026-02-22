#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <string>

struct GameConfig {
    float playerMaxSpeed;
    float playerAngularSpeed;

    static GameConfig load(const std::string& filename = "config.txt");
};

#endif
