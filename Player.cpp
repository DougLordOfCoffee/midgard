#include "Player.h"

Player::Player(int x, int y, int width, int height) 
    : speed(5) {
    rect = { x, y, width, height };
}

void Player::handleInput(const Uint8* keys) {
    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])    rect.y -= speed;
    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])  rect.y += speed;
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])  rect.x -= speed;
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) rect.x += speed;
}

void Player::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 100, 255);
    SDL_RenderFillRect(renderer, &rect);
}

SDL_Rect Player::getRect() const {
    return rect;
}
