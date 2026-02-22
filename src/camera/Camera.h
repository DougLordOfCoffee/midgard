#ifndef CAMERA_H
#define CAMERA_H

#include <SDL3/SDL.h>

class Camera {
public:
    Camera(int screenWidth, int screenHeight);
    
    void followTarget(int targetX, int targetY);
    void setPosition(int x, int y);
    void setViewportSize(int width, int height);  // For fullscreen/window resize
    
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    SDL_Rect getViewport() const;
    
private:
    int x, y;
    int screenWidth, screenHeight;
};

#endif
