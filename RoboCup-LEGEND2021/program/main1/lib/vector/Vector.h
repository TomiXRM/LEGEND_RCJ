#ifndef Vector_h
#define Vector_h

#include "mbed.h"
#ifndef PI
    #define PI 3.14159265359
#endif
typedef struct {
        int16_t x;
        int16_t y;
        int16_t r;
} VectorXY;

extern VectorXY setZero();
extern VectorXY makeXYFromRT(int16_t radius, int16_t theta);
extern VectorXY makeXY(int16_t x, int16_t y);
extern int16_t degBetween(int16_t deg1,int16_t deg2);
extern int16_t degBetween_signed(int16_t deg1,int16_t deg2);
#endif /* Vector_hpp */
