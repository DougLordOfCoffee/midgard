#include "TextRenderer.h"
#include <stdio.h>

TextRenderer::TextRenderer() : font(nullptr) {}

TextRenderer::~TextRenderer() {
    if (font) {
        TTF_CloseFont(font);
    }
}

bool TextRenderer::loadFont(const std::string& filePath, int fontSize) {
    if (TTF_Init() == -1) {
        printf("SDL_ttf initialization failed: %s\n", TTF_GetError());
        return false;
    }
    
    font = TTF_OpenFont(filePath.c_str(), fontSize);
    if (!font) {
        printf("Failed to load font '%s': %s\n", filePath.c_str(), TTF_GetError());
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
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!textSurface) {
        printf("Failed to render text: %s\n", TTF_GetError());
        return;
    }
    
    // Convert surface to texture
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    
    if (!textTexture) {
        printf("Failed to create text texture\n");
        return;
    }
    
    // Determine size
    int w, h;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &w, &h);
    
    // Render to screen
    SDL_Rect dest = {x, y, w, h};
    SDL_RenderCopy(renderer, textTexture, nullptr, &dest);
    
    // Clean up
    SDL_DestroyTexture(textTexture);
}
