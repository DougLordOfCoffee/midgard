#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>

class Player {
public:
    Player(int x, int y, int width, int height);
    
    void handleInput(const Uint8* keys);
    void render(SDL_Renderer* renderer, int cameraX, int cameraY);
    
    SDL_Rect getRect() const;
    int getCenterX() const;
    int getCenterY() const;
    
private:
    SDL_Rect rect;
    int speed;
};

#endif
