#include "mbed.h"
#include "rtos.h"

Serial pc(USBTX,USBRX);
AnalogIn CN1(A0);

int main(){
        wait(2);
        while(1) {
                pc.printf("US:%dcm\r\n",(int)(CN1.read()*300));
        }
}
