#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include "TextRenderer.h"

enum class MenuOption {
    RESUME = 0,
    FULLSCREEN = 1,
    EXIT = 2
};

class Menu {
public:
    Menu();
    
    bool isOpen() const;
    void toggle();
    void handleInput(const Uint8* keys);
    void render(SDL_Renderer* renderer, TextRenderer* textRenderer, int windowWidth, int windowHeight);
    
    MenuOption getSelectedOption() const;
    bool wasOptionSelected() const;
    
private:
    bool open;
    int selectedOption;
    bool optionSelected;
    const int NUM_OPTIONS = 3;
    const int OPTION_HEIGHT = 50;
    const int MENU_WIDTH = 300;
    
    SDL_Color getOptionColor(int option) const;
};

#endif
