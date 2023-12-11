#ifndef LGKIKCER_H
#define LGKIKCER_H
#include "mbed.h"
#include "timeout.h"
class LGKicker {
public:
LGKicker(PinName KickerPin,PinName photo);
bool Kick(void);
short read(void);
float SetThreshold(float i);
DigitalOut Kicker;
DigitalOut SW;
mbed::Timer timer;
bool enabled;
private:
float Threshold;

mbed::Timeout Kicker_timeout;
mbed::Timeout KickerIsRedey;
void  flip(void);
void flipOn(void);
};


#endif
