#pragma once

#include <SDL2/SDL.h>
// #include "generic.h"

typedef struct
{
    SDL_Window* win;
    SDL_Renderer* ren;
    // SDL_Surface* surf;
    // SDL_Texture* tex;
    // Uint32* pix;
    Sint32 w;
    Sint32 h;
} Window;

// typedef struct{
//     int x;
//     int y;
// } point_t;
// typedef struct{
//     int x;
//     int y;
//     int w;
//     int h;
// } rect_t;

typedef SDL_Point point_t;

void render(Window* window);
Window* createWindowed(const char* name, Uint32 width, Uint32 height);
Window* createFullScreen(const char* name);
void cleanup(Window* window);
void setColor(Window* window, Uint8 r, Uint8 b, Uint8 g);

// void drawPoint(Window* window, int x, int y, Uint8 r, Uint8 g, Uint8 b);
void drawLine(Window* window, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b);
void drawLines(Window* window, point_t* points, int nLines,/* bool cycle,*/ Uint8 r, Uint8 g, Uint8 b);
// void drawRect(Window* window, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b);
void fillRect(Window* window, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b);

// void drawPoint(Window* window, point_t* point, Uint8 r, Uint8 g, Uint8 b);
// void drawLine(Window* window, point_t* point1, point_t* point2, Uint8 r, Uint8 g, Uint8 b);
// void drawLines(Window* window, point_t* points, int nLines, bool cycle, Uint8 r, Uint8 g, Uint8 b);
// void drawRect(Window* window, point_t* point1, point_t* point2, Uint8 r, Uint8 g, Uint8 b);
// void fillRect(Window* window, point_t* point1, point_t* point2, Uint8 r, Uint8 g, Uint8 b);

// void drawRect(Window* window, rect_t* rect, Uint8 r, Uint8 g, Uint8 b);
// void fillRect(Window* window, rect_t* rect, Uint8 r, Uint8 g, Uint8 b);