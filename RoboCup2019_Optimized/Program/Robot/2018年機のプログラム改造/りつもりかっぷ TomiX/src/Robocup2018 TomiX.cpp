#include "mbed.h"
#include "SerialDebug.h"

SerialDebug Debug;
int main(){
        Debug.setDebug(0.01);
        Ball.frequency(230400);
        Ball.Setinterval(1000000); // 50ms*/
        wait(1);
        while (1) {
                //Debug.pc.printf("Helo\n");
                Ball.read();
                //wait_ms(20);
        }
}
