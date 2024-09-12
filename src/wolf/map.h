#pragma once

#include <string.h>
#include <stdlib.h>

#define mapS 16
typedef struct{
    int w,h,stX,stY;
    int clW_r, clW_g, clW_b;
    int clD_r, clD_g, clD_b;
    int clF_r, clF_g, clF_b;
    int clC_r, clC_g, clC_b;
    int* map;
} map_t;

typedef struct{
    int mapSX,mapSY;
    float d;
} rayMapCol;

void mapDispose(map_t* map);

map_t* createMap0();