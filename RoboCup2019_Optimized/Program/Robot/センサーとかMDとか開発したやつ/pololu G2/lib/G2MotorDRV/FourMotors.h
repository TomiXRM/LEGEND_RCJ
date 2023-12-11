#ifndef FOURMOTORS_H
#define FOURMOTORS_H

#include "mbed.h"
#define PI 3.14159265358979323846264338327
#define H 1
#define L 0

class Motor {
public:
Motor(PinName MD1,PinName PWM1,PinName MD2,PinName PWM2,PinName MD3,PinName PWM3,PinName MD4,PinName PWM4);
void frequency(float freq);
void tweak(char par1,char par2,char par3,char par4);
int drive(int dig,float power,int HMCmot);
int drive(int dig,float power);
void move(float pwm1,float pwm2,float pwm3,float pwm4);
void move(float pwm1,float pwm2,float pwm3,float pwm4,int HMCmot);
void stop(float power);
float M1SA;
float M2SA;
float M3SA;
float M4SA;
float pwm1;
float pwm2;
float pwm3;
float pwm4;

private:
PwmOut _PWM1; DigitalOut _MD1;
PwmOut _PWM2; DigitalOut _MD2;
PwmOut _PWM3; DigitalOut _MD3;
PwmOut _PWM4; DigitalOut _MD4;
};

#endif
