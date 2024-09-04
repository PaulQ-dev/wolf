#include "window.h"

void drawPixel(Window* win, Uint32 x, Uint32 y, Uint32 cl){
    if(x > 0 || x < win->w || y > 0 || y < win->h){
        win->pix[(y*(win->w))+x] = cl;
    }
}
void drawRect(Window* win, Uint32 x, Uint32 y, Uint32 w, Uint32 h, Uint32 cl){
    for(int i = 0; i < w; i++){
        for(int j = 0 ; j < h; j++){
            drawPixel(win,x+i,y+j,cl);
        }
    }
}
Uint32 getPixel(Window* win, Uint32 x, Uint32 y){
    if(x > 0 || x < win->w || y > 0 || y < win->h){
        return win->pix[x*win->h+y];
    }
    return 0;
}
void render(Window* win){
    SDL_RenderClear(win->ren);
    win->tex = SDL_CreateTextureFromSurface(win->ren,win->surf);
    SDL_RenderCopy(win->ren, win->tex, NULL, NULL);
    SDL_RenderPresent(win->ren);
}
Window* create(const char* name, Uint32 w, Uint32 h){
    Window* win = (Window*)malloc(sizeof(Window));
    win->win = SDL_CreateWindow(name,100,100,w,h,SDL_WINDOW_SHOWN);
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