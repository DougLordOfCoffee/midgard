#include "ui/Menu.h"
#include "event_bus/EventBus.h"
#include <stdio.h>

Menu::Menu() : open(false), selectedOption(0), lastMenuUp(false), lastMenuDown(false) {}

void Menu::init(EventBus& bus) {
    bus.subscribeKeyDown([this, &bus](SDL_Keycode key, bool repeat) {
        if (repeat) return;
        if (key == SDLK_ESCAPE) {
            toggle();
            return;
        }
        if (!open) return;
        if (key == SDLK_UP) {
            moveSelectionUp();
        } else if (key == SDLK_DOWN) {
            moveSelectionDown();
        } else if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
            bus.publishMenuOptionSelected((MenuOption)selectedOption);
        }
    });
    bus.subscribeGamepadButtonDown([this, &bus](Uint8 button) {
        if (button == SDL_GAMEPAD_BUTTON_START || button == SDL_GAMEPAD_BUTTON_EAST) {
            toggle();
            return;
        }
        if (!open) return;
        if (button == SDL_GAMEPAD_BUTTON_SOUTH) {
            bus.publishMenuOptionSelected((MenuOption)selectedOption);
        }
    });
    bus.subscribeGamepadState([this](SDL_Gamepad* gp) {
        if (open && gp) handleGamepadState(gp);
    });
}

bool Menu::isOpen() const {
    return open;
}

void Menu::toggle() {
    open = !open;
    selectedOption = 0;
}

void Menu::moveSelectionUp() {
    selectedOption--;
    if (selectedOption < 0) selectedOption = NUM_OPTIONS - 1;
}

void Menu::moveSelectionDown() {
    selectedOption++;
    if (selectedOption >= NUM_OPTIONS) selectedOption = 0;
}

void Menu::handleGamepadState(SDL_Gamepad* gamepad) {
    if (!gamepad) return;
    const int STICK_DEADZONE = 8000;
    bool dpadUp = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP) != 0;
    bool dpadDown = SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN) != 0;
    Sint16 stickY = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY);
    bool stickUp = (stickY < -STICK_DEADZONE);
    bool stickDown = (stickY > STICK_DEADZONE);
    bool up = dpadUp || stickUp;
    bool down = dpadDown || stickDown;
    if (up && !lastMenuUp) moveSelectionUp();
    lastMenuUp = up;
    if (down && !lastMenuDown) moveSelectionDown();
    lastMenuDown = down;
}

SDL_Color Menu::getOptionColor(int option) const {
    if (option == selectedOption) {
        return {255, 255, 0, 255};  // Yellow for selected
    }
    return {200, 200, 200, 255};    // Gray for unselected
}

void Menu::render(SDL_Renderer* renderer, TextRenderer* textRenderer, int windowWidth, int windowHeight) {
    if (!open) return;
    
    // Draw semi-transparent overlay (SDL3: use SDL_FRect)
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);  // Black with transparency
    SDL_FRect overlay = {0, 0, (float)windowWidth, (float)windowHeight};
    SDL_RenderFillRect(renderer, &overlay);
    
    // Draw menu box in center
    int menuX = (windowWidth - MENU_WIDTH) / 2;
    int menuY = (windowHeight - (NUM_OPTIONS * OPTION_HEIGHT)) / 2;
    
    SDL_FRect menuBox = {(float)(menuX - 10), (float)(menuY - 30), (float)(MENU_WIDTH + 20), (float)((NUM_OPTIONS * OPTION_HEIGHT) + 60)};
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);  // Dark gray
    SDL_RenderFillRect(renderer, &menuBox);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderRect(renderer, &menuBox);
    
    // Draw title
    if (textRenderer) {
        textRenderer->renderText(renderer, "PAUSE MENU", menuX + 60, menuY - 25, {255, 255, 255, 255});
    }
    
    // Draw options
    const char* optionNames[] = {"Resume", "Fullscreen", "Exit"};
    
    for (int i = 0; i < NUM_OPTIONS; i++) {
        SDL_FRect optionRect = {(float)menuX, (float)(menuY + (i * OPTION_HEIGHT)), (float)MENU_WIDTH, (float)(OPTION_HEIGHT - 5)};
        
        // Draw option background
        SDL_Color color = getOptionColor(i);
        SDL_SetRenderDrawColor(renderer, color.r / 3, color.g / 3, color.b / 3, 255);
        SDL_RenderFillRect(renderer, &optionRect);
        
        // Draw option border
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        SDL_RenderRect(renderer, &optionRect);
        
        // Draw option text
        if (textRenderer) {
            textRenderer->renderText(renderer, optionNames[i], menuX + 50, menuY + (i * OPTION_HEIGHT) + 12, color);
        }
    }
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}
