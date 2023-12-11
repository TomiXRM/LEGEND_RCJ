#include "mbed.h"

#define map(x,in_min,in_max,out_min,out_max) ((x-in_min)*(out_max - out_min)/(in_max - in_min)+out_min)
AnalogIn PSD(A0);
Serial pc(USBTX,USBRX);
uint16_t dist;
int ir_val;


int main() {
        pc.baud(230400);
        while(1) {
                // ir_val = PSD.read_u16();
                //dist = (uint16_t)60.357*pow(map(ir_val,0,65535,0,5000)/1000.0,-1.16);
                dist = uint16_t(PSD.read() * 1000);
                pc.printf("%d\r\n",dist);
                wait_ms(10);
        }
}
