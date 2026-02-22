#include "Layer.h"

Layer::Layer(int x, int y, int width, int height, SDL_Color color, float parallaxFactor)
    : color(color), parallaxFactor(parallaxFactor) {
    rect = { x, y, width, height };
}

void Layer::render(SDL_Renderer* renderer, int cameraX, int cameraY) {
    // Apply parallax: parallaxFactor of 1.0 means no movement with camera
    SDL_Rect renderRect = rect;
    renderRect.x = rect.x - (int)(cameraX * (1.0f - parallaxFactor));
    renderRect.y = rect.y - (int)(cameraY * (1.0f - parallaxFactor));
    
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &renderRect);
}

void Layer::setPosition(int x, int y) {
    rect.x = x;
    rect.y = y;
}

SDL_Rect Layer::getRect() const {
    return rect;
}
