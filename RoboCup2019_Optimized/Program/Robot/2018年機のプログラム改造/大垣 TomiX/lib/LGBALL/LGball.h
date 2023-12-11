#ifndef LGball_H
#define LGball_H
#define  average_num 1
#include "mbed.h"

class LGball {
public:
LGball(PinName rx, PinName inter);
short adjustment(short A,short B,short C);
void frequency(int freq);
void Setinterval(uint16_t us);
short read();
short read(uint16_t error_angle);
uint16_t readIRlong();
uint16_t readIRangle();
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

short IRlong;
short _IR;
signed short IRangle;
signed short l[average_num];


};
#endif
