#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {

    // Start up SDL (video is all we need for now)
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "My First Window",       // title
        SDL_WINDOWPOS_CENTERED,  // x position
        SDL_WINDOWPOS_CENTERED,  // y position
        1080,                     // width
        540,                     // height
        SDL_WINDOW_SHOWN         // flags
    );

    if (window == NULL) {
        printf("Window Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer (this is what actually draws stuff)
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == NULL) {
        printf("Renderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Our square: x, y, width, height
    SDL_Rect square = { 350, 250, 100, 100 };
    int speed = 5;

    int running = 1;
    SDL_Event event;

    while (running) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Check which keys are being held down RIGHT NOW
        const Uint8* keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])    square.y -= speed;
        if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])  square.y += speed;
        if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])  square.x -= speed;
        if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) square.x += speed;

        SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 255, 100, 255);
        SDL_RenderFillRect(renderer, &square);

        SDL_RenderPresent(renderer);
    }
    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
