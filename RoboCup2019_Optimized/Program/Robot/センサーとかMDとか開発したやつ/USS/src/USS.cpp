#include "mbed.h"
#include "Ping.h"
Ping US(PB_7,PB_6);
Serial pc(USBTX,NC);
AnalogOut OUT(PA_4);

int Long[2];
int main(){
        while (1) {
                US.Send();
                wait_ms(50);
                Long[0] = US.Read_cm();
                Long[1] = Long[0];
                pc.printf("%dcm\r\n",Long);
                OUT.write(300 * (Long/300));
        }
}
