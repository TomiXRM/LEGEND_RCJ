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

InterruptIn SW1;
InterruptIn SW2;
InterruptIn SW3;
PwmOut SP;


private:
Timer timer;

Timer PusshingTime1;
Timer CountCycle1;
Timer PusshingTime2;
Timer CountCycle2;

Timeout timeout;
void Sw1Rise();
void Sw2Rise();
void Sw3Rise();
void Sw1Fall();
void Sw2Fall();
void Sw3Fall();
void callback();
void callback2();
void callback3();


bool Sw1State;
bool SPf;
short ModeState;
short PreviousMode;
short DifferenceMode;
};

#endif
