#include <mbed.h>

DigitalOut LED(PA_8);
Serial pc(PA_9,NC);
int main() {
  // put your setup code here, to run once:
  pc.baud(230400);
  while(1) {
          LED = !LED;
          pc.printf("helloworld\r\n");
          wait_ms(300);
    // put your main code here, to run repeatedly:
  }
}