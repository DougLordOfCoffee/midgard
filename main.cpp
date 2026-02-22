#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "Player.h"
#include "Camera.h"
#include "Layer.h"
#include "TileMap.h"
#include "TextureManager.h"
#include "TextRenderer.h"
#include "WorldLoader.h"
#include "WorldImporter.h"
#include "Minimap.h"
#include "Menu.h"

int main(int argc, char* argv[]) {

    // Start up SDL (video, gamepad, and image loading support)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    
    // Initialize SDL_image for PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("IMG_Init Error: %s\n", IMG_GetError());
        SDL_Quit();
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

    // Create texture manager and load tile textures
    TextureManager textureManager;
    
    // Load your PNG files here - path should be relative to where game.exe is
    // You'll need to create these PNG files and place them in a folder
    textureManager.loadTexture("grass", "assets/grass.png", renderer);
    textureManager.loadTexture("water", "assets/water.png", renderer);
    textureManager.loadTexture("wall", "assets/wall.png", renderer);
    textureManager.loadTexture("ground", "assets/ground.png", renderer);
    textureManager.loadTexture("trap", "assets/trap.png", renderer);
    
    // Create text renderer and load font
    TextRenderer textRenderer;
    if (!textRenderer.loadFont("assets/Micro5-Regular.ttf", 24)) {
        printf("Warning: Could not load font from assets/Micro5-Regular.ttf\n");
    }
    
    // Check if importedMap.txt exists and import it
    FILE* importedFile = fopen("importedMap.txt", "r");
    if (importedFile) {
        fclose(importedFile);
        printf("Found importedMap.txt - importing...\n");
        WorldImporter::importWorld("importedMap.txt", "world.txt");
    }
    
    // Load world from file
    TileMap tilemap("world.txt", textureManager);
    
    // Get world dimensions to center player
    int worldWidth, worldHeight;
    WorldImporter::getWorldDimensions("world.txt", worldWidth, worldHeight);
    
    // Calculate center of world in pixels
    int centerWorldX = (worldWidth * 64 * 32) / 2;  // chunks * tiles_per_chunk * pixels_per_tile
    int centerWorldY = (worldHeight * 64 * 32) / 2;
    
    // Create camera and player at world center
    Camera camera(1080, 540);
    Player player(centerWorldX - 16, centerWorldY - 16, 32, 32);  // -16 to center the 32x32 player
    
    printf("World dimensions: %d x %d chunks\n", worldWidth, worldHeight);
    printf("Player spawned at: (%d, %d)\n", centerWorldX, centerWorldY);
    
    // Create minimap
    Minimap minimap("world.txt", renderer);
    
    // Create menu system
    Menu menu;
    
    // Window state
    int windowWidth = 1080;
    int windowHeight = 540;
    bool fullscreen = false;
    
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
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    // Toggle menu
                    menu.toggle();
                }
                if (event.key.keysym.sym == SDLK_TAB && !menu.isOpen()) {
                    // Toggle camera mode (only when menu closed)
                    cameraTarget = (cameraTarget == 0) ? 1 : 0;
                }
            }
        }

        // Check which keys are being held down RIGHT NOW
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        
        // Handle menu input if open
        if (menu.isOpen()) {
            menu.handleInput(keys);
            
            // Check if option was selected
            if (menu.wasOptionSelected()) {
                MenuOption selected = menu.getSelectedOption();
                if (selected == MenuOption::EXIT) {
                    running = 0;
                } else if (selected == MenuOption::FULLSCREEN) {
                    // Toggle fullscreen
                    fullscreen = !fullscreen;
                    if (fullscreen) {
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
                    } else {
                        SDL_SetWindowFullscreen(window, 0);
                        windowWidth = 1080;
                        windowHeight = 540;
                        SDL_SetWindowSize(window, windowWidth, windowHeight);
                    }
                    // Update camera viewport
                    camera.setViewportSize(windowWidth, windowHeight);
                    printf("Fullscreen: %s, Window size: %d x %d\n", fullscreen ? "ON" : "OFF", windowWidth, windowHeight);
                } else if (selected == MenuOption::RESUME) {
                    // Just close the menu
                    menu.toggle();
                }
                
            }
        } else {
            // Only handle player input when menu is closed
            player.handleInput(keys);
            
            // Handle controller input if available
            if (controller) {
                player.handleControllerInput(controller);
            }
        }
        
        // Update camera based on mode (when menu closed)
        if (cameraTarget == 0 && !menu.isOpen()) {
            // Follow player
            camera.followTarget(player.getCenterX(), player.getCenterY());
        }
        // In mode 1, camera stays static for world editing
        
        // Load/unload chunks based on camera position (performance optimization!)
        tilemap.updateLoadedChunks(camera.getX(), camera.getY());

        SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255);
        SDL_RenderClear(renderer);

        // Render tilemap with dynamic viewport
        tilemap.render(renderer, camera.getX(), camera.getY(), camera.getWidth(), camera.getHeight());
        
        // Render player with camera offset
        player.render(renderer, camera.getX(), camera.getY());
        
        // Render minimap and chunk coords only if menu is closed
        if (!menu.isOpen()) {
            // Calculate and display player chunk coordinates
            int playerChunkX = (player.getCenterX()) / (64 * 32);  // tiles_per_chunk * pixels_per_tile
            int playerChunkY = (player.getCenterY()) / (64 * 32);
            
            // Render minimap with player position
            minimap.render(renderer, playerChunkX, playerChunkY);
        }
        
        // Render menu on top of everything
        menu.render(renderer, &textRenderer, windowWidth, windowHeight);

        SDL_RenderPresent(renderer);
    }
    // Clean up
    if (controller) {
        SDL_GameControllerClose(controller);
    }
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
