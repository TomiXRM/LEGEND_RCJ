#ifndef _ANGELLINE_H_
#define _ANGELLINE_H_

//------------------------------------------------------------------------------
//三角関数表
#define sin0_0    0
#define cos0_0    1
#define sin22_5   0.3827
#define cos22_5   0.9239
#define sin45_0   0.7071
#define cos45_0   0.7071
#define sin67_5   0.9239
#define cos67_5   0.3827
#define sin90_0   1
#define cos90_0   0
#define sin112_5  0.9239
#define cos112_5 -0.3827
#define sin135_0  0.7071
#define cos135_0  0.7071
#define sin157_5  0.3827
#define cos157_5 -0.9239
#define sin180_0  0
#define cos180_0  -1

#define sin_22_5  -0.3827
#define cos_22_5   0.9239
#define sin_45_0  -0.7071
#define cos_45_0   0.7071
#define sin_67_5  -0.9239
#define cos_67_5   0.3827
#define sin_90_0  -1
#define cos_90_0   0
#define sin_112_5 -0.9239
#define cos_112_5 -0.3827
#define sin_135_0 -0.7071
#define cos_135_0 -0.7071
#define sin_157_5 -0.3827
#define cos_157_5 -0.9239


#include "mbed.h"

class AngelLine {
public:
AngelLine(PinName _tx,PinName _rx,RawSerial *_pc);
void callback_read();//割り込み受信
int16_t lineStates;
const int16_t lineX[16] = {

};
const int16_t lineY[16] = {
        sin0_0*1000,sin22_5*1000,sin45_0*1000,sin67_5*1000,sin90_0*1000,sin_112_5*1000,sin135_0*1000,sin157_5*1000,sin180_0*1000,sin_157_5*1000,sin_135_0*1000,sin_112_5*1000,sin_90_0*1000,sin_67_5*1000,sin_45_0*1000,sin_22_5*1000
};
private:

};

#endif
