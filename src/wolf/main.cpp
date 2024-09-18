#include <SDL2/SDL.h>
#include <iostream>
#include "window.h"
#include "map.h"
#include "generic.h"

#define minimapN 9
#define minimapS 28
#define minimapPlS 6


float fract(float a){
    if(a < 0){
        while (a <= -1){
            a++;
        }
        return a;
    }
    if(a > 0){
        while (a >= 1){
            a--;
        }
        return a;
    }
    return 0;
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

#define renW (winW-minimapS*minimapN)
#define minimapNS minimapN*minimapS

#define m_forw  0b100000
#define m_backw 0b010000
#define m_left  0b000010
#define m_right 0b000001
#define t_left  0b001000
#define t_right 0b000100

Window* win;
Player* plr;
bool quit;
float spd, spdH, spdA;
int plrSX, plrSY;
Uint8 mov;

void prepMap(){
    mapData = map->map;
    plr->x = mapS*(map->start.x+.5);
    plr->y = mapS*(map->start.y+.5);
    plr->a = 90;
}
void prepMap(int mapN, bool clean){
    if(clean) mapDispose(map);
    if(mapN == 0) map = createMap0();
    if(mapN == 1) map = createMap1();
    prepMap();
}

void init(){
    SDL_Init(SDL_INIT_VIDEO);
    win = createFullScreen("Window");
    winH = win->h;
    winW = win->w;
    plr = (Player*)malloc(sizeof(Player));
    prepMap(1, false);
}

void cleanup(){
    mapDispose(map);
    cleanup(win);
    SDL_Quit();
}

void render(){
    render(win);
}

void handleEvents(){
    SDL_Event e;
    while (SDL_PollEvent(&e)){
        if (e.type == SDL_QUIT){
            quit = true;
        }
        if (e.type == SDL_KEYDOWN){
            int nextSX, nextSY, val;
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
            case SDLK_LEFT:
                spdH = movS;
                break;
            case SDLK_RIGHT:
                spdH = -movS;
                break;
            case SDLK_e:
                spdX = mapS/2*dcos(plr->a);
                spdY = mapS/2*dsin(plr->a);
                nextSX = (plr->x+spdX+plrS/2*sgn(spdX))/mapS;
                nextSY = (plr->y-spdY-plrS/2*sgn(spdY))/mapS;
                val = mapData[nextSY*map->w+nextSX];
                if(val == 2){
                    mapData[nextSY*map->w+nextSX] = 0;
                }
                if(val == 3){
                    prepMap(0, true);
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
            case SDLK_LEFT:
            case SDLK_RIGHT:
                spdH = 0;
                break;
            case SDLK_p:
                prepMap();
            default:
                break;
            }
        }
    }
}
rayMapCol* castRay(Window* win, Player* plr, float a, Uint8* cl){
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
    }else if(val == 3){
        cl[0] = 0;
        cl[1] = 255;
        cl[2] = 0;
    }
    rayMapCol* col = (rayMapCol*)malloc(sizeof(rayMapCol));
    col->d = sqrtf((x-plr->x)*(x-plr->x)+(y-plr->y)*(y-plr->y));
    col->mapValue = val;
    col->interceptPos = *getMapSectorFromPoint(x,y);
    return col;
}
void drawWallSlice(Window* win, int x, float h, float d, Uint8* cl){
    int y = floor(winH/2-h/2);
    Uint8 col[3];
    col[0] = cl[0]-(int)((float)d/400.0*cl[0]);
    col[1] = cl[1]-(int)((float)d/400.0*cl[1]);
    col[2] = cl[2]-(int)((float)d/400.0*cl[2]);
    int y1 = y+h;
    if(y < 0) y = 0;
    if(y1 >= winH) y1 = winH-1;
    drawLine(win, x,y,x,y1, col[0],col[1],col[2]);
}
void draw3d(){
    int renW_3d = renW-12;
    for(int i = 0; i < winH/2; i++){
        Uint8 clR = (int)(((winH-i)/(float)winH)*map->clC_r);
        Uint8 clG = (int)(((winH-i)/(float)winH)*map->clC_g);
        Uint8 clB = (int)(((winH-i)/(float)winH)*map->clC_b);
        SDL_Point p1,p2;
        p1.y = p2.y = i;
        p1.x = 0;
        p2.x = winW-1-(winW-renW_3d);
        drawLine(win, 0,i,winW-1-(winW-renW_3d),i, clR, clG, clB);
    }
    for(int i = 0; i < winH/2; i++){
        Uint8 clR = (int)(((i+winH/2)/(float)winH)*map->clF_r);
        Uint8 clG = (int)(((i+winH/2)/(float)winH)*map->clF_g);
        Uint8 clB = (int)(((i+winH/2)/(float)winH)*map->clF_b);
        SDL_Point p1,p2;
        p1.y = p2.y = i+winH/2;
        p1.x = 0;
        p2.x = winW-1-(winW-renW_3d);
        drawLine(win, 0,i+winH/2,winW-1-(winW-renW_3d),i+winH/2, clR, clG, clB);
    }
    float da = 70.0/(float)renW_3d;
    for(int i = 0; i < renW_3d; i++){
        float a = (plr->a)-35+(float)i*da;
        Uint8 cl[3];
        rayMapCol* col = castRay(win, plr, a, cl);
        float d = col->d * dcos(a-plr->a);
        drawWallSlice(win,winW-i-1-(winW-renW_3d),((float)winH)/2/d*mapS,d, cl);
    }
}
void drawPlr(){
    int x = renW;
    int plPos = minimapN*minimapS/2;
    SDL_Point lines[5];
    lines[4].x = lines[0].x = x+round(plPos+minimapPlS*dcos(plr->a+120));
    lines[4].y = lines[0].y = round(plPos-minimapPlS*dsin(plr->a+120));
    lines[1].x = x+round(plPos+minimapPlS*dcos(plr->a));
    lines[1].y = round(plPos-minimapPlS*dsin(plr->a));
    lines[2].x = x+round(plPos+minimapPlS*dcos(plr->a-120));
    lines[2].y = round(plPos-minimapPlS*dsin(plr->a-120));
    lines[3].x = x+round(plPos);
    lines[3].y = round(plPos);
    drawLines(win, lines, 5, 255,0,0);
}
void drawMap(){
    int x = renW;
    for(int i = 0; i < minimapN*minimapS/2; i++) for(int j = 0; j < minimapN*minimapS/2; j++){
        int i_fix = i-(minimapN*minimapS/4)+plr->x, j_fix = j-(minimapN*minimapS/4)+plr->y;
        mapPos* pos = getMapSectorFromPoint(i_fix, j_fix);
        Uint8 r,g,b;
        if(pos->y < 0 || pos->x < 0 || pos->y >= map->h || pos->x >= map->w || mapData[(pos->y)*map->w+(pos->x)])
             {r=map->clW_r;g=map->clW_g;b=map->clW_b;}
        else {r=map->clF_r;g=map->clF_g;b=map->clF_b;}
        fillRect(win, x+i*2,j*2,x+i*2+2,j*2+2,r,g,b);
    }
    drawPlr();
}
void drawStatBar(){
    fillRect(win, renW-12, 0 ,renW, winH-1, map->clW_r/2,map->clW_g/2,map->clW_b/2);
    fillRect(win, renW, minimapNS, winW-1, minimapNS+12, map->clW_r/2,map->clW_g/2,map->clW_b/2);
    fillRect(win, winW-minimapNS, minimapNS+12, winW-1, winH-1, map->clW_r,map->clW_g,map->clW_b);
}

void movePlr(){
    plr->a -= spdA;
    float spdX = spd*dcos(plr->a)-spdH*dsin(plr->a);
    float spdY = spd*dsin(plr->a)+spdH*dcos(plr->a);
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
}

void display(){
    while(true){
        movePlr();
        draw3d();
        drawMap();
        drawStatBar();
        render();
		handleEvents();
		if (quit) break;
    }
}

int main(){
    init();
    display();
    cleanup();
    return 0;
}

int WinMain(){return main();}