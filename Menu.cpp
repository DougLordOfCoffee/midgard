#include "Menu.h"
#include <stdio.h>

Menu::Menu() : open(false), selectedOption(0), optionSelected(false) {}

bool Menu::isOpen() const {
    return open;
}

void Menu::toggle() {
    open = !open;
    selectedOption = 0;
    optionSelected = false;
}

void Menu::handleInput(const Uint8* keys) {
    static bool upPressed = false;
    static bool downPressed = false;
    static bool enterPressed = false;
    
    // Up arrow - previous option
    if (keys[SDL_SCANCODE_UP] && !upPressed) {
        selectedOption--;
        if (selectedOption < 0) {
            selectedOption = NUM_OPTIONS - 1;
        }
        upPressed = true;
    }
    if (!keys[SDL_SCANCODE_UP]) {
        upPressed = false;
    }
    
    // Down arrow - next option
    if (keys[SDL_SCANCODE_DOWN] && !downPressed) {
        selectedOption++;
        if (selectedOption >= NUM_OPTIONS) {
            selectedOption = 0;
        }
        downPressed = true;
    }
    if (!keys[SDL_SCANCODE_DOWN]) {
        downPressed = false;
    }
    
    // Enter - select option
    if (keys[SDL_SCANCODE_RETURN] && !enterPressed) {
        optionSelected = true;
        enterPressed = true;
    }
    if (!keys[SDL_SCANCODE_RETURN]) {
        enterPressed = false;
    }
}

SDL_Color Menu::getOptionColor(int option) const {
    if (option == selectedOption) {
        return {255, 255, 0, 255};  // Yellow for selected
    }
    return {200, 200, 200, 255};    // Gray for unselected
}

void Menu::render(SDL_Renderer* renderer, TextRenderer* textRenderer, int windowWidth, int windowHeight) {
    if (!open) return;
    
    // Draw semi-transparent overlay
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);  // Black with transparency
    SDL_Rect overlay = {0, 0, windowWidth, windowHeight};
    SDL_RenderFillRect(renderer, &overlay);
    
    // Draw menu box in center
    int menuX = (windowWidth - MENU_WIDTH) / 2;
    int menuY = (windowHeight - (NUM_OPTIONS * OPTION_HEIGHT)) / 2;
    
    SDL_Rect menuBox = {menuX - 10, menuY - 30, MENU_WIDTH + 20, (NUM_OPTIONS * OPTION_HEIGHT) + 60};
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);  // Dark gray
    SDL_RenderFillRect(renderer, &menuBox);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &menuBox);
    
    // Draw title
    if (textRenderer) {
        textRenderer->renderText(renderer, "PAUSE MENU", menuX + 60, menuY - 25, {255, 255, 255, 255});
    }
    
    // Draw options
    const char* optionNames[] = {"Resume", "Fullscreen", "Exit"};
    
    for (int i = 0; i < NUM_OPTIONS; i++) {
        SDL_Rect optionRect = {menuX, menuY + (i * OPTION_HEIGHT), MENU_WIDTH, OPTION_HEIGHT - 5};
        
        // Draw option background
        SDL_Color color = getOptionColor(i);
        SDL_SetRenderDrawColor(renderer, color.r / 3, color.g / 3, color.b / 3, 255);
        SDL_RenderFillRect(renderer, &optionRect);
        
        // Draw option border
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderDrawRect(renderer, &optionRect);
        
        // Draw option text
        if (textRenderer) {
            textRenderer->renderText(renderer, optionNames[i], menuX + 50, menuY + (i * OPTION_HEIGHT) + 12, color);
        }
    }
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

MenuOption Menu::getSelectedOption() const {
    return (MenuOption)selectedOption;
}

bool Menu::wasOptionSelected() const {
    return optionSelected;
}
