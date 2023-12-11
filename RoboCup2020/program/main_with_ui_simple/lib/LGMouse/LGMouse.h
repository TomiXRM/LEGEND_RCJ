#ifndef LGMOUSE_H
#define LGMOUSE_H
#include "mbed.h"

class LGMouse{
    public:
    LGMouse(PinName mouseRX);
    void read();
    signed char mouseX;
    signed char mouseY;


    private:
    char head;
    uint8_t data_length;

    RawSerial mouse;
};



#endif