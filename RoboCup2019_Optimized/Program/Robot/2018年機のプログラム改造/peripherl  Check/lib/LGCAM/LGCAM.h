#ifndef LG_CAM_H
#define LG_CAM_H

#include "mbed.h"

class LGCAM {
public:
LGCAM(PinName rx, PinName inter);
void frequency(int freq);
void Setinterval(uint16_t us);
char receive();
short read(int mode);
private:
RawSerial device;
DigitalOut device_call;
Timer timer;
char highbyte;
char lowbyte;
short intdat;
char c;

short a;
short b;
short x;
short y;
short dir;

uint8_t interval;
bool isok;
};
#endif
