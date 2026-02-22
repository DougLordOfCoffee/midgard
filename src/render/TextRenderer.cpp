#include "render/TextRenderer.h"
#include <stdio.h>

TextRenderer::TextRenderer() : font(nullptr) {}

TextRenderer::~TextRenderer() {
    if (font) {
        TTF_CloseFont(font);
    }
}

bool TextRenderer::loadFont(const std::string& filePath, int fontSize) {
    if (!TTF_Init()) {
        printf("SDL_ttf initialization failed: %s\n", SDL_GetError());
        return false;
    }
    
    font = TTF_OpenFont(filePath.c_str(), (float)fontSize);
    if (!font) {
        printf("Failed to load font '%s': %s\n", filePath.c_str(), SDL_GetError());
        return false;
    }
    
    printf("Font loaded successfully: %s (%d pt)\n", filePath.c_str(), fontSize);
    return true;
}

void TextRenderer::renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color) {
    if (!font) {
        printf("Warning: No font loaded\n");
        return;
    }
    
    // Render text to surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), text.size(), color);
    if (!textSurface) {
        printf("Failed to render text: %s\n", SDL_GetError());
        return;
    }
    
    // Convert surface to texture
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_DestroySurface(textSurface);
    
    if (!textTexture) {
        printf("Failed to create text texture\n");
        return;
    }
    
    // Determine size (SDL3: SDL_GetTextureSize)
    float w, h;
    SDL_GetTextureSize(textTexture, &w, &h);
    
    // Render to screen (SDL3: SDL_RenderTexture takes SDL_FRect)
    SDL_FRect dest = {(float)x, (float)y, w, h};
    SDL_RenderTexture(renderer, textTexture, nullptr, &dest);
    
    // Clean up
    SDL_DestroyTexture(textTexture);
}
