#include "mbed.h"
Serial pc(USBTX,USBRX);
int main() {
        pc.baud(230400);
        while(1) {
                pc.printf("Hello world\n");
                wait_ms(100);
        }
}
