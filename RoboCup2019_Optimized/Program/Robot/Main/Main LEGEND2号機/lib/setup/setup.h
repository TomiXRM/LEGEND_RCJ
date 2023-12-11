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
        float camDir;
        float calDir;//カメラとコンパス、使う角度によって入る角度データが変わります。
        uint16_t DistB;//PSD
        uint16_t PhotoSensor;
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
// RawSerial UI(PC_10,PC_11);
RawSerial Serial4(NC,PA_1);
RawSerial Serial5(NC,PD_2);
RawSerial Serial6(NC,PC_7);

DigitalOut Serial4Call(PH_0);
DigitalOut Serial5Call(PC_12);
DigitalOut Serial6Call(PB_9);
//------------------------------------------LINE------------------------------------------
DigitalIn LINE1(PC_9);
DigitalIn LINE2(PB_8);
DigitalIn LINE3(PC_8);
DigitalIn LINE4(PC_6);
//------------------------------------------AnalogPin-------------------------------------
AnalogIn CN1(D13);  //PA5
AnalogIn CN2(D12);  //PA6
AnalogIn CN3(D11);  //PA7
AnalogIn CN4(PB_1);

AnalogIn Photo(PC_2);

DigitalIn _imu_(PC_3);//方位センサ
AnalogIn IMU(PC_3);//方位センサ

DigitalOut MotoSLP(PA_15);
DigitalIn _MotoSLP(PA_15);

LGUI ui(&pc,&MotoSLP,PC_10,PC_11);//UI
LGball IR(&pc,PC_14);//ボールセンサ
LineManage Line(PC_9,PB_8,PC_8,PC_6);//ラインセンサ
OpenMV cam(PB_7,PC_5,&sensors.dir);//カメラ
PID pidDir(&sensors.dir, &sensors.turnDir,0, 1.4, 0, 0.04,DIRECT);//方向制御のPID
PID pidCamDir(&sensors.camDir, &sensors.turnDir,0, 3, 0, 0.02,DIRECT);//方向制御のPID
Motors MD(PB_4,PB_5,PB_3,PA_10,PA_15);//モーター

Timer directionOpsCtrl;//相手ゴールが見えなくなった時にコンパスの角度に使用変更するまでのディレイ
//SPI Bridge(PB_15,PB_14,PB_13);
//DigitalOut SS(PB_2);


#endif // SETUP_H
