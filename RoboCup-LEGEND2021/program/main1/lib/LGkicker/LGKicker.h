#ifndef LGKIKCER_H
#define LGKIKCER_H
#include "mbed.h"
// #include "timeout.h"
class LGKicker {
public:
LGKicker(PinName KickerPin);
bool Kick(void);
void setPower(uint16_t p);
void setPower();
DigitalOut Kicker;
// PwmOut Kicker;
Timer timer;
bool enabled;
float power;
private:
float Threshold;
uint16_t time;
Timeout Kicker_timeout;
Timeout KickerIsRedey;
void  flip(void);
void flipOn(void);
};


#endif
