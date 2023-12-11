#ifndef debug_h
#define debug_h

#include "mbed.h"
#include "SensorAPI.h"

class SerialDebug {

Ticker ticker;
float Period;

public:
RawSerial pc;
SerialDebug();
void setDebug(float period);
void enable();
void disable();
void callback();
void Ballprint();
void IMUprint();
private:

};

#endif
