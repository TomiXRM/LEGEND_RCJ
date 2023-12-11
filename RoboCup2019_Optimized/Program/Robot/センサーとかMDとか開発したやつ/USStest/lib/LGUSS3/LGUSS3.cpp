
#include "mbed.h"
#include "LGUSS3.h"

LGUSS::LGUSS(PinName trig) : Trig(trig),Echo(trig)
{
        timer1.start();
        Echo.mode(PullDown);
        Echo.rise(this,&LGUSS::Rise_U);
        Echo.fall(this,&LGUSS::Fall_U);

        Start_U();
}

void LGUSS::Start_U(){
        Trig.output();
        Trig = 1;
        Measure.attach_us(mbed::callback(this,&LGUSS::Stop_U),15);
}

void LGUSS::Stop_U(){
        Trig = 0;
        Trig.input();
}

void LGUSS::Rise_U(){
        timer1.start();
        timer1.reset();
}

void LGUSS::Fall_U(){
        distance = ((timer1.read_us()*343)/20000);
        Measure.attach_us(mbed::callback(this,&LGUSS::Start_U),5);
}

short LGUSS::Get(){
        return distance;
}
