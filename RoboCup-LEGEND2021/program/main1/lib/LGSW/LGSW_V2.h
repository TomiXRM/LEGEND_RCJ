#ifndef LGSW_H
#define LGSW_H

#define tone_A  262
#define tone_S  294
#define tone_D  330
#define tone_F  349
#define tone_G  392
#define tone_H  440
#define tone_J  494
#define tone_K  523
#define BZoff 7
#define volume 0.5
#include "mbed.h"

class LGSW {
public:
LGSW(PinName sw1,PinName sw2,PinName sw3);
LGSW(PinName sw1,PinName sw2,PinName sw3,PinName sp);
void setSound(bool f);
void setMode(short mode);
void setSw1(bool state);
short getMode();
bool getSw1State();
bool sound();
void soundIndex(short num);

void ReadLongPusshPlus();
void ReadLongPusshMinus();

DigitalIn SW1;
DigitalIn SW2;
DigitalIn SW3;
PwmOut SP;

Ticker sampling;

bool stateCenterLongPush;
int8_t lastSPM;

private:
void sample();
void callback();
void callback2();
void callback3();
Timer timer;

Timer PusshingTime1;
Timer CountCycle1;
Timer PusshingTime2;
Timer CountCycle2;
Timer PusshingTime0;

Timeout timeout;

bool laststates[3];
bool states[3];

bool Sw1State;
bool SPf;
short ModeState;
short PreviousMode;
short DifferenceMode;
};

#endif
