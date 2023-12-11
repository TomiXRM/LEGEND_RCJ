#ifndef LGball_H
#define LGball_H
#define  dirTranslate 1//1か-1

#include "mbed.h"

class LGball {
public:
LGball(RawSerial* _pc,PinName device_call);
short get();
void Receive();
uint8_t getDist();
short getDir();
short adjustment(uint8_t a, uint8_t b, uint8_t c,uint8_t d,uint8_t e);
short data;
uint8_t dist;
short dir;
short dirt;
private:
RawSerial* dev;
DigitalOut device_call;
unsigned char highbyte;
unsigned char lowbyte;
uint8_t data_length;
char head;

short data_;
uint8_t dist_;
short dir_;
bool received;//データの完全受信のフラグ
};
#endif
