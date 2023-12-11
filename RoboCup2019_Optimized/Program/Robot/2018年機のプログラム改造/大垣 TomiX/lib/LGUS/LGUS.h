#ifndef LGUS_H
#define LGUS_H
#include "mbed.h"

class LGUS {
public:
LGUS(PinName tx, PinName rx,PinName inter);
void frequency(int freq);
void Setinterval(uint16_t us);
short read();
char read1byte();

void receive();
unsigned char read_F();
unsigned char read_R();
unsigned char read_B();
unsigned char read_L();

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

volatile short us1;
volatile short us2;
volatile short us3;
volatile short us4;
};
#endif
