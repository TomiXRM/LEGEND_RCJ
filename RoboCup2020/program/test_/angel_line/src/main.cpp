#include "mbed.h"

Serial pc(USBTX, USBRX);
BusOut channel(D7, D8, D9, D10, D11);
AnalogIn analog(A0);
PwmOut led(D6);

uint16_t data[16];

DigitalOut LED(D13);
Ticker blink;

void blinking()
{
        LED = !LED;
}
int main(){
        pc.printf("HELLO\r\n");
        pc.baud(230400);
        blink.attach(&blinking, 0.5);
        led.period_ms(5);
        led = 0.5;
        while (1) {
                for (uint8_t i = 0; i < 32; i++)
                {
                        channel = i;
                        data[i] = analog;
                        pc.printf("%d\t%d\r\n", i, data[i]);
                        wait_ms(50);
                }
        }
}