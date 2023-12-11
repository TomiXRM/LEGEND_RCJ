#include "LGDrib.h"
#include "mbed.h"

LGDrib::LGDrib(PinName pin) : drib(pin){
        state(0); wait_ms(300);
        state(1); wait_ms(300);
        state(0); wait_ms(100);
}

void LGDrib::dribIRQ0r(){
        drib = 0;
        dribOnTime.attach_us(callback(this,&LGDrib::dribIRQright),1500);
}
void LGDrib::dribIRQ0l(){
        drib = 0;
        dribOnTime.attach_us(callback(this,&LGDrib::dribIRQleft),1500);
}
void LGDrib::dribIRQright(){
        drib = 1;
        dribOnTime.attach_us(callback(this,&LGDrib::dribIRQ0r),2000);
}
void LGDrib::dribIRQleft(){
        drib = 1;
        dribOnTime.attach_us(callback(this,&LGDrib::dribIRQ0l),1000);
}

void LGDrib::state(bool state){
        if(state == 1) {
                if(isDribOn == 0) {
                        dribIRQright();
                        isDribOn = 1;
                }
        }else{
                if(isDribOn == 1) {

                        dribIRQleft();
                        isDribOn = 0;
                }
        }
}
