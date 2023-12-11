#include "mbed.h"


//閾値は自分で合わせましょう！
 #define ThresholdF1 400       //前
 #define ThresholdF2 400       //前

 #define ThresholdR1 400       //右1
 #define ThresholdR2 400       //右2

 #define ThresholdB1 470       //後ろ1
 #define ThresholdB2 400       //後ろ2

 #define ThresholdL1 400       //左1
 #define ThresholdL2 400       //左2


 #define bitRead(value, bit) (((value) >> (bit)) & 0x01)
 #define bitSet(value, bit) ((value) |= (1UL << (bit)))
 #define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
 #define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

// #define PI 3.14159265358979323846264338327

int a;







AnalogIn F1(PA_0);
AnalogIn F2(PA_2);
AnalogIn R1(PA_6);
AnalogIn R2(PA_7);
AnalogIn B1(PA_4);
AnalogIn B2(PA_5);
AnalogIn L1(PA_1);
AnalogIn L2(PA_3);

DigitalIn f1(PA_0);
DigitalIn f2(PA_2);
DigitalIn r1(PA_6);
DigitalIn r2(PA_7);
DigitalIn b1(PA_4);
DigitalIn b2(PA_5);
DigitalIn l1(PA_1);
DigitalIn l2(PA_3);



DigitalOut LINE1(PB_7);
DigitalOut LINE2(PB_4);
DigitalOut LINE3(PB_5);
DigitalOut LINE4(PB_6);

extern serial_t stdio_uart;
Serial * pc;
RawSerial Main(PA_9,PA_10);


DigitalOut LED(D2);
Ticker aliving;

PwmOut NJW4617(PA_12);

uint16_t line[8];
bool OnWhite[8];
uint8_t state;   //linestate

void Blink(){
        LED = !LED;
}

int main() {
        serial_free(&stdio_uart);
        pc = new Serial(PB_3, NC);
        pc->baud(230400);
        Main.baud(460800);
        aliving.attach(&Blink,0.6);
        f1.mode(PullDown);
        f2.mode(PullDown);
        r1.mode(PullDown);
        r2.mode(PullDown);
        b1.mode(PullDown);
        b2.mode(PullDown);
        l1.mode(PullDown);
        l2.mode(PullDown);
        pc->printf("Hello World!!!\r\n\r\n\r\n");


        while (1) {
                OnWhite[0] = 0;
                OnWhite[1] = 0;
                OnWhite[2] = 0;
                OnWhite[3] = 0;
                OnWhite[4] = 0;
                OnWhite[5] = 0;
                OnWhite[6] = 0;
                OnWhite[7] = 0;
                state = 0;

                line[0] = F1.read()*1000;
                line[1] = F2.read()*1000;
                line[2] = R1.read()*1000;
                line[3] = R2.read()*1000;
                line[4] = B1.read()*1000;
                line[5] = B2.read()*1000;
                line[6] = L1.read()*1000;
                line[7] = L2.read()*1000;

                if(line[0] > ThresholdF1) OnWhite[0] = 1;
                if(line[1] > ThresholdF2) OnWhite[1] = 1;
                if(line[2] > ThresholdR1) OnWhite[2] = 1;
                if(line[3] > ThresholdR2) OnWhite[3] = 1;
                if(line[4] > ThresholdB1) OnWhite[4] = 1;
                if(line[5] > ThresholdB2) OnWhite[5] = 1;
                if(line[6] > ThresholdL1) OnWhite[6] = 1;
                if(line[7] > ThresholdL2) OnWhite[7] = 1;

                // if(OnWhite[0] || OnWhite[1]) {LINE1 = 1;}else{LINE1 = 0;}
                // if(OnWhite[2] || OnWhite[3]) {LINE2 = 1;}else{LINE2 = 0;}
                // if(OnWhite[4] || OnWhite[5]) {LINE3 = 1;}else{LINE3 = 0;}
                // if(OnWhite[6] || OnWhite[7]) {LINE4 = 1;}else{LINE4 = 0;}
                LINE1 = OnWhite[0]|OnWhite[1];
                LINE2 = OnWhite[2]|OnWhite[3];
                LINE3 = OnWhite[4]|OnWhite[5];
                LINE4 = OnWhite[6]|OnWhite[17];


                bitWrite(state,0,OnWhite[0]);
                bitWrite(state,1,OnWhite[1]);
                bitWrite(state,2,OnWhite[2]);
                bitWrite(state,3,OnWhite[3]);
                bitWrite(state,4,OnWhite[4]);
                bitWrite(state,5,OnWhite[5]);
                bitWrite(state,6,OnWhite[6]);
                bitWrite(state,7,OnWhite[7]);

                Main.putc(state);
                pc->printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\tstate: %d\r\n",line[0],line[1],line[2],line[3],line[4],line[5],line[6],line[7],state);

        }
}


// #include "mbed.h"
//
// #define bitRead(value, bit) (((value) >> (bit)) & 0x01)
// #define bitSet(value, bit) ((value) |= (1UL << (bit)))
// #define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
// #define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
//
// extern serial_t stdio_uart;
// Serial * pc;
//
//
// AnalogIn F1(PA_0);
// // AnalogIn F2(PA_2);
// AnalogIn R1(PA_6);
// AnalogIn R2(PA_7);
// AnalogIn B1(PA_4);
// AnalogIn B2(PA_5);
// AnalogIn L1(PA_1);
// AnalogIn L2(PA_3);
//
// DigitalIn f1(PA_0);
// // DigitalIn f2(PA_2);
// DigitalIn r1(PA_6);
// DigitalIn r2(PA_7);
// DigitalIn b1(PA_4);
// DigitalIn b2(PA_5);
// DigitalIn l1(PA_1);
// DigitalIn l2(PA_3);
//
//
//
// int main(){
//         serial_free(&stdio_uart);
//         pc = new Serial(PB_3, NC);
//         pc->baud(230400);
//         while(1) {
//                 pc->printf("Hello\n");
//         }
// }
