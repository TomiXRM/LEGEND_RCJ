#include "mbed.h"
#include "LGUSS.h"

/*
   PA0   A0   13
   PA1   A1   14
   PA2   A7    USBTX
   PA3   A2   15
   PA4   A3    SSEL
   PA5   A4    SCK
   PA6   A5    MISO
   PA7   A6    MOSI

   PA8   D9   16
   PA9   D1   InterTX
   PA10  D0   RX
   PA11  D10
   PA12  D2

   PB0   D3   1
   PB1   D6   2
   PB2
   PB3   D13  3
   PB4   D12
   PB5   D11  4
   PB6   D5   5
   PB7   D4   6
   PF0   D7   11
   PF1   D8   12
 */
/*ピン割り込みを使う場合はPAやPBのあとにつく数字をダブらせてはいけない。
   ダブるとピンが正常に動作しない。
 */
LGUSS US1(A3,A4);
// LGUSS US2(D7,D6);
// LGUSS US3(A0,D13);
// LGUSS US4(A1,D11);
// AnalogIn US16(A4);
Serial pc(USBTX,NC);

int main(){
        pc.baud(230400);
        while(1) {
                //pc.printf("%d\t%d\t%d\r\n",US1.distance,US2.distance,US3.distance);
                pc.printf("%d\r\n",US1.distance);
                // pc.printf("%d\n",(uint16_t)(US16.read()*300));
                //wait_ms(50);
        }
}
