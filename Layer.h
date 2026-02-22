#ifndef LAYER_H
#define LAYER_H

#include <SDL2/SDL.h>

class Layer {
public:
    Layer(int x, int y, int width, int height, SDL_Color color, float parallaxFactor);
    
    void render(SDL_Renderer* renderer, int cameraX, int cameraY);
    void setPosition(int x, int y);
    SDL_Rect getRect() const;
    
private:
    SDL_Rect rect;
    SDL_Color color;
    float parallaxFactor;  // 0.0 = moves with camera, 1.0 = fully static
};

#endif
