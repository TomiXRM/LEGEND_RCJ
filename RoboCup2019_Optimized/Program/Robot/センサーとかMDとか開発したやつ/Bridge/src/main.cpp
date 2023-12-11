#include "mbed.h"

//Including
#include "SPP.hpp"
#include "SysDefs.hpp"
#include "sys.hpp"
#include "SPILib.hpp"

//Port Initialize
Serial pc(USB_TX, USB_RX);     //Serial
sys Bridge;
//===================================

int main() {

        // put your setup code here, to run once:
        pc.baud(115200);
        short val=0x00;
        InterruptIn ss(SS);

        char *str="Hello";
        Bridge.sysLog(0x00,str);
        Slave spi(0x01,ss);

        while(1) {
                // put your main code here, to run repeatedly:
                if(spi.sentonce==true) {

                        spi.SetData(val);
                        val=val+1;
                }

                short gdata=spi.data;
                pc.printf("Recive:%c\n",gdata);
                wait_ms(30);
        }
        return 0;
}
