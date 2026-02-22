#include "player/Player.h"

Player::Player(int x, int y, int width, int height) 
    : speed(3) {
    rect = { x, y, width, height };
}

void Player::handleInput(const bool* keys) {
    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])    rect.y -= speed;
    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])  rect.y += speed;
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])  rect.x -= speed;
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) rect.x += speed;
}

void Player::handleControllerInput(SDL_Gamepad* gamepad) {
    if (!gamepad) return;
    
    // Left stick input
    Sint16 stickX = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX);
    Sint16 stickY = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY);
    
    // Dead zone to prevent drift
    const int DEAD_ZONE = 8000;
    
    if (stickX < -DEAD_ZONE) rect.x -= speed;
    if (stickX > DEAD_ZONE) rect.x += speed;
    if (stickY < -DEAD_ZONE) rect.y -= speed;
    if (stickY > DEAD_ZONE) rect.y += speed;
    
    // D-pad as alternative
    if (SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP)) rect.y -= speed;
    if (SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN)) rect.y += speed;
    if (SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT)) rect.x -= speed;
    if (SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT)) rect.x += speed;
}

void Player::render(SDL_Renderer* renderer, int cameraX, int cameraY) {
    SDL_FRect renderRect = { (float)(rect.x - cameraX), (float)(rect.y - cameraY), (float)rect.w, (float)rect.h };
    SDL_SetRenderDrawColor(renderer, 0, 255, 100, 255);
    SDL_RenderFillRect(renderer, &renderRect);
}

SDL_Rect Player::getRect() const {
    return rect;
}

int Player::getCenterX() const {
    return rect.x + rect.w / 2;
}

int Player::getCenterY() const {
    return rect.y + rect.h / 2;
}
