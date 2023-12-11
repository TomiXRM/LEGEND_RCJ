#include "Vector.h"

VectorXY setZero() {
        VectorXY rslt;
        rslt.x = 0;
        rslt.y = 0;
        rslt.r = 0;
        return rslt;
}

VectorXY makeXYFromRT(int16_t radius, int16_t theta) {

        VectorXY rslt;
        theta += 90;
        rslt.x = (int16_t)(radius * cos(theta / 180.0 * PI));
        rslt.y = (int16_t)(radius * sin(theta / 180.0 * PI));

        return rslt;
}

VectorXY makeXY(int16_t x, int16_t y) {

        VectorXY rslt;
        rslt.x = x;
        rslt.y = y;

        return rslt;
}

int16_t degBetween(int16_t deg1,int16_t deg2){
        int16_t a = deg1 - deg2;
        while(a < 0) a += 360;
        while(a > 180) a -= 360;
        return abs(a);
}

int16_t degBetween_signed(int16_t deg1,int16_t deg2){
        int16_t a = deg1 - deg2;
        while(a < 0) a += 360;
        while(a > 180) a -= 360;
        return a;
}



