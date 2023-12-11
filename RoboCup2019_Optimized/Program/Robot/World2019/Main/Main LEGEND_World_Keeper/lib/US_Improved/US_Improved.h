#include "mbed.h"

#define SAMPLE_NUMBER 10
class LGUS {

public:
LGUS(PinName trigPin, PinName echoPin);
//~LGUS();

uint16_t distance;

private:
InterruptIn echo;
DigitalOut trig;
Timer t;
Timeout interval;
uint16_t f[SAMPLE_NUMBER];
void echoFall();
void echoRise();
void Trigger();
};
