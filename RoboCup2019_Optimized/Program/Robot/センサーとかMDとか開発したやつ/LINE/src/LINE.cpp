#include "mbed.h"
AnalogIn LINE[4] = {
        A0,A1,A2,A3
};

DigitalOut L1(D0);
DigitalOut L2(D1);
DigitalOut L3(D2);
DigitalOut L4(D3);
Serial pc(USBTX,NC);

uint16_t line[4];
uint16_t line_Threshold[4] {
        500,500,500,500
};
bool lineIsOn[4];

int main() {
        pc.baud(115200);
        while(1) {

                for (uint8_t e = 0; e < 4; e++) {
                        lineIsOn[e] = 0;
                        line[e] = 0;
                }//リセット
                for (uint8_t n = 0; n < 4; n++) {
                        line[n] = LINE[n].read() *1000;
                }


                for (uint8_t n = 0; n < 4; n++) {
                        if(line[n] > line_Threshold[n]) {
                                lineIsOn[n] = 1;
                        }
                }
                L1 = lineIsOn[0];
                L2 = lineIsOn[1];
                L3 = lineIsOn[2];
                L4 = lineIsOn[3];

                pc.printf("%d\t %d\t %d\t %d\r\n",line[0],line[1],line[2],line[3]);
        }
}
