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
private:
DigitalOut Kicker;
AnalogIn SW;
float Threshold;

mbed::Timeout Kicker_timeout;
mbed::Timeout KickerIsRedey;
mbed::Timer timer;
void  flip(void);
void flipOn(void);
};


#endif
