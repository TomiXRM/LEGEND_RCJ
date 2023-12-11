#ifndef LGKIKCER_H
#define LGKIKCER_H
#include "mbed.h"
// #include "timeout.h"
class LGKicker {
public:
LGKicker(PinName KickerPin);
bool Kick(void);
DigitalOut Kicker;
Timer timer;
bool enabled;
private:
float Threshold;

Timeout Kicker_timeout;
Timeout KickerIsRedey;
void  flip(void);
void flipOn(void);
};


#endif
