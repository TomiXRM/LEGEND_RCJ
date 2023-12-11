#ifndef LGMOTORS_H
#define LGMOTORS_H

#define MD_BRAKE//ブレーキかける

#define DEG_M1 135
#define DEG_M2 -135
#define DEG_M3 -45
#define DEG_M4 45

#ifndef PI
    #define PI 3.14159265359
#endif

#include "mbed.h"
#include "Vector.h"
#include "TrigonometricTable.h"


class LGMotors {
public:

LGMotors(PinName _pha1,PinName _ena1,PinName _pha2,PinName _ena2,PinName _phb1,PinName _enb1,PinName _phb2,PinName _enb2,PinName _comSleep);
void runMotorIndex(float M_speed,PwmOut& PH,PwmOut& EN);
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


private:

//Motors
PwmOut PHA1;
PwmOut ENA1;

PwmOut PHA2;
PwmOut ENA2;

PwmOut PHB1;
PwmOut ENB1;

PwmOut PHB2;
PwmOut ENB2;



//Common motors
DigitalOut slp;


};

#endif
