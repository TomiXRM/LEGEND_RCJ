#ifndef LGLINE_H
#define LGLINE_H
#include "mbed.h"
#include "Vector.h"

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define INLINE inline __attribute__((always_inline))

class LineManage {
public:
LineManage(PinName LF,PinName LR,PinName LB,PinName LL);
LineManage(PinName LF,PinName LR,PinName LB,PinName LL,PinName tx,PinName rx);
void EnableIRQ(uint16_t tm);
void DisableIRQ();
void LineCheck();
void SetLine(VectorXY *vector,VectorXY *IRv,VectorXY *Bound);
INLINE void serialLineGet();


bool front;//ラインの状態
bool right;//ラインの状態
bool back;//ラインの状態
bool left;//ラインの状態

bool front1;//ラインの状態
bool right1;//ラインの状態
bool back1;//ラインの状態
bool left1;//ラインの状態

bool front2;//ラインの状態
bool right2;//ラインの状態
bool back2;//ラインの状態
bool left2;//ラインの状態

short OnWH1st;//ラインを1番に踏んだセンサ番号
short OnWH2nd;//ラインを2番に踏んだセンサ番号
short OnWH3rd;//ラインを3番に踏んだセンサ番号
short OnWH4th;//ラインを4番に踏んだセンサ番号
bool Frised;//前方のラインセンサが反応したことあるかどうか
bool Rrised;//右方のラインセンサが反応したことあるかどうか
bool Brised;//後方のラインセンサが反応したことあるかどうか
bool Lrised;//左方のラインセンサが反応したことあるかどうか
short Raw;//ラインの踏んでいる角度の生情報
short Raw_16;////ラインの踏んでいるセンサーを16の整数で表したもの
short LastRaw;//ラインの踏んでいた角度
uint8_t LastWhiteSensor;//最後白に触れていたセンサ
short order;////ラインを踏んだセンサの順番
bool OutFlag;//ラインから出てしまったかどうか
int T;
uint8_t wt;

bool readformat;

uint8_t linestate;
private:
DigitalIn lineF;//前方
DigitalIn lineR;//右方
DigitalIn lineB;//後方
DigitalIn lineL;//左方


Ticker lineOP;//タイマー割り込み
Timer line_wait;
Timer Out_wait;
Serial serial;
};
#endif
