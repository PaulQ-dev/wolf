#include "generic.h"

float dsin(float deg){
    return sin(deg/180.0*M_PI);
}
float dcos(float deg){
    return cos(deg/180.0*M_PI);
}
int sgn(float a){
    return a > 0 ? 1 : a < 0 ? -1 : 0;
}
int sgn(int a){
    return sgn((float)a);
}
float max(float a, float b){
    return a>b?a:b;
}
int max(int a, int b){
    return a>b?a:b;
}
float min(float a, float b){
    return a<b?a:b;
}
int min(int a, int b){
    return a<b?a:b;
}