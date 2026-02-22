#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>

class Camera {
public:
    Camera(int screenWidth, int screenHeight);
    
    void followTarget(int targetX, int targetY);
    void setPosition(int x, int y);
    
    int getX() const;
    int getY() const;
    SDL_Rect getViewport() const;
    
private:
    int x, y;
    int screenWidth, screenHeight;
};

#endif
