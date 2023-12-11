
#define LGUSS_H
#ifdef LGUSS_H


#include "mbed.h"

class LGUSS {

Timeout Measure;
public:
LGUSS(PinName trig,PinName echo);
void Start_U();
short Get();
void enable();
void disable();
short distance;  //距離を記憶する変数
private:
InterruptIn Echo;    //受信
DigitalOut Trig;     //送信
Timer timer1;
bool disable_frag;


void Stop_U();
void Rise_U();
void Fall_U();
};
#endif
