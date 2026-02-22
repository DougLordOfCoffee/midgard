#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();
    
    bool loadFont(const std::string& filePath, int fontSize);
    void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color);
    
private:
    TTF_Font* font;
};

#endif
