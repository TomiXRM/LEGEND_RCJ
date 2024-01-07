#ifndef LGDRIB_H
#define LGDRIB_H
#include "mbed.h"

class LGDrib {
public:
LGDrib(PinName pin);
void dribIRQ0r();
void dribIRQ0l();
void dribIRQright();
void dribIRQleft();
void state(bool state);

private:
DigitalOut drib;
Timeout dribOnTime;
Timeout dribOffTime;
bool isDribOn;
};
#endif
