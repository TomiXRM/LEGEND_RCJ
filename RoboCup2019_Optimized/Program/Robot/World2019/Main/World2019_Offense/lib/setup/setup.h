#ifndef _SETUP_H_
#define _SETUP_H_

#include "mbed.h"
#include "LGUI.h"


#include "LGball.h"
#include "LGMotors.h"
#include "LGLineManage.h"
// #include "OpenMV.h"
#include "OpenMV_rev.h"
#include "PID_v1.h"
#include "Vector.h"
#include "Servo.h"
#include "LGIMU.h"
// #include "LGIMU_rev.h"
// #include "LGDotStar.h"

#include "SpeedProcess.hpp"
#include "BallProcess.hpp"
#include "CompassProcess.hpp"
#include "LineProcess.hpp"
#include "CameraProcess.hpp"
#include "UsProcess.hpp"



type_sensors sensors;
type_camera camera;

short TrackingDeg;
VectorXY DriveV;//ロボットの移動ベクトル
VectorXY IRv;//ボールのベクトル(距離なし)
VectorXY Bound;//ラインのベクトル

//-----------------------------------------Serial----------------------------------------
RawSerial pc(USBTX,USBRX);//PC,Ball
RawSerial Serial1(PA_9,PA_10);
// RawSerial Serial4(PA_0,PA_1);
RawSerial Serial5(PC_12,PD_2);
// RawSerial Serial6(PC_6,PC_7);

//------------------------------------------LINE------------------------------------------
DigitalIn LINE1(PC_13);
DigitalIn LINE2(PC_14);
DigitalIn LINE3(PC_15);
DigitalIn LINE4(PH_0);
//------------------------------------------AnalogPin-------------------------------------
AnalogIn CN1(PC_0);
AnalogIn Photo1(PC_1);
AnalogIn CN3(PC_2);
AnalogIn Photo2(PC_3);

// DigitalIn _imu_(PC_3);//方位センサ
// AnalogIn IMU(PC_3);//方位センサ

DigitalOut MotoSLP(PB_7);
LGIMU IMU(PC_12,PD_2);
// LGIMU IMU(&Serial5);
// 20201/7/5になぜか変なエラーがでて、OSエラーでてて、ピンの調査をしたけどわからず
// NOTE 20217/22 PinのNCをするとバグる
// LGDotStar DotStar(PB_15,PB_13,16);

LGUI ui(&pc,&MotoSLP,PC_10,PC_11);//UI
LGball IR(&pc,PB_2);//ボールセンサ


LineManage Line(PH_0,PC_13,PC_14,PC_15);//ラインセンサ
// LineManage Line(NC,NC,NC,NC);//ラインセンサ
// 20201/7/5になぜか変なエラーがでて、OSエラーでてて、ピンの調査をしたけどわからず
// NOTE 20217/22 PinのNCをするとバグる

// LineManage Line(PH_0,PC_13,PC_14,PC_15,PA_0,PA_1);//ラインセンサ　

// OpenMV cam(PA_10,PA_9,&sensors.dir);//カメラ
OpenMV cam(&Serial1,&sensors.dir);//カメラ
PID pidDir(&sensors.calDir, &sensors.turnDir,0, 1.4, 0, 0.04,DIRECT);//方向制御のPID
Motors MD(PB_5,PB_6,PB_3,PB_4,PB_7);//モーター
Timer directionOpsCtrl;//相手ゴールが見えなくなった時にコンパスの角度に使用変更するまでのディレイ


#endif // SETUP_H
