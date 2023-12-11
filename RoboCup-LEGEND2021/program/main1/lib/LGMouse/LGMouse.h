#ifndef LGMOUSE_H
#define LGMOUSE_H
#include "mbed.h"
#include "movingAverage.h"
class LGMouse {
public:
LGMouse(RawSerial *port);
void read();
void reset();

int16_t mouseX,mouseXPrev;
int16_t mouseY,mouseYPrev;
int32_t x;
int32_t y;

int32_t dataXH,dataXL;
int32_t dataYH,dataYL;
private:
char head;
uint8_t data_length;

RawSerial *mouse;
Ticker t;
void calc();
};



#endif