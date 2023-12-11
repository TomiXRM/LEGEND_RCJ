#ifndef _SETUP_H_
#define _SETUP_H_

#include "mbed.h"
#include "LGUI.h"

#include "LGball.h"
#include "LGMotors.h"
#include "LGLineManage.h"
#include "LGIMU.h"
#include "LGUSS.h"
#include "OpenMV.h"
#include "PID_v1.h"
#include "Vector.h"

#include "SpeedProcess.hpp"
#include "BallProcess.hpp"
#include "CompassProcess.hpp"
#include "LineProcess.hpp"
#include "OwnGoalProcess.hpp"
#include "CameraProcess.hpp"
#include "UsProcess.hpp"

struct {
        int16_t dir;          //方位センサ
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
        short EchoRight;
        short EchoLeft;
        uint16_t PhotoSensorFront;  //フォトセンサー前
        uint16_t PhotoSensorBack; //フォトセンサー後
        uint16_t DistB;
        uint8_t velocity;
} sensors;

struct {
        int8_t OwnX;
        int8_t OwnY;
        short X;//コート上の座標
        short Y;//コート上の座標
        short OwnDir;
        short OwnDist;
        short OwnGoalRightEnd;     //自ゴールのゴールの右端
        short OwnGoalLeftEnd;      //自ゴールのゴールの左端
        uint8_t OwnW;              //自ゴールの幅
        short OpsDir;
        short OpsDist;
        short OpsGoalRightEnd;     //相手ゴールのゴールの右端
        short OpsGoalLeftEnd;      //相手ゴールのゴールの左端
        uint8_t OpsW;
        bool useCamDirection;//使う角度はカメラかコンパスか(1:カメラ.0:コンパス)
} camera;

short TrackingDeg;
VectorXY DriveV;//ロボットの移動ベクトル
VectorXY IRv;//ボールのベクトル(距離なし)
VectorXY Bound;//ラインのベクトル

//-----------------------------------------Serial----------------------------------------
RawSerial pc(USBTX,PA_3);//PC,Ball
// RawSerial Serial4(PA_0,PA_1);
// RawSerial Serial5(PC_12,PD_2);
// RawSerial Serial6(PC_6,PC_7);

//------------------------------------------LINE------------------------------------------
DigitalIn LINE1(PC_13);
DigitalIn LINE2(PC_14);
DigitalIn LINE3(PC_15);
DigitalIn LINE4(PH_0);
//------------------------------------------AnalogPin-------------------------------------
AnalogIn CN1(PC_0);
AnalogIn CN2(PC_1);//フォトセンサー
AnalogIn CN3(PC_2);//フォトセンサー
AnalogIn CN4(PC_3);

//DigitalIn _imu_(PC_3);//方位センサ
//AnalogIn IMU(PC_3);//方位センサ

DigitalOut MotoSLP(PB_7);

LGIMU IMU(PA_0,PA_1);
LGUI ui(&pc,&MotoSLP,PC_10,PC_11);//UI
LGball IR(&pc,PB_2);//ボールセンサ
LineManage Line(PC_13,PC_14,PC_15,PH_0);//ラインセンサ
LGUSS US_R(PA_4,PA_5);
LGUSS US_L(PA_6,PA_7);
OpenMV cam(PA_10,PA_9,&sensors.dir);//カメラ
PID pidDir(&sensors.calDir, &sensors.turnDir,0, 1.4, 0, 0.07,DIRECT);//方向制御のPID
Motors MD(PB_5,PB_6,PB_3,PB_7);//モーター



#endif // SETUP_H
