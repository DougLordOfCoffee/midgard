#ifndef MENU_H
#define MENU_H

#include <SDL3/SDL.h>
#include "render/TextRenderer.h"

enum class MenuOption {
    RESUME = 0,
    FULLSCREEN = 1,
    EXIT = 2
};

class EventBus;

class Menu {
public:
    Menu();
    
    void init(EventBus& bus);
    
    bool isOpen() const;
    void toggle();
    void render(SDL_Renderer* renderer, TextRenderer* textRenderer, int windowWidth, int windowHeight);
    
private:
    bool open;
    int selectedOption;
    const int NUM_OPTIONS = 3;
    const int OPTION_HEIGHT = 50;
    const int MENU_WIDTH = 300;
    
    SDL_Color getOptionColor(int option) const;
};

#endif
