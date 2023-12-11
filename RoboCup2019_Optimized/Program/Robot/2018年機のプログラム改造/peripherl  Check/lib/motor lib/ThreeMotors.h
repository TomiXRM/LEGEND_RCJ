#ifndef THREEMOTORS_H
#define THREEMOTORS_H

#include "mbed.h"
#define PI 3.14159265358979323846264338327
#define H 1
#define L 0
class Motor {
public:
Motor(PinName INA1,PinName INB1,PinName PWM1,PinName INA2,PinName INB2,PinName PWM2,PinName INA3,PinName INB3,PinName PWM3);
void frequency(float freq);
void tweak(char par1,char par2,char par3);
int drive(int dig,float power,int HMCmot);
void move(float pwm1,float pwm2,float pwm3);
float M1SA;
float M2SA;
float M3SA;
float pwm1;
float pwm2;
float pwm3;

private:
PwmOut _PWM1; DigitalOut _INA1; DigitalOut _INB1;
PwmOut _PWM2; DigitalOut _INA2; DigitalOut _INB2;
PwmOut _PWM3; DigitalOut _INA3; DigitalOut _INB3;
};

#endif
