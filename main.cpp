#include <SDL2/SDL.h>
#include <stdio.h>
#include "Player.h"
#include "Camera.h"
#include "Layer.h"
#include "TileMap.h"

int main(int argc, char* argv[]) {

    // Start up SDL (video and gamepad support)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
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
    Player player(512, 512, 32, 32);  // Start player in middle-ish of world
    
    // Create 100x100 chunk world (use your own dimensions here!)
    TileMap tilemap(3, 3);
    
    // Try to open a controller if one is connected
    SDL_GameController* controller = nullptr;
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
            if (controller) {
                printf("Controller found! Using gamepad input.\n");
                break;
            }
        }
    }
    
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
        
        // Handle controller input if available
        if (controller) {
            player.handleControllerInput(controller);
        }
        
        // Update camera based on mode
        if (cameraTarget == 0) {
            // Follow player
            camera.followTarget(player.getCenterX(), player.getCenterY());
        }
        // In mode 1, camera stays static for world editing

        SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
        SDL_RenderClear(renderer);

        // Render tilemap
        tilemap.render(renderer, camera.getX(), camera.getY(), 1080, 540);
        
        // Render player with camera offset
        player.render(renderer, camera.getX(), camera.getY());

        SDL_RenderPresent(renderer);
    }
    // Clean up
    if (controller) {
        SDL_GameControllerClose(controller);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
