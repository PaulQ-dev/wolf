
#include <SDL2/SDL.h>
#include <iostream>
#include "window.h"
#include <math.h>
#include "map.h"

#ifndef M_PI
#define M_PI 3.141592654
#endif

#define minimapN 9
#define minimapS 28
#define minimapPlS 6

float dsin(float deg){
    return sin(deg/180.0*M_PI);
}
float dcos(float deg){
    return cos(deg/180.0*M_PI);
}
int sgn(float a){
    return a > 0 ? 1 : a < 0 ? -1 : 0;
}

int map[24*24];

typedef struct{
    float x;
    float y;
    float a;
} Player;

#define winW 1120
#define winH 630
#define plrS 4
#define rotS 3.0
#define movS 0.4

Window* win;
Player* plr;
bool quit;
float spd, spdA;
int plrSX, plrSY;
bool mapM;



void init(){
    SDL_Init(SDL_INIT_VIDEO);
    win = create("Window",winW,winH);
    plr = (Player*)malloc(sizeof(Player));
    memcpy(map, mapAr, mapW*mapH*sizeof(int));
    plr->x = mapS*(plrStX+.5);
    plr->y = mapS*(plrStY+.5);
    plr->a = 90;
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
            int nextSX, nextSY;
            float spdX, spdY;
            switch (e.key.keysym.sym)
            {
            case SDLK_w:
                spd = movS;
                break;
            case SDLK_s:
                spd = -movS;
                break;
            case SDLK_d:
                spdA = rotS;
                break;
            case SDLK_a:
                spdA = -rotS;
                break;
            case SDLK_e:
                spdX = mapS/2*dcos(plr->a);
                spdY = mapS/2*dsin(plr->a);
                nextSX = (plr->x+spdX+plrS/2*sgn(spdX))/mapS;
                nextSY = (plr->y-spdY-plrS/2*sgn(spdY))/mapS;
                if(map[nextSY*mapW+nextSX] == 2){
                    map[nextSY*mapW+nextSX] = 0;
                }
                break;
            case SDLK_m:
                mapM = true;
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
            case SDLK_m:
                mapM = false;
                break;
            case SDLK_p:
                memcpy(map, mapAr, mapW*mapH*sizeof(int));
                plr->x = mapS*(plrStX+.5);
                plr->y = mapS*(plrStY+.5);
                plr->a = 90;
            default:
                break;
            }
        }
    }
}
float drawRay(Window* win, Player* plr, float a, Uint8* cl){
    float dx = dcos(a);
    float dy = dsin(a);
    float x = plr->x;
    float y = plr->y;
    while(map[(int)y/mapS*mapW+(int)x/mapS]==0){
        x+=dx*.1;
        y-=dy*.1;
    }
    int val = map[(int)y/mapS*mapW+(int)x/mapS];
    if(val == 1){
        cl[0] = 128;
        cl[1] = 128;
        cl[2] = 96;
    }else if(val == 2){
        cl[0] = 128;
        cl[1] = 128;
        cl[2] = 128;
    }
    return sqrtf((x-plr->x)*(x-plr->x)+(y-plr->y)*(y-plr->y));
}
void drawWallSlice(Window* win, int x, float h, float d, Uint8* cl){
    int y = floor(winH/2-h/2);
    Uint8 col[3];
    col[0] = cl[0]-(int)((float)d/400.0*cl[0]);
    col[1] = cl[1]-(int)((float)d/400.0*cl[1]);
    col[2] = cl[2]-(int)((float)d/400.0*cl[2]);
    SDL_SetRenderDrawColor(win->ren,col[0],col[1],col[2],255);
    SDL_RenderDrawLine(win->ren, x,y,x,y+h);
}
void draw3d(){
    for(int i = 0; i < winH/2; i++){
        Uint8 cl = (int)(((winH-i)/(float)winH)*48);
        SDL_SetRenderDrawColor(win->ren,0,0,cl,255);
        SDL_RenderDrawLine(win->ren, 0, i, winW-1, i);
    }
    for(int i = 0; i < winH/2; i++){
        Uint8 cl = (int)(((i+winH/2)/(float)winH)*64);
        SDL_SetRenderDrawColor(win->ren,cl,cl,cl,255);
        SDL_RenderDrawLine(win->ren, 0, i+winH/2, winW-1, i+winH/2);
    }
    int rays = winW;
    float da = 70.0/(float)rays;
    for(int i = 0; i < rays; i++){
        float a = (plr->a)-35+(float)i*da;
        Uint8 cl[3];
        float d = drawRay(win, plr, a, cl);
        d = d * dcos(a-plr->a);
        drawWallSlice(win,winW-i-2,((float)winH)/2/d*mapS,d, cl);
    }
}
void draw2d(){
    int x = winW-minimapN*minimapS;
    SDL_Rect rect;
    rect.w = minimapS;
    rect.h = minimapS;
    for(int i = 0; i < minimapN; i++) for(int j = 0; j < minimapN; j++){
        int jS = j-(minimapN/2)+plrSY, iS = i-(minimapN/2)+plrSX;
        if(jS < 0 || iS < 0 || jS > mapH || iS > mapW || map[(jS)*mapW+(iS)]) SDL_SetRenderDrawColor(win->ren,255,255,255,255);
        else SDL_SetRenderDrawColor(win->ren,0,0,0,255);
        rect.x = x+i*minimapS;
        rect.y = j*minimapS;
        SDL_RenderFillRect(win->ren, &rect);
    }
    SDL_SetRenderDrawColor(win->ren,255,0,0,255);
    int plPos = minimapN*minimapS/2;
    SDL_Point lines[3];
    lines[0].x = x+round(plPos+minimapPlS*dcos(plr->a+120));
    lines[0].y = round(plPos-minimapPlS*dsin(plr->a+120));
    lines[1].x = x+round(plPos+minimapPlS*dcos(plr->a));
    lines[1].y = round(plPos-minimapPlS*dsin(plr->a));
    lines[2].x = x+round(plPos+minimapPlS*dcos(plr->a-120));
    lines[2].y = round(plPos-minimapPlS*dsin(plr->a-120));
    SDL_RenderDrawLines(win->ren, lines, 3);
}

void display(){
    while(true){
		handleEvents();
		if (quit) break;
        plr->a -= spdA;
        float spdX = spd*dcos(plr->a);
        float spdY = spd*dsin(plr->a);
        int nextSX = (plr->x+spdX+plrS/2*sgn(spdX))/mapS;
        int nextSY = (plr->y-spdY-plrS/2*sgn(spdY))/mapS;
        if(map[plrSY*mapW+nextSX]==0) {
            plr->x+=spdX/2;
        }
        if(map[nextSY*mapW+plrSX]==0){
            plr->y-=spdY/2;
        }
        if(map[nextSY*mapW+nextSX]==0){
            plr->y-=spdY/2;
            plr->x+=spdX/2;
        }
        plrSX = (int)(plr->x)/mapS;
        plrSY = (int)(plr->y)/mapS;
        draw3d();
        if (mapM) draw2d();
        render();
    }
}

int main(){
    init();
    display();
    cleanup();
    return 0;
}

