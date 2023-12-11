#ifndef _SETUP_H_
#define _SETUP_H_
//#include "BNO055.h"
//#include "MathTable.h"
#include "mbed.h"

#define TX D1
#define RX D0
#define ballrx D2
#define ballinter PC_14
#define usrx PC_11
#define ustx PC_10
#define usinter PA_15

#define INLINE inline __attribute__((always_inline))
#define PI 3.14159265358979323846264338327

#define WORK_MODE 'A'
#define US_MODE 'B'
#define WAIT_MODE 'X'

extern Serial pc(D1,NC);

/*extern DigitalOut led1(PB_13);
   extern DigitalOut led2(PB_2);
   extern DigitalOut led3(PB_15);
   extern DigitalOut led4(PB_14);

   extern AnalogIn CN1(PC_2);
   extern AnalogIn CN2(PC_3);
   extern AnalogIn CN3(PC_0);
   extern AnalogIn CN4(PB_1);

   extern AnalogIn LINE1(A0);
   extern AnalogIn LINE2(A1);
   extern AnalogIn LINE3(A2);
   extern AnalogIn LINE4(A3);

   extern InterruptIn SW(PC_13);
   extern DigitalOut buzzer(PH_1);
   extern DigitalIn solenoid(PC_6);*/


#endif // SETUP_H
