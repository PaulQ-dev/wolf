#pragma once

#include <SDL2/SDL.h>

namespace wolf{
    class window{
    private:
        Uint32 winW, winH;
        SDL_Window* win;
        SDL_Renderer* ren;
        SDL_Surface* surf;
        SDL_Texture* tex;
    public:
        Uint32* pix;
        Uint32 getPixel(Uint32 x, Uint32 y);
        void setPixel(Uint32 x, Uint32 y, Uint32 cl);
        void render(bool clean);
        void render() {render(false);}

        void init(Uint32 width, Uint32 height, const char* title);
        ~window();
    };
}