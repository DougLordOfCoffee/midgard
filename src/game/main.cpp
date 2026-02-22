#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include "player/Player.h"
#include "camera/Camera.h"
#include "render/Layer.h"
#include "world/TileMap.h"
#include "render/TextureManager.h"
#include "render/TextRenderer.h"
#include "world/WorldLoader.h"
#include "world/WorldImporter.h"
#include "ui/Minimap.h"
#include "ui/Menu.h"
#include "event_bus/EventBus.h"

int main(int argc, char* argv[]) {

    // Start up SDL (video, gamepad, and image loading support)
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    
    // SDL3_image: no separate init required; first load initializes.

    // Create a window (SDL3: use properties for position/size)
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetStringProperty(props, SDL_PROP_WINDOW_CREATE_TITLE_STRING, "My First Window");
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_X_NUMBER, SDL_WINDOWPOS_CENTERED_DISPLAY(0));
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_Y_NUMBER, SDL_WINDOWPOS_CENTERED_DISPLAY(0));
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, 1080);
    SDL_SetNumberProperty(props, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, 540);
    SDL_Window* window = SDL_CreateWindowWithProperties(props);
    SDL_DestroyProperties(props);

    if (window == NULL) {
        printf("Window Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer (SDL3: second arg is renderer name, NULL = default; no flags)
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

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
    
    // Create event bus and menu system
    EventBus bus;
    Menu menu;
    menu.init(bus);
    
    // Window state
    int windowWidth = 1080;
    int windowHeight = 540;
    bool fullscreen = false;
    
    // Try to open a gamepad if one is connected (SDL3: SDL_GetGamepads returns SDL_JoystickID*)
    SDL_Gamepad* gamepad = nullptr;
    int num_gamepads = 0;
    SDL_JoystickID* gamepad_ids = SDL_GetGamepads(&num_gamepads);
    if (gamepad_ids && num_gamepads > 0) {
        gamepad = SDL_OpenGamepad(gamepad_ids[0]);
        SDL_free(gamepad_ids);
        if (gamepad) {
            printf("Gamepad found! Using gamepad input.\n");
        }
    }
    
    int cameraTarget = 0;  // 0 = player, 1 = fixed world editing mode

    int running = 1;
    SDL_Event event;

    // Event bus subscriptions
    bus.subscribeQuitRequested([&running]() { running = 0; });
    bus.subscribeKeyDown([&menu, &cameraTarget](SDL_Keycode key, bool repeat) {
        if (key == SDLK_TAB && !menu.isOpen())
            cameraTarget = (cameraTarget == 0) ? 1 : 0;
    });
    bus.subscribeMenuOptionSelected([&](MenuOption option) {
        if (option == MenuOption::EXIT) {
            running = 0;
        } else if (option == MenuOption::FULLSCREEN) {
            fullscreen = true;
            SDL_SetWindowFullscreen(window, true);
            SDL_GetWindowSize(window, &windowWidth, &windowHeight);
            camera.setViewportSize(windowWidth, windowHeight);
            printf("Fullscreen: ON, Window size: %d x %d\n", windowWidth, windowHeight);
        } else if (option == MenuOption::RESUME) {
            menu.toggle();
        }
    });
    bus.subscribeKeyboardState([&](const bool* k) {
        if (!menu.isOpen()) player.handleInput(k);
    });
    bus.subscribeGamepadState([&](SDL_Gamepad* gp) {
        if (!menu.isOpen() && gp) player.handleControllerInput(gp);
    });

    while (running) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                bus.publishQuitRequested();
            }
            if (event.type == SDL_EVENT_KEY_DOWN) {
                bus.publishKeyDown(event.key.key, event.key.repeat);
            }
        }

        // Check which keys are being held down RIGHT NOW (SDL3: returns const bool*)
        const bool* keys = SDL_GetKeyboardState(NULL);
        bus.publishKeyboardState(keys);
        bus.publishGamepadState(gamepad);
        
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
    if (gamepad) {
        SDL_CloseGamepad(gamepad);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
