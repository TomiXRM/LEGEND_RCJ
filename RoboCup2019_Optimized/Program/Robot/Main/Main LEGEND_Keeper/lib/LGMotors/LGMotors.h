#ifndef LGMOTORS_H
#define LGMOTORS_H


#include "mbed.h"
#include "Vector.h"

class Motors {
public:

Motors(PinName _dir1,PinName _dir2,PinName _dir3,PinName _comSleep);
void run(uint8_t index,float value);
void run(float p1,float p2,float p3);

void stopAll();
void stop(uint8_t index);
int drive(int deg,float power,float degree);
void driveVector(VectorXY vec,float Output);
//Wrapper functions
void period(float seconds);
void period_us(int us);
void period_ms(int ms);

short Deg_m1;
short Deg_m2;
short Deg_m3;

private:

//Motors

DigitalIn m1_;
DigitalIn m2_;
DigitalIn m3_;


PwmOut m1;
PwmOut m2;
PwmOut m3;

//Common motors
DigitalOut slp;


};

#endif
