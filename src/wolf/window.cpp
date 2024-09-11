#include "window.h"

void render(Window* win){
    SDL_RenderPresent(win->ren);
}
Window* create(const char* name, Uint32 w, Uint32 h){
    Window* win = (Window*)malloc(sizeof(Window));
    win->win = SDL_CreateWindow(name,10,10,w,h,SDL_WINDOW_SHOWN);
    win->ren = SDL_CreateRenderer(win->win,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    win->surf = SDL_GetWindowSurface(win->win);
    win->pix = (Uint32*)(win->surf->pixels);
    win->w = w;
    win->h = h;
    return win;
}
void cleanup(Window* win){
    SDL_DestroyTexture(win->tex);
    SDL_FreeSurface(win->surf);
    SDL_DestroyRenderer(win->ren);
    SDL_DestroyWindow(win->win);
    free(win);
}