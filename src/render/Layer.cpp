#include "render/Layer.h"

Layer::Layer(int x, int y, int width, int height, SDL_Color color, float parallaxFactor)
    : color(color), parallaxFactor(parallaxFactor) {
    rect = { x, y, width, height };
}

void Layer::render(SDL_Renderer* renderer, int cameraX, int cameraY) {
    // Apply parallax: parallaxFactor of 1.0 means no movement with camera (SDL3: use SDL_FRect)
    SDL_FRect renderRect;
    renderRect.x = (float)(rect.x - (int)(cameraX * (1.0f - parallaxFactor)));
    renderRect.y = (float)(rect.y - (int)(cameraY * (1.0f - parallaxFactor)));
    renderRect.w = (float)rect.w;
    renderRect.h = (float)rect.h;
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
