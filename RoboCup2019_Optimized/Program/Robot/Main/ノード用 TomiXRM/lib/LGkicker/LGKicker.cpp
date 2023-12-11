#include "mbed.h"
#include "LGKicker.h"

LGKicker::LGKicker(PinName KickerPin,PinName photo) : Kicker(KickerPin),SW(photo),KickerIsRedey(),Kicker_timeout(),timer(){
        Kicker = 0;
        timer.reset();
        timer.start();
        Threshold = 0.7;
}

bool LGKicker::Kick(void){
        if(timer.read_ms() > 200 && SW.read() < Threshold) {
                KickerIsRedey.attach(mbed::callback(this, &LGKicker::flipOn), 15);
                return 1;
        }else{
                Kicker = 0;
                return 0;
        }
}
void LGKicker::flipOn(void){
        Kicker = 1;
        Kicker_timeout.attach(mbed::callback(this,&LGKicker::flip),0.02);
        timer.reset();
}
void LGKicker::flip(void)  {
        Kicker = 0;
}
short LGKicker::read(void){
        return (short)(SW.read()*1000);
}

float LGKicker::SetThreshold(float i){
        //0~1000の閾値設定を入力
        return Threshold = (i/1000);
}
