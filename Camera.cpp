#include "Camera.h"

Camera::Camera(int screenWidth, int screenHeight)
    : x(0), y(0), screenWidth(screenWidth), screenHeight(screenHeight) {}

void Camera::followTarget(int targetX, int targetY) {
    // Center camera on target
    x = targetX - screenWidth / 2;
    y = targetY - screenHeight / 2;
}

void Camera::setPosition(int posX, int posY) {
    x = posX;
    y = posY;
}

void Camera::setViewportSize(int width, int height) {
    screenWidth = width;
    screenHeight = height;
}

int Camera::getX() const {
    return x;
}

int Camera::getY() const {
    return y;
}

int Camera::getWidth() const {
    return screenWidth;
}

int Camera::getHeight() const {
    return screenHeight;
}

SDL_Rect Camera::getViewport() const {
    return { x, y, screenWidth, screenHeight };
}
