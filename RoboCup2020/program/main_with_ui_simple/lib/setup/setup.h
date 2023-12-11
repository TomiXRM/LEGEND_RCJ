#ifndef _SETUP_H_
#define _SETUP_H_

#include "mbed.h"
#include "setup_com.h"
#include "pinDefs.h"

#include "LGball.h"
#include "LGIMU.h"
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


uint16_t dataThr[32]={   //checkThrで取った値をserialerからコピペで取ってきてそのまま貼り付ける。
        210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210,210
};


type_sensors sensors;
type_camera camera;
type_pid pidDir;



VectorXY DriveV;//ロボットの移動ベクトル
VectorXY IRv;//ボールのベクトル(距離なし)
VectorXY Bound;//ラインのベクトル
Ticker getDx;
Timer pidDt;
Timer moveBackTimer;

LGLCD LCD(LCD_SDA,LCD_SCL);
LGSW btn(BUTTON_CENTRE,BUTTON_LEFT,BUTTON_RIGHT,BUZZER);
RGB rgb(RGB_R,RGB_G,RGB_B);
// LGMotors MD(MD_B_PH2,MD_B_EN2,MD_B_PH1,MD_B_EN1,MD_A_PH2,MD_A_EN2,MD_A_PH1,MD_A_EN1,MD_SLP);
Motors MD(MD_B_EN1,MD_B_PH1,MD_B_EN2,MD_B_PH2,MD_SLP);
//-----------------------------------------Serial----------------------------------------
RawSerial pc(PC_TX,IR_RX);
LGIMU IMU(IMU_TX,IMU_RX);
LGball IR(&pc,IR_INT);//ボールセンサ
LGMouse mouse(MOUSE_RX);
OpenMV cam(CAM_RX,CAM_TX,&sensors.dir);//カメラ
//kicker
AnalogIn Photo1(KICKER_PHOTO1);
AnalogIn Photo2(KICKER_PHOTO2);
LGKicker Kicker(KICKER_KICK);
//------------------------------------------LINE------------------------------------------
LGAngelLine Line(LINE_A0,LINE_A1,LINE_A2,LINE_A3,LINE_A4,LINE_SIG, dataThr,&pc);
//------------------------------------------AnalogPin-------------------------------------
// PID pidDir(&sensors.calDir, &sensors.turnDir,0,0.9,0,0.095,DIRECT);//方向制御のPID
// PID pidDir(&sensors.calDir, &sensors.turnDir,0, 1.0, 0, 0.04,DIRECT);//方向制御のPID

uint8_t lastSw1State=0; // beforeやAfterを走らせるために直前のsw1Stateをとっている
uint8_t runningMode=0; //実行するモード。 sw1State==1の時にsw2(左),sw3(右)が押されてモードが変更されてしまうとまずいので

#endif // SETUP_H
