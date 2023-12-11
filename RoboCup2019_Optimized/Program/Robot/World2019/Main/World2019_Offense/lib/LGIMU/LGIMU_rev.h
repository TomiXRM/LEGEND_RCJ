#ifndef LGIMU_H
#define LGIMU_H
#include "mbed.h"

class LGIMU {
public:
LGIMU(RawSerial* _serial);
int16_t getDeg();
uint8_t getVelocity();
int16_t dir;
uint8_t velocity;
private:
RawSerial *serial;
void receive();
uint8_t data_length;
unsigned char highbyte;
unsigned char lowbyte;
unsigned char head;
};
#endif