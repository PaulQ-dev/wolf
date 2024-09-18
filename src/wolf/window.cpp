#include "window.h"

void render(Window* win){
    // SDL_DestroyTexture(win->tex);
    // win->tex = SDL_CreateTextureFromSurface(win->ren, win->surf);
    // SDL_RenderCopy(win->ren, win->tex, NULL, NULL);
    SDL_RenderPresent(win->ren);
    SDL_SetRenderDrawColor(win->ren,0,0,0,255);
    SDL_RenderClear(win->ren);
}
Window* createFullScreen(const char* name){
    Window* win = (Window*)malloc(sizeof(Window));
    win->win = SDL_CreateWindow(name,10,10,640,480,SDL_WINDOW_SHOWN);
    win->ren = SDL_CreateRenderer(win->win,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetWindowFullscreen(win->win, SDL_WINDOW_FULLSCREEN_DESKTOP);
    // win->surf = SDL_GetWindowSurface(win->win);
    // win->tex = SDL_CreateTextureFromSurface(win->ren, win->surf);
    // win->pix = (Uint32*)(win->surf->pixels);
    int w,h;
    SDL_GetWindowSizeInPixels(win->win, &w, &h);
    win->w = w;
    win->h = h;
    return win;
}
Window* createWindowed(const char* name, Uint32 w, Uint32 h){
    Window* win = (Window*)malloc(sizeof(Window));
    win->win = SDL_CreateWindow(name,10,10,w,h,SDL_WINDOW_SHOWN);
    win->ren = SDL_CreateRenderer(win->win,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // win->surf = SDL_GetWindowSurface(win->win);
    // win->tex = SDL_CreateTextureFromSurface(win->ren, win->surf);
    // win->pix = (Uint32*)(win->surf->pixels);
    win->w = w;
    win->h = h;
    return win;
}
void cleanup(Window* win){
    // SDL_DestroyTexture(win->tex);
    // SDL_FreeSurface(win->surf);
    SDL_DestroyRenderer(win->ren);
    SDL_DestroyWindow(win->win);
    free(win);
}
void setColor(Window* win, Uint8 r, Uint8 g, Uint8 b){
    SDL_SetRenderDrawColor(win->ren,r,g,b,255);
}
void drawLine(Window* win, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b){
    setColor(win,r,g,b);
    SDL_RenderDrawLine(win->ren,x1,y1,x2,y2);
}
void drawLines(Window* win, point_t* points, int nLines, Uint8 r, Uint8 g, Uint8 b){
    setColor(win,r,g,b);
    SDL_RenderDrawLines(win->ren,points,nLines);
}
void fillRect(Window* win, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b){
    setColor(win,r,g,b);
    SDL_Rect rect;
    rect.x = x1;
    rect.y = y1;
    rect.w = abs(x1-x2);
    rect.h = abs(y1-y2);
    SDL_RenderFillRect(win->ren,&rect);
}

// void drawPoint(Window* win, int x, int y, Uint8 r, Uint8 g, Uint8 b){
//     if((x >= 0 && x < win->w) && (y >= 0 && y < win->h)){
//         Uint32 cl = b+(0x100*g)+(0x10000*r);
//         win->pix[(y*(win->w))+x] = cl;
//     }
// }
// void drawLine(Window* win, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b){
//     int dx = abs(x2 - x1);
//     int sx = x1 < x2 ? 1 : -1;
//     int dy = -abs(y2 - y1);
//     int sy = y1 < y2 ? 1 : -1;
//     int error = dx + dy;
//     while (true){
//         drawPoint(win, x1, y1, r,g,b);
//         if (x1 == x2 && y1 == y2) break;
//         int e2 = 2 * error;
//         if (e2 >= dy){
//             error = error + dy;
//             x1 = x1 + sx;
//         }
//         if (e2 <= dx){
//             error = error + dx;
//             y1 = y1 + sy;
//         }
//     }
// }
// void fillRect(Window* win, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b){
//     int px1,px2,py1,py2;
//     py1 = y1;
//     py2 = y2;
//     for(px1 = min(x1,x2); px1 <= max(x2,x1); px2 = px1++){
//         drawLine(win, px1,py1,px2,py2, r, g, b);
//     }
// }
// void drawLines(Window* win, int* x_arr, int* y_arr, int nLines, bool cycle, Uint8 r, Uint8 g, Uint8 b){
//     for(int i = 0; i < nLines; i++){
//         if(cycle){
//             drawLine(win, x_arr[i],y_arr[i],x_arr[(i+1)%nLines],y_arr[(i+1)%nLines],r,g,b);
//         }
//         else {
//             if(i == 0) continue;
//             drawLine(win, x_arr[i-1],y_arr[i-1],x_arr[i],y_arr[i],r,g,b);
//         }
//     }
// }
// void drawRect(Window* win, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b){
//     int x[] = {x1,x2,x2,x1}, y[] = {y1,y1,y2,y2};
//     drawLines(win,x,y,4,true,r,g,b);
// }

// void drawPoint(Window* win, point_t* pt, Uint8 r, Uint8 g, Uint8 b) {drawPoint(win,pt->x,pt->y,r,g,b);}
// void drawLine(Window* win, point_t* pt1, point_t* pt2, Uint8 r, Uint8 g, Uint8 b) {drawLine(win,pt1->x,pt1->y,pt1->x,pt1->y,r,g,b);}
// void drawLines(Window* win, point_t* pt_arr, int nLines, bool cycle, Uint8 r, Uint8 g, Uint8 b){
//     int x[nLines],y[nLines];
//     for(int i = 0; i < nLines; i++){
//         x[i] = pt_arr[i].x;
//         y[i] = pt_arr[i].y;
//     }
//     drawLines(win,x,y,nLines,cycle,r,g,b);
// }
// void drawRect(Window* win, point_t* pt1, point_t* pt2, Uint8 r, Uint8 g, Uint8 b){drawRect(win,pt1->x,pt1->y,pt1->x,pt1->y,r,g,b);}
// void fillRect(Window* win, point_t* pt1, point_t* pt2, Uint8 r, Uint8 g, Uint8 b){fillRect(win,pt1->x,pt1->y,pt1->x,pt1->y,r,g,b);}
// void drawRect(Window* win, rect_t* rect, Uint8 r, Uint8 g, Uint8 b){drawRect(win,rect->x,rect->y,rect->x+rect->w,rect->y+rect->h,r,g,b);}
// void fillRect(Window* win, rect_t* rect, Uint8 r, Uint8 g, Uint8 b){fillRect(win,rect->x,rect->y,rect->x+rect->w,rect->y+rect->h,r,g,b);}