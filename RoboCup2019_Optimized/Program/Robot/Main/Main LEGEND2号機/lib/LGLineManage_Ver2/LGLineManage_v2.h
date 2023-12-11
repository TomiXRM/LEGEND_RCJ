#ifndef _LGLINEMANAGE_V2_H_
#define _LGLINEMANAGE_V2_H_
#include "mbed.h"
#include "Vector.h"

#define _LINE_DEBUG_

#define FIRST_LINE_IS_NONE  lineWhiteNum[0] == 0
#define SECOND_LINE_IS_NONE lineWhiteNum[1] == 0
#define THIRD_LINE_IS_NONE  lineWhiteNum[2] == 0
#define FORTH_LINE_IS_NONE  lineWhiteNum[3] == 0
#define ON_THE_LINE lineStates[0]||lineStates[1]||lineStates[2]||lineStates[3]


class LineManage {
public:
LineManage(PinName LF,PinName LR,PinName LB,PinName LL,RawSerial *serial);
void LineCheck();
void SetLine(VectorXY *vector,VectorXY *irVector,VectorXY *lineVector,float *_dir,int8_t *_OwnX,int8_t *_OwnY,uint8_t _OwnW,int8_t *_OpsX,int8_t *_OpsY,uint8_t _OpsW);
DigitalIn lineF;        //前方
DigitalIn lineR;        //右方
DigitalIn lineB;        //後方
DigitalIn lineL;        //左方


uint8_t lineOrder;//順番
uint8_t lineWhiteNum[4];//白色に触れたラインセンサの順番の配列
bool lineStates[4];//0前,1右,2後,3左

uint16_t order;//踏んだ順番
uint8_t lineStateRaw16;
int16_t lineStateRaw360;
bool isOnLine;
uint8_t lineDepth;//ラインの深さ
uint8_t lineProgress;//白を踏んだラインセンサの順番を数えるやつの進行度(1,2,3,4回目の今どこか)
private:

RawSerial *pc;
Ticker lineRead;


};

#endif
