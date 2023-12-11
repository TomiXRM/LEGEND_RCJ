#include "mbed.h"


//閾値は自分で合わせましょう！
#define ThresholdF1 450       //前

#define ThresholdR1 450       //右1
#define ThresholdR2 450       //右2

#define ThresholdB1 450       //後ろ1
#define ThresholdB2 450       //後ろ2

#define ThresholdL1 450       //左1
#define ThresholdL2 450       //左2

#define PI 3.14159265358979323846264338327

int a;


AnalogIn F1(PA_0);
AnalogIn R1(PA_1);
AnalogIn R2(PA_3);
AnalogIn B1(PA_4);
AnalogIn B2(PA_5);
AnalogIn L1(PA_6);
AnalogIn L2(PA_7);

DigitalIn f1(PA_0);
DigitalIn r1(PA_1);
DigitalIn r2(PA_3);
DigitalIn b1(PA_4);
DigitalIn b2(PA_5);
DigitalIn l1(PA_6);
DigitalIn l2(PA_7);

Serial PC(USBTX,NC);

DigitalOut LINE1(PB_4);
DigitalOut LINE2(PB_5);
DigitalOut LINE3(PB_6);
DigitalOut LINE4(PB_7);

DigitalOut LED(D13);
Ticker living;

PwmOut NJW4617(PA_12);

uint16_t line[7];
bool OnWhite[7];

void Blink(){
        LED = !LED;
}

int main() {
        PC.baud(230400);
        living.attach(&Blink,0.6);
        f1.mode(PullDown);
        r1.mode(PullDown);
        r2.mode(PullDown);
        b1.mode(PullDown);
        b2.mode(PullDown);
        l1.mode(PullDown);
        l2.mode(PullDown);


        /*NJW4617.period_us(15);
           for (size_t i = 0; i < 7; i++) {
                line[i] = 0;
                OnWhite[i] = 0;
           }
           while(1) {
                for (float i = 0; i < 1; i=i+0.01) {
                        NJW4617 = i;
                        wait_ms(3);
                }
                for (float i = 1; i > 0; i=i-0.01) {
                        NJW4617 = i;
                        wait_ms(3);
                }
                //NJW4617 = 0.3;
                wait_ms(10);*/
        while (1) {
                OnWhite[0] = 0;
                OnWhite[1] = 0;
                OnWhite[2] = 0;
                OnWhite[3] = 0;
                OnWhite[4] = 0;
                OnWhite[5] = 0;
                OnWhite[6] = 0;

                line[0] = F1.read()*1000;
                line[1] = R1.read()*1000;
                line[2] = R2.read()*1000;
                line[3] = B1.read()*1000;
                line[4] = B2.read()*1000;
                line[5] = L1.read()*1000;
                line[6] = L2.read()*1000;

                if(line[0] > ThresholdF1) OnWhite[0] = 1;
                if(line[1] > ThresholdR1) OnWhite[1] = 1;
                if(line[2] > ThresholdR2) OnWhite[2] = 1;
                if(line[3] > ThresholdB1) OnWhite[3] = 1;
                if(line[4] > ThresholdB2) OnWhite[4] = 1;
                if(line[5] > ThresholdL1) OnWhite[5] = 1;
                if(line[6] > ThresholdL2) OnWhite[6] = 1;

                if(OnWhite[0]) { LINE1 = 1;}else{LINE1 = 0;}
                if(OnWhite[1] || OnWhite[2]) {LINE2 = 1;}else{LINE2 = 0;}
                if(OnWhite[3] || OnWhite[4]) {LINE3 = 1;}else{LINE3 = 0;}
                if(OnWhite[5] || OnWhite[6]) {LINE4 = 1;}else{LINE4 = 0;}
                PC.printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n",line[0],line[1],line[2],line[3],line[4],line[5],line[6]); //*/
        }
}
