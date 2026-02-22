#include <SDL2/SDL.h>
#include <stdio.h>
#include "Player.h"
#include "Camera.h"
#include "Layer.h"

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

    // Create camera and player
    Camera camera(1080, 540);
    Player player(350, 250, 100, 100);
    
    // Create background layers with parallax effect
    // parallaxFactor: 1.0 = completely static, 0.0 = moves with camera
    Layer backgroundFar(0, 0, 1200, 540, {50, 80, 150, 255}, 1.0f);      // Sky - fully static
    Layer backgroundNear(0, 200, 1200, 340, {34, 139, 34, 255}, 0.8f);   // Ground hills - slight parallax
    
    int cameraTarget = 0;  // 0 = player, 1 = fixed world editing mode

    int running = 1;
    SDL_Event event;

    while (running) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_TAB) {
                    // Toggle camera mode
                    cameraTarget = (cameraTarget == 0) ? 1 : 0;
                }
            }
        }

        // Check which keys are being held down RIGHT NOW
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        player.handleInput(keys);
        
        // Update camera based on mode
        if (cameraTarget == 0) {
            // Follow player
            camera.followTarget(player.getCenterX(), player.getCenterY());
        }
        // In mode 1, camera stays static for world editing

        SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
        SDL_RenderClear(renderer);

        // Render background layers with parallax
        backgroundFar.render(renderer, camera.getX(), camera.getY());
        backgroundNear.render(renderer, camera.getX(), camera.getY());
        
        // Render player with camera offset
        player.render(renderer, camera.getX(), camera.getY());

        SDL_RenderPresent(renderer);
    }
    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
