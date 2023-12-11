#ifndef LGMOTORS_H
#define LGMOTORS_H


#include "mbed.h"
#include "Vector.h"
#include "TrigonometricTable.h"

#define DEG_M1 135
#define DEG_M2 -135
#define DEG_M3 -45
#define DEG_M4 45
//1正転 -1逆転
#define DIR_M1 -1
#define DIR_M2 -1
#define DIR_M3 -1
#define DIR_M4 -1

class Motors {
public:

Motors(PinName _pwm1,PinName _pwm2,PinName _pwm3,PinName _pwm4,PinName _comSleep);
void run(uint8_t index,float value);
void run(float p1,float p2,float p3,float p4);

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
short Deg_m4;

private:

//Motors

DigitalIn m1_;
DigitalIn m2_;
DigitalIn m3_;
DigitalIn m4_;


PwmOut m1;
PwmOut m2;
PwmOut m3;
PwmOut m4;

//Common motors
DigitalOut slp;
Timer timer;

};

#endif
