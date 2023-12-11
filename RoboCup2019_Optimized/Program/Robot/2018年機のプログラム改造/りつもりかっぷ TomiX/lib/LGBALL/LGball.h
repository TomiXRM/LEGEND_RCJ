#ifndef LGball_H
#define LGball_H
#define  average_num 1
#include "mbed.h"
#include "setup.h"

class LGball {
public:
LGball();
short adjustment(short A,short B,short C);
void frequency(int freq);
void Setinterval(uint16_t us);
short read();
short read(uint16_t error_angle);
uint16_t readIRlong();
uint16_t readIRangle();
short IRangle;
short IRlong;
short IRreceive;
short intdat;
private:
DigitalOut device_call;
Timer timer;
RawSerial device;
unsigned char highbyte;
unsigned char lowbyte;

char c;
bool isok;
uint16_t interval;
short _IR;
signed short l[average_num];


};
#endif
