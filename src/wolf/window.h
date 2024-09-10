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

void render(Window* window);
Window* create(const char* name, Uint32 width, Uint32 height);
void cleanup(Window* window);