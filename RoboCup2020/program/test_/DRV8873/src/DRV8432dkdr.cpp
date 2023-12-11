#include "mbed.h"
#define PI 3.141592653589
#define MD_BRAKE

Serial pc(USBTX,NC);

PwmOut PH(D2);
PwmOut EN(D3);
int n;
float motor;



void run(float M_speed){
        if(abs(M_speed)> 100) {
                if(M_speed > 0) {
                        M_speed = 100;
                }else{
                        M_speed = -100;
                }
        }
        // if(abs(M_speed) <= 100) {
        if(M_speed == 0) {
                #ifdef MD_BRAKE
                EN.write(1); PH.write(1);
                #else
                EN.write(0); PH.write(0);
                #endif
        }else{
                if(M_speed > 0) {
                        //正
                        EN.write(0);
                        PH.write(abs(M_speed)/100);
                }else{
                        //負
                        EN.write(abs(M_speed)/100);
                        PH.write(0);
                }
        }
        // }
}



int main() {
        pc.baud(230400);

        PH.period_us(50);
        EN.period_us(50);

//sp.write(0);

        while(1) {
                n++;
                motor = sin((n)*PI/180)*100;

                run(motor);
                pc.printf("n:%d\r\n",(int)motor);
                wait_ms(10);
        }
}


