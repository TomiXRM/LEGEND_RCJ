#include "mbed.h"
#include "LGKicker.h"

LGKicker::LGKicker(PinName KickerPin) : Kicker(KickerPin),KickerIsRedey(),Kicker_timeout(),timer(){
        Kicker = 0;
        timer.reset();
        timer.start();
        Threshold = 0.7;
}

bool LGKicker::Kick(void){
        if(timer.read_ms() > 200) {//&& SW.read() < Threshold
                if(enabled == 0) {
                        KickerIsRedey.attach_us(mbed::callback(this, &LGKicker::flipOn), 2000);
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
        Kicker_timeout.attach_us(mbed::callback(this,&LGKicker::flip),50000);
}
void LGKicker::flip(void)  {
        Kicker = 0;
        timer.reset();
        enabled = 0;
}