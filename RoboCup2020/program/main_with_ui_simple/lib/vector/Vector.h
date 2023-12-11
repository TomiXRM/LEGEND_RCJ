#ifndef Vector_h
#define Vector_h

#include "mbed.h"
#ifndef PI
    #define PI 3.14159265359
#endif
typedef struct {
        short x;
        short y;
        short r;
} VectorXY;

extern VectorXY setZero();
extern VectorXY makeXYFromRT(short radius, short theta);
extern VectorXY makeXY(short x, short y);

#endif /* Vector_hpp */
