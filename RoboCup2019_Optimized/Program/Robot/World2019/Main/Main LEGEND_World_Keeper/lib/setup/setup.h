#ifndef _SETUP_H_
#define _SETUP_H_

#include "LGUI.h"
#include "mbed.h"

#include "LGESP.h"
#include "LGIMU.h"
#include "LGLineManage.h"
#include "LGMotors.h"
#include "LGball.h"
#include "OpenMV.h"
#include "PID_v1.h"
#include "US_Improved.h"
#include "Vector.h"

#include "BallProcess.hpp"
#include "CameraProcess.hpp"
#include "CompassProcess.hpp"
#include "LineProcess.hpp"
#include "SpeedProcess.hpp"

#include "UsProcess.hpp"
#include "setup_com.h"

#include "config.h"
type_sensors sensors;
type_camera camera;

short TrackingDeg;
short BackingToHomeDegree;
bool canKick;
VectorXY DriveV; //ロボットの移動ベクトル
VectorXY IRv;    //ボールのベクトル(距離なし)
VectorXY IRvD;
VectorXY Bound; //ラインのベクトル

//-----------------------------------------Serial----------------------------------------
RawSerial pc(USBTX, PA_3); // PC,Ball
// RawSerial UI(PC_10,PC_11);

RawSerial Serial6(NC, PC_7);

DigitalOut Serial6Call(PB_9);
//------------------------------------------LINE------------------------------------------
DigitalOut LINE1(PC_9);
DigitalIn LINE2(PB_8);
DigitalIn LINE3(PC_8);
DigitalIn LINE4(PC_6);
//------------------------------------------AnalogPin-------------------------------------
AnalogIn PSDlb(PA_5); // CN1,PA_5
AnalogIn PSDl(PB_1);  // CN2,PA6

AnalogIn PSDrb(PA_0);
AnalogIn PSDr(PC_1);

AnalogIn Photo(PC_2);

LGIMU IMU(PH_0, PA_1); // Serial5

DigitalOut MotoSLP(PA_15);
DigitalIn _MotoSLP(PA_15);

LGUI ui(&pc, &MotoSLP, PC_10, PC_11);    // UI
LGball IR(&pc, PC_14);                   //ボールセンサ
LineManage Line(PC_9, PB_8, PC_8, PC_6); //ラインセンサ
OpenMV cam(PB_7, PC_5, &sensors.dir);    //カメラ
PID pidDir(&sensors.dir, &sensors.turnDir, 0, 1.4, 0.1, 0.04,
           DIRECT); //方向制御のPID
PID pidCamDir(&sensors.camDir, &sensors.turnDir, 0, 3, 0.1, 0.02,
              DIRECT); //方向制御のPID

Motors MD(PB_4, PB_5, PB_3, PA_10, PA_15); //モーター

Timer
        directionOpsCtrl; //相手ゴールが見えなくなった時にコンパスの角度に使用変更するまでのディレイ
Timer KickerCtrl;     //キッカー発動までのディレイ

DigitalOut kicker(PB_2);
LGUS USBack(PA_7, PA_6);
LGESP esp(PC_12, PD_2, &sensors, &camera, &pc);

#endif // SETUP_H
