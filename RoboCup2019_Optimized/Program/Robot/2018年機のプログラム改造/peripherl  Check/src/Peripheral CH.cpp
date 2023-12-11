#include "mbed.h"
InterruptIn A(PA_2);//D2
InterruptIn B(PB_3);//D3
InterruptIn C(PB_5);//D4
InterruptIn D(PC_4);
InterruptIn E(PB_14);
Serial pc(USBTX,NC);

uint16_t Time;

void A_rise(){
        pc.printf("PinA\r\n");
}
void B_rise(){
        pc.printf("PinB\r\n");
}
void C_rise(){
        pc.printf("PinC\r\n");
}
void D_rise(){
        pc.printf("PinD\r\n");
}
void E_rise(){
        pc.printf("PinE\r\n");
}
int main() {
        A.mode(PullDown);
        B.mode(PullDown);
        C.mode(PullDown);
        D.mode(PullDown);
        E.mode(PullDown);

        A.rise(&A_rise);
        B.rise(&B_rise);
        C.rise(&C_rise);
        D.rise(&D_rise);
        E.rise(&D_rise);

        Time = 0;
        while(1) {
                wait_ms(50);
                pc.printf("%d\r\n",(Time++));
        }
}
