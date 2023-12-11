#ifndef Vector_h
#define Vector_h

#include "mbed.h"
#define PI 3.14159265358979323846264338327
typedef struct {
        short x;
        short y;
        short r;
} VectorXY;

extern VectorXY SetZero();
extern VectorXY makeXYFromRT(short radius, short theta);
extern VectorXY makeXY(short x, short y);

#endif /* Vector_hpp */
