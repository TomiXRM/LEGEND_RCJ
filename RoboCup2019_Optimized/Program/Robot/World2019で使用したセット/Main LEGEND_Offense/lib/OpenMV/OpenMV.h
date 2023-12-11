#ifndef _OPENMV_H_
#define _OPENMV_H_

#include "mbed.h"
#define PI 3.14159265358979323846264338327




class OpenMV {
public:
OpenMV(PinName _RX,PinName _call,float* _dir);
void baud(int b);
void get();
//本当に見えているか見えていないかだけでチェックする方法
void recognizeCheck();
//ワイドを見て閾値以下の場合は見えていないことにするフィルタリングをする方法
void recognizeCheck(uint8_t Own_Wide_Threshold,uint8_t Ops_Wide_Threshold);
short calDegree(bool q);

//
int8_t OwnX;               //自ゴール
int8_t OwnY;
int8_t OpsX;               //相手ゴール
int8_t OpsY;
//追加↓
uint8_t OwnW;              //相手ゴールの幅
uint8_t OpsW;              //自ゴールの幅
short OwnGoalRightEnd;     //自ゴールのゴールの右端
short OwnGoalLeftEnd;      //自ゴールのゴールの左端
short OpsGoalRightEnd;     //相手ゴールのゴールの右端
short OpsGoalLeftEnd;      //相手ゴールのゴールの左端

uint16_t data;
uint8_t highbyte;
uint8_t lowbyte;           //データの上位と下位(0)
char head;

bool OwnRecognize;         //認識できたかどうか
bool OpsRecognize;         //認識できたかどうか

int8_t PointOwnX[5];       //中立点用の変数(自陣X)
int8_t PointOwnY[5];       //中立点用の変数(自陣Y)
int8_t PointOpsX[5];       //中立点用の変数(敵陣X)
int8_t PointOpsY[5];       //中立点用の変数(敵陣Y)


RawSerial device;
DigitalOut call;



private:
void receive();
void receiveXY();
void receiveXYW();

uint8_t data_length;    //データの長さ
//計算用
int8_t _OwnX;                //自ゴール
int8_t _OwnY;
int8_t _OpsX;                //相手ゴール
int8_t _OpsY;
//追加↓
uint8_t _OwnW;                //相手ゴールの幅
uint8_t _OpsW;                //自ゴールの幅
int8_t _OwnGoalRightEnd;     //自ゴールのゴールの右端
int8_t _OwnGoalLeftEnd;      //自ゴールのゴールの左端
int8_t _OpsGoalRightEnd;     //相手ゴールのゴールの右端
int8_t _OpsGoalLeftEnd;      //相手ゴールのゴールの左端



float* dir;                                    //方位センサの角度データ(ポインタ)
};

#endif
