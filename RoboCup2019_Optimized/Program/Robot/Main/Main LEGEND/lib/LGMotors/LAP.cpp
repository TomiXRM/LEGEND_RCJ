#include "mbed.h"
#include "LGMotors.h"


#define PI 3.14159265358979323846264338327


Serial pc(USBTX,USBRX);

Motors motor(PB_4,PB_5,PB_3,PA_10,PB_9);

//PWM1
float m;//
int16_t s;
uint16_t i;

//PWM2
float n;
int16_t q;
int a;

int main() {
        motor.stopAll();
        wait(1);

        pc.baud(9600);
        motor.period_us(50);

        motor.Deg_m1=150;
        motor.Deg_m2=-135;
        motor.Deg_m3=-45;
        motor.Deg_m4=30;

        while(1) {
                i++;
                motor.drive(i,100,a);

                wait_ms(10);

                pc.printf("%d\r\n",i);

        }
}
