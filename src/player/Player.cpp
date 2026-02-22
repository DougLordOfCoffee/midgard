#include "player/Player.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const float TWO_PI = (float)(2.0 * M_PI);

Player::Player(float x, float y, int width, int height, float maxSpeed, float angularSpeed)
    : x(x), y(y), w(width), h(height)
    , maxSpeed(maxSpeed), angularSpeed(angularSpeed), angle(0.0f)
    , moveX(0.0f), moveY(0.0f), turn(0.0f) {
}

void Player::handleInput(const bool* keys) {
    moveX = 0.0f;
    moveY = 0.0f;
    turn = 0.0f;

    if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])    moveY -= 1.0f;
    if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])  moveY += 1.0f;
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])  moveX -= 1.0f;
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) moveX += 1.0f;

    if (keys[SDL_SCANCODE_Q]) turn -= 1.0f;
    if (keys[SDL_SCANCODE_E]) turn += 1.0f;
}

void Player::handleControllerInput(SDL_Gamepad* gamepad) {
    if (!gamepad) return;

    const int DEAD_ZONE = 8000;
    const float SCALE = 32768.0f;

    Sint16 stickX = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX);
    Sint16 stickY = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY);

    moveX = (std::abs(stickX) > DEAD_ZONE) ? (float)stickX / SCALE : 0.0f;
    moveY = (std::abs(stickY) > DEAD_ZONE) ? (float)stickY / SCALE : 0.0f;

    Sint16 turnStick = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_RIGHTX);
    turn = (std::abs(turnStick) > DEAD_ZONE) ? (float)turnStick / SCALE : 0.0f;

    if (SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP))    moveY -= 1.0f;
    if (SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN))  moveY += 1.0f;
    if (SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT))  moveX -= 1.0f;
    if (SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT)) moveX += 1.0f;
}

void Player::update(float dt) {
    float lenSq = moveX * moveX + moveY * moveY;
    if (lenSq > 0.0f) {
        float len = std::sqrt(lenSq);
        float dx = (moveX / len) * maxSpeed * dt;
        float dy = (moveY / len) * maxSpeed * dt;
        x += dx;
        y += dy;
    }

    if (turn != 0.0f) {
        angle += turn * angularSpeed * dt;
        while (angle >= TWO_PI) angle -= TWO_PI;
        while (angle < 0.0f) angle += TWO_PI;
    }
}

void Player::render(SDL_Renderer* renderer, int cameraX, int cameraY) {
    SDL_FRect renderRect = {
        (float)((int)x - cameraX),
        (float)((int)y - cameraY),
        (float)w,
        (float)h
    };
    SDL_SetRenderDrawColor(renderer, 0, 255, 100, 255);
    SDL_RenderFillRect(renderer, &renderRect);
}

SDL_Rect Player::getRect() const {
    return { (int)x, (int)y, w, h };
}

int Player::getCenterX() const {
    return (int)x + w / 2;
}

int Player::getCenterY() const {
    return (int)y + h / 2;
}
