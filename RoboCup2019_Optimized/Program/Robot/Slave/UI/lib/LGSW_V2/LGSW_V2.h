#define LGSW_H
#ifdef LGSW_H



#include "mbed.h"

class LGSW {
public:
LGSW(PinName sw1,PinName sw2,PinName sw3);
LGSW(PinName sw1,PinName sw2,PinName sw3,PinName sp);
void SetSound(bool f);
void  SetMode(short mode);
void  SetSw1(bool state);
short GetMode();
bool  GetSw1State();
bool Sound();

void ReadLongPusshPlus();
void ReadLongPusshMinus();

DigitalIn SW1;
DigitalIn SW2;
DigitalIn SW3;
PwmOut SP;

Ticker sampling;


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
