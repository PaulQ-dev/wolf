
#include <SDL2/SDL.h>
#include <iostream>
#include "window.h"
#include <math.h>

//-----------------------------MAP----------------------------------------------
#define mapX  8      //map width
#define mapY  8      //map height
#define mapS 48      //map cube size

float dsin(float deg){
    return sin(deg/180.0*M_PI);
}
float dcos(float deg){
    return cos(deg/180.0*M_PI);
}
int sgn(float a){
    return a > 0 ? 1 : a < 0 ? -1 : 0;
}

int map[]=           //the map array. Edit to change level but keep the outer walls
{
    1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,1,1,0,1,
    1,0,0,0,0,1,0,1,
    1,0,0,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,
};

typedef struct{
    float x;
    float y;
    float a;
} Player;

#define WW 640
#define WH 480
#define PS 10

Window* win;
Player* plr;
bool quit;
float spd, spdA;
int plrSX, plrSY;



void init(){
    SDL_Init(SDL_INIT_VIDEO);
    win = create("Window",WW,WH);
    plr = (Player*)malloc(sizeof(Player));
    plr->x = mapX*mapS/2;
    plr->y = mapY*mapS/2;
}

void cleanup(){
    cleanup(win);
    SDL_Quit();
}

void render(){
    SDL_SetRenderDrawColor(win->ren,0,0,0,255);
    render(win);
}

void handleEvents(){
    SDL_Event e;
    while (SDL_PollEvent(&e)){
        if (e.type == SDL_QUIT){
            quit = true;
        }
        if (e.type == SDL_KEYDOWN){
            switch (e.key.keysym.sym)
            {
            case SDLK_w:
                spd = .5;
                break;
            case SDLK_s:
                spd = -.5;
                break;
            case SDLK_d:
                spdA = 5;
                break;
            case SDLK_a:
                spdA = -5;
                break;
            default:
                break;
            }
        }
        if (e.type == SDL_KEYUP){
            switch (e.key.keysym.sym){
            case SDLK_ESCAPE:
                quit = true;
                break;
            case SDLK_w:
            case SDLK_s:
                spd = 0;
                break;
            case SDLK_d:
            case SDLK_a:
                spdA = 0;
                break;
            default:
                break;
            }
        }
    }
}
void drawMap(Window* win, int x, int y, int size, int mw, int mh, int* map){
    for(int i = 0; i < mw; i++) for(int j = 0; j < mh; j++){
        if(map[j*mw+i]) SDL_SetRenderDrawColor(win->ren,255,255,255,255);
        else SDL_SetRenderDrawColor(win->ren,0,0,0,255);
        SDL_Rect rect;
        rect.x = x+i*size;
        rect.y = y+j*size;
        rect.w = size;
        rect.h = size;
        SDL_RenderFillRect(win->ren, &rect);
    }
}
void drawPlr(Window* win, Player* plr, int size){
    SDL_SetRenderDrawColor(win->ren,255,255,255,255);
    float dx = plr->x+(size/2)*dcos(plr->a);
    float dy = plr->y-(size/2)*dsin(plr->a);
    for(int i = 0; i < 360; i++){
        SDL_RenderDrawPoint(win->ren,round(plr->x+(size/2)*dcos(i)),round(plr->y+(size/2)*dsin(i)));
    }
    SDL_SetRenderDrawColor(win->ren,255,0,0,255);
    SDL_RenderDrawLine(win->ren,round(plr->x),round(plr->y),round(dx),round(dy));
}
float drawRay(Window* win, Player* plr, float a){
    float dx = dcos(a);
    float dy = dsin(a);
    float x = plr->x;
    float y = plr->y;

    while(map[(int)y/mapS*mapX+(int)x/mapS]==0){
        x+=dx*.1;
        y-=dy*.1;
    }
    SDL_SetRenderDrawColor(win->ren,255,255,0,255);
    // SDL_RenderDrawLine(win->ren, plr->x+dcos(plr->a)*PS/2, plr->y-dsin(plr->a)*PS/2, x, y);
    return sqrtf((x-plr->x)*(x-plr->x)+(y-plr->y)*(y-plr->y));
}
void drawWallSlice(Window* win, int x, float h, float d){
    int y = floor(WH/2-h/2);
    int cl = 255-(int)((float)d/800.0*255);
    SDL_SetRenderDrawColor(win->ren,cl,cl,0,255);
    SDL_RenderDrawLine(win->ren, x,y,x,y+h);
    SDL_RenderDrawLine(win->ren, x+1,y,x+1,y+h);
}

void display(){
    while(true){
		handleEvents();
		if (quit) break;
        plr->a -= spdA;
        float spdX = spd*dcos(plr->a);
        float spdY = spd*dsin(plr->a);
        int nextSX = (plr->x+spdX+PS/2*sgn(spdX))/mapS;
        int nextSY = (plr->y-spdY-PS/2*sgn(spdY))/mapS;
        if(map[plrSY*mapX+nextSX]==0) {
            plr->x+=spdX/2;
        }
        if(map[nextSY*mapX+plrSX]==0){
            plr->y-=spdY/2;
        }
        if(map[nextSY*mapX+nextSX]==0){
            plr->y-=spdY/2;
            plr->x+=spdX/2;
        }
        plrSX = (int)(plr->x)/mapS;
        plrSY = (int)(plr->y)/mapS;
        // drawMap(win,0,0,mapS,mapX,mapY,map);
        SDL_Rect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = WW;
        rect.h = WH/2;
        SDL_SetRenderDrawColor(win->ren, 0,0,0,255);
        SDL_RenderFillRect(win->ren, &rect);
        rect.y = WH/2;
        SDL_SetRenderDrawColor(win->ren, 63,63,63,255);
        SDL_RenderFillRect(win->ren, &rect);
        int rays = WW/2;
        float da = 70.0/(float)rays;
        for(int i = 0; i < rays; i++){
            float a = (plr->a)-35+(float)i*da;
            float d = drawRay(win, plr, a);
            d = d * dcos(a-plr->a);
            drawWallSlice(win,WW-i*2-2,((float)WH)/2/d*mapS,d);
        }
        // drawPlr(win, plr, PS);
        render();
    }
}

int main(){
    init();
    display();
    cleanup();
    return 0;
}

