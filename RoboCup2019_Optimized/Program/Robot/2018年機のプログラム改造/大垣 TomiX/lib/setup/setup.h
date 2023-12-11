#ifndef _SETUP_H_
#define _SETUP_H_
//#include "BNO055.h"
//#include "MathTable.h"
#include "mbed.h"
#include "LGCAM.h"
//#include "LGUS.h"
#include "LGball.h"
#include "FourMotors.h"

#define BALL_RX PD_2
#define BALL_INT PA_12
#define IMU_RX PB_7
#define IMU_INT PC_15

#define INLINE inline __attribute__((always_inline))
#define PI 3.14159265358979323846264338327

#define LINENONE 0
#define LINEFRONT 1
#define LINERIGHT 2
#define LINEBACK 3
#define LINELEFT 4

#define IS_THE_LINE_ON LINE1.read() == 1 &&LINE2.read() == 1&&LINE3.read() == 1&&LINE4.read() == 1


extern Motor MD(D8,D13,D6,D11,D7,D12,D5,D10);

extern LGball Ball(BALL_RX, BALL_INT);
extern LGCAM IMU(IMU_RX, IMU_INT);

extern Serial pc(D1,NC);

extern DigitalOut led1(PB_2);
extern DigitalOut led2(PB_15);
extern DigitalOut led3(PC_8);
extern DigitalOut led4(PB_13);

/*extern AnalogIn CN1(PC_2);
   extern AnalogIn CN2(PC_3);
   extern AnalogIn CN3(PC_0);
   extern AnalogIn CN4(PB_1);*/

extern AnalogIn US1(A0);
extern AnalogIn US2(A1);
extern AnalogIn US3(A2);
extern AnalogIn US4(A3);

/*extern AnalogIn LINE1(A0);
   extern AnalogIn LINE2(A1);
   extern AnalogIn LINE3(A2);
   extern AnalogIn LINE4(A3);*/

extern DigitalIn LINE1(D2);//front
extern DigitalIn LINE2(D4);//right
extern DigitalIn LINE3(PC_4);//back
extern DigitalIn LINE4(PB_14);//left

extern DigitalIn SW(PC_9);

extern DigitalOut solenoid(PC_6);
extern AnalogIn Photo(A4);





#endif // SETUP_H
