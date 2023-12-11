#include "mbed.h"

#define OUT_OFFSET 0
#define TIMEOUT 833
#define INLINE inline __attribute__((always_inline))


DigitalIn IR[4] = {D1,D2,D3,D4};
DigitalOut state_led[4] = {D6,D8,D9,D10};
DigitalOut led(A2);
AnalogOut OUT(A3);
Timer IRTimer;
Serial pc(USBTX,NC);

uint16_t IR_PulseWidth[4];

INLINE void Read() {
        //  ir = 0;
        IR_PulseWidth[0] = 0;
        IR_PulseWidth[1] = 0;
        IR_PulseWidth[2] = 0;
        IR_PulseWidth[3] = 0;
        IRTimer.reset();
        IRTimer.start();
        while (IRTimer.read_us() < TIMEOUT) {
                for (int i = 0; i < 4; i++) {
                        if (!IR[i]) {
                                state_led[i]=!IR[i];
                                IR_PulseWidth[i] += 1;
                        }
                }
        }
        IRTimer.stop();

        pc.printf("%d\t", IR_PulseWidth[0]);
        pc.printf("%d\t", IR_PulseWidth[1]);
        pc.printf("%d\t", IR_PulseWidth[2]);
        pc.printf("%d\t", IR_PulseWidth[3]);
}

int main() {
        pc.baud(115200);
        IRTimer.start();
        while (1) {
                Read();
        }
}
