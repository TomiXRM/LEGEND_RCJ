#include "mbed.h"
#include "LGKicker.h"

LGKicker::LGKicker(PinName KickerPin,PinName photo) : Kicker(KickerPin),SW(photo),KickerIsRedey(),Kicker_timeout(),timer(){
        Kicker = 0;
        timer.reset();
        timer.start();
        Threshold = 0.7;
}

bool LGKicker::Kick(void){
        if(timer.read_ms() > 200) {//&& SW.read() < Threshold
                if(enabled == 0) {
                        KickerIsRedey.attach_us(mbed::callback(this, &LGKicker::flipOn), 1000);
                        enabled = 1;
                }
                //flipOn();
                return 1;
        }else{
                Kicker = 0;
                return 0;
        }
}

void LGKicker::flipOn(void){
        Kicker = 1;
        SW = 1;
        Kicker_timeout.attach_us(mbed::callback(this,&LGKicker::flip),50000);
}
void LGKicker::flip(void)  {
        Kicker = 0;
        SW = 0;
        timer.reset();
        enabled = 0;
}
short LGKicker::read(void){
        //return (short)(SW.read()*1000);
}

float LGKicker::SetThreshold(float i){
        //0~1000の閾値設定を入力
        return Threshold = (i/1000);
}
