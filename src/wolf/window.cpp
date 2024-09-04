#include "window.h"

using namespace wolf;

Uint32 window::getPixel(Uint32 x, Uint32 y){
    return pix[x*winH+y];
}
void window::setPixel(Uint32 x, Uint32 y, Uint32 cl){
    pix[x*winH+y] = cl;
}

void window::render(bool clean){
    if (clean) SDL_RenderClear(ren);
    tex = SDL_CreateTextureFromSurface(ren,surf);
    SDL_RenderCopy(ren, tex, NULL, NULL);
    SDL_RenderPresent(ren);
}
void window::init(Uint32 w, Uint32 h, const char* title){
    win = SDL_CreateWindow(title, 100, 100, w, h, SDL_WINDOW_SHOWN);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    surf = SDL_GetWindowSurface(win);
    pix = (Uint32*)(surf->pixels);
}
window::~window(){
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
}