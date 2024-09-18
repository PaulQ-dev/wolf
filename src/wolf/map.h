#pragma once

#include <string.h>
#include <stdlib.h>

typedef struct{
    int x;
    int y;
} mapPos;

#define mapS 16
typedef struct{
    int w,h;
    mapPos start;
    int clW_r, clW_g, clW_b;
    int clD_r, clD_g, clD_b;
    int clF_r, clF_g, clF_b;
    int clC_r, clC_g, clC_b;
    int* map;
} map_t;

typedef struct{
    mapPos interceptPos;
    int mapValue;
    float d;
} rayMapCol;

void mapDispose(map_t* map);

map_t* createMap0();
map_t* createMap1();
mapPos* getMapSectorFromPoint(int x, int y);