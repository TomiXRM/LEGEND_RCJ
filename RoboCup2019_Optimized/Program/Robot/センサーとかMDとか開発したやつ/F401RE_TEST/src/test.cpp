#include "mbed.h"

Serial pc(USBTX, USBTX);
int16_t count;

void initial(){
        count = 0;
        pc.baud(115200);
}

int main(){
        initial();
        while(1) {
                count++;
                pc.printf("hello world!!! count:%d\r\n",count);
                wait_ms(100);
        }
}