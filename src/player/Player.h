#ifndef PLAYER_H
#define PLAYER_H

#include <SDL3/SDL.h>

class Player {
public:
    Player(float x, float y, int width, int height, float maxSpeed, float angularSpeed);

    void handleInput(const bool* keys);
    void handleControllerInput(SDL_Gamepad* gamepad);
    void update(float dt);
    void render(SDL_Renderer* renderer, int cameraX, int cameraY);

    SDL_Rect getRect() const;
    int getCenterX() const;
    int getCenterY() const;

private:
    float x, y;
    int w, h;
    float maxSpeed;
    float angularSpeed;
    float angle;

    float moveX, moveY;
    float turn;
};

#endif
