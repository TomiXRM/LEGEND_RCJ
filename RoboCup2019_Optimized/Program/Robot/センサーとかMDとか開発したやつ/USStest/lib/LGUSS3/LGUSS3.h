
#define LGUSS_H
#ifdef LGUSS_H


#include "mbed.h"

class LGUSS {

Timeout Measure;
public:
LGUSS(PinName trig);
void Start_U();
short Get();
short distance;  //距離を記憶する変数
private:
InterruptIn Echo;    //受信
DigitalInOut Trig;     //送信
Timer timer1;


void Stop_U();
void Rise_U();
void Fall_U();
};
#endif
