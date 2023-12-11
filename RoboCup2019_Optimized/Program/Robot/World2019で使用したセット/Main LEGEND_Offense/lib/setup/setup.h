#ifndef _SETUP_H_
#define _SETUP_H_

#include "mbed.h"
#include "LGUI.h"


#include "LGball.h"
#include "LGMotors.h"
#include "LGLineManage.h"
#include "OpenMV.h"
#include "PID_v1.h"
#include "Vector.h"
#include "Servo.h"
#include "LGIMU.h"

#include "SpeedProcess.hpp"
#include "BallProcess.hpp"
#include "CompassProcess.hpp"
#include "LineProcess.hpp"
#include "CameraProcess.hpp"
#include "UsProcess.hpp"


struct {
        float dir;          //方位センサ
        float turnDir;      //モーター用の方位センサ変数
        short irRecieved;   //受信したデータ
        short irDeg;        //ボールの角度
        short irDegTweak;   //ボールの角度をいじったやつ
        short irDegLast;    //前のボールの角度
        bool irDegSame;     //ボールの方向が前回と同じかどうかのフラグ
        uint8_t irDist;     //ボールの距離
        uint8_t irDistLast; //ボールの距離
        bool irDistSame;        //ボールの方向が前回と同じかどうかのフラグ
        float calDir;//カメラとコンパス、使う角度によって入る角度データが変わります。
        uint8_t velocity;//ロボットの速度
        uint16_t PhotoSensor1;
        uint16_t PhotoSensor2;
} sensors;

struct {
        int8_t OwnX;
        int8_t OwnY;
        int8_t OpsX;
        int8_t OpsY;
        short X;//コート上の座標
        short Y;//コート上の座標
        short OpsDir;
        short OwnDir;
        short OwnDist;
        short OpsDist;
        short OwnGoalRightEnd;     //自ゴールのゴールの右端
        short OwnGoalLeftEnd;      //自ゴールのゴールの左端
        short OpsGoalRightEnd;     //相手ゴールのゴールの右端
        short OpsGoalLeftEnd;      //相手ゴールのゴールの左端
        uint8_t OwnW;               //相手ゴールの幅
        uint8_t OpsW;               //自ゴールの幅
        bool useCamDirection;//使う角度はカメラかコンパスか(1:カメラ.0:コンパス)
} camera;

short TrackingDeg;
VectorXY DriveV;//ロボットの移動ベクトル
VectorXY IRv;//ボールのベクトル(距離なし)
VectorXY Bound;//ラインのベクトル

//-----------------------------------------Serial----------------------------------------
RawSerial pc(USBTX,PA_3);//PC,Ball
//
// RawSerial Serial4(PA_0,PA_1);
// RawSerial Serial5(PC_12,PD_2);
// RawSerial Serial6(PC_6,PC_7);

//------------------------------------------LINE------------------------------------------
DigitalOut LINE1(PC_13);
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

Servo DRIB(PB_1);
LGIMU IMU(PA_0,PA_1);

LGUI ui(&pc,&MotoSLP,PC_10,PC_11);//UI
LGball IR(&pc,PB_2);//ボールセンサ
LineManage Line(PH_0,PC_13,PC_14,PC_15);//ラインセンサ
OpenMV cam(PA_10,PA_9,&sensors.dir);//カメラ
PID pidDir(&sensors.calDir, &sensors.turnDir,0, 1.4, 0, 0.04,DIRECT);//方向制御のPID
Motors MD(PB_5,PB_6,PB_3,PB_4,PB_7);//モーター

Timer directionOpsCtrl;//相手ゴールが見えなくなった時にコンパスの角度に使用変更するまでのディレイ


#endif // SETUP_H
