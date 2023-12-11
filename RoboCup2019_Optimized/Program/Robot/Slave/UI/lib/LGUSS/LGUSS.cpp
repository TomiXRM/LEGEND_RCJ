
#include "mbed.h"
#include "LGUSS.h"

LGUSS::LGUSS(PinName trig,PinName echo) : Trig(trig),Echo(echo)
{
        timer1.start();
        Echo.mode(PullDown);
        Echo.rise(this,&LGUSS::Rise_U);
        Echo.fall(this,&LGUSS::Fall_U);
        disable_frag = 0;
        Start_U();
}

void LGUSS::Start_U(){
        Trig = 1;
        Measure.attach_us(mbed::callback(this,&LGUSS::Stop_U),10);
}

void LGUSS::Stop_U(){
        Trig = 0;
}

void LGUSS::Rise_U(){
        timer1.start();
        timer1.reset();
}
void LGUSS::disable() {
        disable_frag = 1;
}

void LGUSS::enable(){
        timer1.start();
        Echo.mode(PullDown);
        Echo.rise(this,&LGUSS::Rise_U);
        Echo.fall(this,&LGUSS::Fall_U);
        disable_frag = 0;
        Start_U();
}

void LGUSS::Fall_U(){
        distance = ((timer1.read_us()*343)/20000);
        if(disable_frag == 1) {
                Trig=1;
                timer1.stop();
                timer1.reset();
                Echo.rise(NULL);//割り込み禁止
                Echo.fall(NULL);//割り込み禁止
                distance = -1;
        }else{
                Measure.attach_us(mbed::callback(this,&LGUSS::Start_U),5);
        }
}

short LGUSS::Get(){
        return distance;
}
