#include "mbed.h"
#include "LGKicker.h"

LGKicker::LGKicker(PinName KickerPin) : Kicker(KickerPin),KickerIsRedey(),Kicker_timeout(),timer(){
        Kicker = 0;
        timer.reset();
        timer.start();
        //   Kicker.period_us(50000);
        time = 50000;
        Threshold = 0.7;
        power = 1.0;
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
        Kicker = power;
        Kicker_timeout.attach_us(mbed::callback(this,&LGKicker::flip),time);//50000
}
void LGKicker::flip(void)  {
        Kicker = 0;
        timer.reset();
        enabled = 0;
}

void LGKicker::setPower(uint16_t p){
        if(p > 60000) p = 50000;
        time = p;
}

void LGKicker::setPower(){
        time = 50000;
}

