#include "mbed.h"
#include "SPP.hpp"
#include "sys.hpp"

extern Serial pc;
extern sys sysobj;
#define BTAddress "600308A174E9"//The Bluetooth device addrress that this is connecting.

bool SPPInit(){
        pc.printf("$$$");
        pc.printf("C,%s\r",BTAddress);

        wait(1.5);//Connecting Delay
        pc.printf("Hello\n");
        return true;
}
