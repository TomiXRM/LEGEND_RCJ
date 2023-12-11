#include "mbed.h"
/*PwmOut M1(D12);
   DigitalOut CH(D11);
   Serial pc(USBTX,NC);
   int dir = 0;
   float pwm1 = 0;
   void move(int a){
        pwm1 = (sin((a) * 3.14159265358979323846264338327 / 180));
        if (pwm1 > 0) {
                M1 = abs(pwm1);
                CH = 1;
        } else if (pwm1 < 0) {
                M1 = abs(pwm1);
                CH = 0;

        } else {
                CH = 0;
                M1 = 0;
        }
   }

   int main() {
        M1.period(1.0/100000);
        while(1) {
                dir=dir + 1;
                move(dir);
                pc.printf("pwm is %lf\r\n",pwm1);
                wait_ms(10);

        }
   }*/
#include "FourMotors.h"
Motor MD(D8,D13,D7,D12,D6,D11,D5,D10);

int main(){

}
