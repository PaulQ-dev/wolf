#pragma once

#include <SDL2/SDL.h>

typedef struct
{
    SDL_Window* win;
    SDL_Renderer* ren;
    SDL_Surface* surf;
    SDL_Texture* tex;
    Uint32* pix;
    Uint32 w;
    Uint32 h;
} Window;

void drawPixel(Window* window, Uint32 x, Uint32 y, Uint32 color);
void drawRect(Window* Window, Uint32 x, Uint32 y, Uint32 width, Uint32 height, Uint32 color);
Uint32 getPixel(Window* window, Uint32 x, Uint32 y);
void render(Window* window);
Window* create(const char* name, Uint32 width, Uint32 height);
void cleanup(Window* window);