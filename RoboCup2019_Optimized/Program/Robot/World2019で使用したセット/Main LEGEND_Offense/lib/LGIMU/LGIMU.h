#ifndef LGIMU_H
#define LGIMU_H

#include "mbed.h"

class LGIMU {
private:
RawSerial serial;
DigitalOut init;
Ticker TrigerInterrupt;

uint8_t data_length;
unsigned char highbyte;
unsigned char lowbyte;
unsigned char head;

public:

int16_t dir;
uint8_t velocity;
LGIMU(PinName _init,PinName _rx);
void setGetInterval(uint16_t interval);
void disableGetInterrupt();
void receive();
void get();
int16_t getDeg();
uint8_t getVelocity();
};

#endif
