#ifndef LGLINE_H
#define LGLINE_H
#include "mbed.h"
#include "Vector.h"



class LineManage {
public:
LineManage(PinName LF,PinName LR,PinName LB,PinName LL);
void EnableIRQ(uint16_t tm);
void DisableIRQ();
void LineCheck();
void SetLine(VectorXY *vector,VectorXY *IRv,VectorXY *Bound);



bool OutFlag;
bool front;
bool right;
bool back;
bool left;
short OnWH1st;
short OnWH2nd;
short OnWH3rd;
short OnWH4th;
bool Frised;
bool Rrised;
bool Brised;
bool Lrised;
short Raw;//0~8(0は踏んでない)
short LastRaw;
short order;
int T;
uint8_t wt;
private:
DigitalIn lineF;
DigitalIn lineR;
DigitalIn lineB;
DigitalIn lineL;

Ticker lineOP;
Timer line_wait;
};
#endif
