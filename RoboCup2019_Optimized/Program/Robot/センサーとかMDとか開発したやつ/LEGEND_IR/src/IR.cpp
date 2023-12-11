#include "mbed.h"
DigitalOut led(D13, 0);
Serial pc(USBTX, NC);
RawSerial Nucleo(D5, NC); //必要
InterruptIn Call(D4);     ////必要
Timer IRTimer;
#define average_num 3
#define PI 3.14159265359
/*DigitalIn IRsensors[12] = {
        D0,//3
        D1,//2
        D2,//1
        D3,//0
        A6,//11
        A5,//10
        D6,//9
        D7,//8
        D8,//7
        D9,//6
        D10,//5
        D11//4
   };*/

DigitalIn IRsensors[12] = {D0, D1, D11, D2, D10, D3, D9,
                           A6, // A6
                           D8,
                           A5, // A5
                           D7, D6};
short degree = 5000;
short Pulse_123[3];
int IR_MaxPulse = 0;
char IRdistance = 0;
short IRdig = 0;
short IRlong;
short ir = 0;
short IR_PulseWidth[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint16_t IR_ActiveSensorCount;
const int timeout = 833;
static int v[2] = {0, 0};
static int f[2] = {0, 0};
short l[average_num];
float A = 0.2;
short data;
int Vx, Vy = 0;
void ballread() {
        degree = 5000;
        //  ir = 0;
        for (int j = 0; j < 12; j++) {
                IR_PulseWidth[j] = 0;
        }
        IRTimer.reset();
        IRTimer.start();
        while (IRTimer.read_us() < timeout) {
                for (int i = 0; i < 12; i++) {
                        if (!IRsensors[i]) {
                                IR_PulseWidth[i] += 1;
                        }
                }
        }
        IRTimer.stop();

        pc.printf("%d\t", IR_PulseWidth[0]);
        pc.printf("%d\t", IR_PulseWidth[1]);
        pc.printf("%d\t", IR_PulseWidth[2]);
        pc.printf("%d\t", IR_PulseWidth[3]);
        pc.printf("%d\t", IR_PulseWidth[3]);
        pc.printf("%d\t", IR_PulseWidth[5]);
        pc.printf("%d\t", IR_PulseWidth[6]);
        pc.printf("%d\t", IR_PulseWidth[7]);
        pc.printf("%d\t", IR_PulseWidth[8]);
        pc.printf("%d\t", IR_PulseWidth[9]);
        pc.printf("%d\t", IR_PulseWidth[10]);
        pc.printf("%d\t", IR_PulseWidth[11]);

        /*if (digree> 180) {
                digree -= 360;
           }*/

        IR_ActiveSensorCount = 0;
        Pulse_123[0] = 0;
        Pulse_123[1] = 0;
        Pulse_123[2] = 0;

        IR_MaxPulse = 0;
        for (int CT12 = 0; CT12 < 12; CT12++) { //見えてるやつ
                if (IR_PulseWidth[CT12] > IR_MaxPulse) {

                        Pulse_123[1] = IR_MaxPulse = IR_PulseWidth[CT12];
                        degree = CT12; // 30;
                }
        }
        IR_PulseWidth[degree] = 0; // 0にする(無効果)
        if (degree == 0) {
                degree = 0;
                /*if (abs(IR_PulseWidth[1] - IR_MaxPulse) < 25) {
                   // 1より
                   degree += 15;
                   }
                   if (abs(IR_PulseWidth[2] - IR_MaxPulse) < 25) {
                   // 2より
                   degree -= 15;
                   }

                   /*Vy = IR_PulseWidth[2] * 0.5 + IR_PulseWidth[4] * 0.866 +
                     IR_PulseWidth[3] * (-0.866) + IR_PulseWidth[1] * (-0.5);
                   Vx = IR_PulseWidth[0] + IR_PulseWidth[1] * 0.866 + IR_PulseWidth[3] * 0.5 +
                     IR_PulseWidth[4] * 0.5 + IR_PulseWidth[2] * (-0.866);*/
                /*degree += 360;
                   degree %= 360;*/
                if (IR_MaxPulse > 50) {
                        ir = 5000; //近い
                } else if (IR_MaxPulse > 35) {
                        ir = 4000; //中距離
                } else {
                        ir = 3000;
                }
        } else if (degree == 1) {
                degree = 330;
                /*Vy = IR_PulseWidth[2] * 0.5 + IR_PulseWidth[5] * (-1) +
                     IR_PulseWidth[3] * (-0.866) + IR_PulseWidth[1] * (-0.5);
                   Vx = IR_PulseWidth[0] + IR_PulseWidth[2] * 0.866 + IR_PulseWidth[3] * 0.5 +
                     IR_PulseWidth[1] * (-0.866);
                   degree = (atan2(Vy, Vx) / PI * 180) + 360;
                   degree %= 360;*/

                if (IR_MaxPulse > 35) {
                        ir = 5000; //近い
                } else if (IR_MaxPulse > 25) {
                        ir = 4000; //中距離
                } else {
                        ir = 3000;
                }
        } else if (degree == 2) {
                degree = 30;
                /*Vy = IR_PulseWidth[2] * 0.5 + IR_PulseWidth[4] * 0.866 + IR_PulseWidth[6]
                 + IR_PulseWidth[1] * (-0.5); Vx = IR_PulseWidth[0] + IR_PulseWidth[2] *
                   0.866 + IR_PulseWidth[4] * 0.5 + IR_PulseWidth[1] * (-0.866); degree =
                   (atan2(Vy, Vx) / PI * 180) + 360; degree %= 360; //*/
                if (IR_MaxPulse > 35) {
                        ir = 5000; //近い
                } else if (IR_MaxPulse > 25) {
                        ir = 4000; //中距離
                } else {
                        ir = 3000;
                }

        } else if (degree == 3) {
                degree = 300;
                if (IR_MaxPulse > 35) {
                        ir = 5000; //近い
                } else if (IR_MaxPulse > 25) {
                        ir = 4000; //中距離
                } else {
                        ir = 3000;
                }
        } else if (degree == 4) {
                degree = 60;

                if (IR_MaxPulse > 35) {
                        ir = 5000; //近い
                } else if (IR_MaxPulse > 25) {
                        ir = 4000; //中距離
                } else {
                        ir = 3000;
                }
        } else if (degree == 5) {
                degree = 270;

                if (IR_MaxPulse > 45) {
                        ir = 5000; //近い
                } else if (IR_MaxPulse > 35) {
                        ir = 4000; //中距離
                } else {
                        ir = 3000;
                }
        } else if (degree == 6) {
                degree = 90;
                if (IR_MaxPulse > 45) {
                        ir = 5000; //近い
                } else if (IR_MaxPulse > 35) {
                        ir = 4000; //中距離
                } else {
                        ir = 3000;
                }
        } else if (degree == 7) {
                degree = 240;
                if (IR_MaxPulse > 40) {
                        ir = 5000; //近い
                } else if (IR_MaxPulse > 30) {
                        ir = 4000; //中距離
                } else {
                        ir = 3000;
                }
        } else if (degree == 8) {
                degree = 120;
                if (IR_MaxPulse > 40) {
                        ir = 5000; //近い
                } else if (IR_MaxPulse > 30) {
                        ir = 4000; //中距離
                } else {
                        ir = 3000;
                }
        } else if (degree == 9) {
                degree = 210;
                if (IR_MaxPulse > 40) {
                        ir = 5000; //近い
                } else if (IR_MaxPulse > 30) {
                        ir = 4000; //中距離
                } else {
                        ir = 3000;
                }
        } else if (degree == 10) {
                degree = 150;
                if (IR_MaxPulse > 40) {
                        ir = 5000; //近い
                } else if (IR_MaxPulse > 30) {
                        ir = 4000; //中距離
                } else {
                        ir = 3000;
                }
        } else if (degree == 11) {
                degree = 180;
                if (IR_MaxPulse > 40) {
                        ir = 5000; //近い
                } else if (IR_MaxPulse > 30) {
                        ir = 4000; //中距離
                } else {
                        ir = 3000;
                }
        } else {
                ir = 2000;
                degree = 0;
        }

        /*v[0] = v[1];
           v[1] = A * v[0] + (1 - A) * degree;
           f[0] = f[1];
           f[1] = A * f[0] + (1 - A) * v[1];

           degree = f[1];*/

        /*if (IR_MaxPulse != 0) {
           //移動平均
           for (int i = 0; i < (average_num - 1); i++) {
            l[i] = l[i + 1];
           }
           l[(average_num - 1)] = degree;
           for (int i = 0; i < (average_num); i++) {
            degree += l[i];
           }
           degree = degree / average_num;
           //移動平均
           }*/

        IRdig = (int)degree;
        ir = ir + IRdig;

        data = ir;

        pc.printf(" angle:%d \t", IRdig);
        pc.printf(" long:%d \t", IR_MaxPulse);
        pc.printf(" Active:%d \t", IR_ActiveSensorCount);
        pc.printf(" ir:%d\r\n", ir); //*/
        wait_us(500);
}

void sendTomaster() {

        Nucleo.putc('H');
        Nucleo.putc(data % 256);
        Nucleo.putc(data / 256);
        // pc.printf("Interrupt!!\t");
        /// pc.printf("%d\r\n",data);
}

int main() {
        pc.baud(230400);
        Nucleo.baud(230400);
        IRTimer.start();
        Call.rise(&sendTomaster);
        Call.mode(PullDown);
        while (1) {
                led = !led;
                ballread();
                // pc.printf("angle is %d !!\r\n",ir);
        }
}
