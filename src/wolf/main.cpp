
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

map_t* map;
int* mapData;

typedef struct{
    float x;
    float y;
    float a;
} Player;

int winW = 1120;
int winH = 630;
#define plrS 4
#define rotS 3.0
#define movS 0.4

Window* win;
Player* plr;
bool quit;
float spd, spdA;
int plrSX, plrSY;

void prepMap(){
    mapData = (int*)calloc(map->w*map->h, sizeof(int));
    memcpy(mapData, map->map, map->w*map->h*sizeof(int));
    plr->x = mapS*(map->stX+.5);
    plr->y = mapS*(map->stY+.5);
    plr->a = 90;
}
void prepMap(int mapN){ //mapN is ingored for now
    map = createMap0();
    prepMap();
}

void init(){
    SDL_Init(SDL_INIT_VIDEO);
    win = create("Window",winW,winH);
    SDL_SetWindowFullscreen(win->win,SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_GetWindowSizeInPixels(win->win, &(win->w), &(win->h));
    winH = win->h;
    winW = win->w;
    plr = (Player*)malloc(sizeof(Player));
    prepMap(0);
}

void cleanup(){
    mapDispose(map);
    free(mapData);
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
                if(mapData[nextSY*map->w+nextSX] == 2){
                    mapData[nextSY*map->w+nextSX] = 0;
                }
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
            case SDLK_p:
                prepMap();
            default:
                break;
            }
        }
    }
}
float castRay(Window* win, Player* plr, float a, Uint8* cl){
    float dx = dcos(a);
    float dy = dsin(a);
    float x = plr->x;
    float y = plr->y;
    while(mapData[(int)y/mapS*map->w+(int)x/mapS]==0){
        x+=dx*.1;
        y-=dy*.1;
    }
    int val = mapData[(int)y/mapS*map->w+(int)x/mapS];
    if(val == 1){
        cl[0] = map->clW_r;
        cl[1] = map->clW_g;
        cl[2] = map->clW_b;
    }else if(val == 2){
        cl[0] = map->clD_r;
        cl[1] = map->clD_g;
        cl[2] = map->clD_b;
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
    int renW = winW-minimapN*minimapS;
    for(int i = 0; i < winH/2; i++){
        Uint8 clR = (int)(((winH-i)/(float)winH)*map->clC_r);
        Uint8 clG = (int)(((winH-i)/(float)winH)*map->clC_g);
        Uint8 clB = (int)(((winH-i)/(float)winH)*map->clC_b);
        SDL_SetRenderDrawColor(win->ren,clR,clG,clB,255);
        SDL_RenderDrawLine(win->ren, 0, i, winW-1-(winW-renW), i);
    }
    for(int i = 0; i < winH/2; i++){
        Uint8 clR = (int)(((i+winH/2)/(float)winH)*map->clF_r);
        Uint8 clG = (int)(((i+winH/2)/(float)winH)*map->clF_g);
        Uint8 clB = (int)(((i+winH/2)/(float)winH)*map->clF_b);
        SDL_SetRenderDrawColor(win->ren,clR,clG,clB,255);
        SDL_RenderDrawLine(win->ren, 0, i+winH/2, winW-1-(winW-renW), i+winH/2);
    }
    float da = 70.0/(float)renW;
    for(int i = 0; i < renW; i++){
        float a = (plr->a)-35+(float)i*da;
        Uint8 cl[3];
        float d = castRay(win, plr, a, cl);
        d = d * dcos(a-plr->a);
        drawWallSlice(win,winW-i-1-(winW-renW),((float)winH)/2/d*mapS,d, cl);
    }
}
void drawMap(){
    int x = winW-minimapN*minimapS;
    SDL_Rect rect;
    rect.w = minimapS;
    rect.h = minimapS;
    for(int i = 0; i < minimapN; i++) for(int j = 0; j < minimapN; j++){
        int jS = j-(minimapN/2)+plrSY, iS = i-(minimapN/2)+plrSX;
        if(jS < 0 || iS < 0 || jS >= map->h || iS >= map->w || mapData[(jS)*map->w+(iS)])
             SDL_SetRenderDrawColor(win->ren,map->clW_r,map->clW_g,map->clW_b,255);
        else SDL_SetRenderDrawColor(win->ren,map->clF_r,map->clF_g,map->clF_b,255);
        rect.x = x+i*minimapS;
        rect.y = j*minimapS;
        SDL_RenderFillRect(win->ren, &rect);
    }
    SDL_SetRenderDrawColor(win->ren,255,0,0,255);
    int plPos = minimapN*minimapS/2;
    SDL_Point lines[5];
    lines[0].x = x+round(plPos+minimapPlS*dcos(plr->a+120));
    lines[0].y = round(plPos-minimapPlS*dsin(plr->a+120));
    lines[1].x = x+round(plPos+minimapPlS*dcos(plr->a));
    lines[1].y = round(plPos-minimapPlS*dsin(plr->a));
    lines[2].x = x+round(plPos+minimapPlS*dcos(plr->a-120));
    lines[2].y = round(plPos-minimapPlS*dsin(plr->a-120));
    lines[3].x = x+round(plPos);
    lines[3].y = round(plPos);
    lines[4].x = lines[0].x;
    lines[4].y = lines[0].y;
    SDL_RenderDrawLines(win->ren, lines, 5);
}
void drawStatBar(){
    SDL_SetRenderDrawColor(win->ren,map->clW_r,map->clW_g,map->clW_b,255);
    SDL_Rect rect;
    rect.y = minimapN*minimapS;
    rect.x = winW-rect.y;
    rect.w = rect.y;
    rect.h = winH-rect.y;
    SDL_SetRenderDrawColor(win->ren,map->clW_r,map->clW_g,map->clW_b,255);
    SDL_RenderFillRect(win->ren, &rect);
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
        if(mapData[plrSY*map->w+nextSX]==0) {
            plr->x+=spdX/2;
        }
        if(mapData[nextSY*map->w+plrSX]==0){
            plr->y-=spdY/2;
        }
        if(mapData[nextSY*map->w+nextSX]==0){
            plr->y-=spdY/2;
            plr->x+=spdX/2;
        }
        plrSX = (int)(plr->x)/mapS;
        plrSY = (int)(plr->y)/mapS;
        draw3d();
        drawMap();
        drawStatBar();
        render();
    }
}

int main(){
    init();
    display();
    cleanup();
    return 0;
}

int WinMain(){return main();}