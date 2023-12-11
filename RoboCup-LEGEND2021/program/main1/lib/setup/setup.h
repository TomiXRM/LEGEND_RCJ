#ifndef _SETUP_H_
#define _SETUP_H_

#include "mbed.h"
#include "setup_com.h"
#include "pinDefs.h"

#include "LGball.h"
#include "LGIMU_V2.h"
#include "OpenMV.h"
// #include "LGMotors.h"
#include "LGMotors_DRV8432.h"
#include "LGMouse.h"
#include "LGAngelLine3.h"
// #include "LGAngelLine.h"
#include "PID_v1.h"
#include "Vector.h"

#include "LGLCD.h"
#include "LGSW_V2.h"
#include "LGModes.h"
#include "colors.h"
#include "RGB.h"
#include "LGKicker.h"
#include "LGDrib.h"


uint16_t dataThr[32]={   //checkThrで取った値をserialerからコピペで取ってきてそのまま貼り付ける。
        180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180,180
};


type_sensors sensors;
type_camera camera;
type_pid pidDir;


VectorXY DriveV;//ロボットの移動ベクトル
VectorXY IRv;//ボールのベクトル(距離なし)
VectorXY Bound;//ラインのベクトル
VectorXY MouseV;//ラインのベクトル
Ticker getDx;
Timer pidDt;


LGSW btn(BUTTON_CENTRE,BUTTON_LEFT,BUTTON_RIGHT,BUZZER);
RGB rgb(RGB_R,RGB_G,RGB_B);
I2C i2c(I2C_SDA,I2C_SCL);
LGLCD LCD(i2c);
BNO055 IMU(&i2c);
// LGMotors MD(MD_B_PH2,MD_B_EN2,MD_B_PH1,MD_B_EN1,MD_A_PH2,MD_A_EN2,MD_A_PH1,MD_A_EN1,MD_SLP);
Motors MD(MD_1,MD_2,MD_3,MD_4,MD_SLP);
//-----------------------------------------Serial----------------------------------------
RawSerial pc(PC_TX,IR_RX);
RawSerial Serial6(UART_6_TX,UART_6_RX);
// LGIMU IMU(IMU_TX,IMU_RX);
LGball IR(&pc,NC);//ボールセンサ
LGMouse mouse(&Serial6);
OpenMV cam(CAM_RX,CAM_TX,&sensors.dir);//カメラ
//kicker
AnalogIn Photo1(KICKER_PHOTO1);
AnalogIn Photo2(KICKER_PHOTO2);
LGKicker Kicker(KICKER_KICK);
//Dribler
// LGDrib drib(DRIB_PWM);

AnalogIn BK(CN4);
//------------------------------------------LINE------------------------------------------
LGAngelLine Line(LINE_A0,LINE_A1,LINE_A2,LINE_A3,LINE_A4,LINE_SIG, dataThr,&pc);
//------------------------------------------AnalogPin-------------------------------------
// PID pidDir(&sensors.calDir, &sensors.turnDir,0,0.9,0,0.095,DIRECT);//方向制御のPID
// PID pidDir(&sensors.calDir, &sensors.turnDir,0, 1.0, 0, 0.04,DIRECT);//方向制御のPID

uint8_t lastSw1State=0; // beforeやAfterを走らせるために直前のsw1Stateをとっている
uint8_t runningMode=0; //実行するモード。 sw1State==1の時にsw2(左),sw3(右)が押されてモードが変更されてしまうとまずいので
bool imuDirEnable=1;//タイマー割り込みで姿勢制御するやつを解除したりするため


#endif // SETUP_H
