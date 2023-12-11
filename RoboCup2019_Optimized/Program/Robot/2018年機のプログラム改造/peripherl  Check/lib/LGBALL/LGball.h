#ifndef LGball_H
#define LGball_H
#define Ver1
#include "mbed.h"

class LGball {
public:
LGball(PinName rx, PinName inter);
void frequency(int freq);
void Setinterval(uint16_t us);
short read();
private:
DigitalOut device_call;
Timer timer;
RawSerial device;
unsigned char highbyte;
unsigned char lowbyte;
short intdat;
char c;
bool isok;
uint16_t interval;
};
#endif
