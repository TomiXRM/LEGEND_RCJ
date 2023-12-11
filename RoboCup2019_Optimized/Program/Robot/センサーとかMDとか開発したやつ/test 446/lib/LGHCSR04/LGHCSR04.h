#ifndef LGHCSR04_H
#define LGHCSR04_H

#include "mbed.h"
class LG_HCSR04 {
private:
Timer _timer;
Timer pulsetime;
DigitalInOut _pin(Trig_PIN)
Timer _timer;

int _Time;
int _SPEED_OF_SOUND_CM;       /* in milliseconds */

public:
LGHCSR04::LGHCSR04(PinName PIN);


};
#endif
