#include "Vector.h"

VectorXY SetZero() {
        VectorXY rslt;
        rslt.x = 0;
        rslt.y = 0;
        rslt.r = 0;
        return rslt;
}

VectorXY makeXYFromRT(short radius, short theta) {

        VectorXY rslt;
        theta += 90;
        rslt.x = (short)(radius * cos(theta / 180.0 * PI));
        rslt.y = (short)(radius * sin(theta / 180.0 * PI));

        return rslt;
}

VectorXY makeXY(short x, short y) {

        VectorXY rslt;
        rslt.x = x;
        rslt.y = y;

        return rslt;
}
