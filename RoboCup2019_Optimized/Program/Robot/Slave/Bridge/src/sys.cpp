#include "sys.hpp"
#include "mbed.h"
#include "CodeBody.hpp"

//Sys.h's body.
//The defines are at Sys.h

extern Serial pc;
extern Serial spp;
//Additional Settings
#define DefaultMode 1
#define WaitSeconds 5
#define OneSecondCycles 700000

#define waitcycles OneSecondCycles*WaitSeconds

sys::sys(){

        pc.baud(115200);
        spp.baud(115200);
        sysLog(0x01,"Started\n");
// ------------------------------------------------

pc.printf("Hello. I'm Bridge\n Which mode will I play?\n 1  GAME MODE SPP Master\n 2  GAME MODE SPP Slave\n 3 RN42-I/RM Config Mode\n 4 Debug mode *This Mode prtints SPP and SPI law data.*\n");
pc.printf("Choose one! You have 5 minutes.\n *if you don't choose, dafault mode (Mode : %d )will be played. *\n",DefaultMode);


bool flg=false;
for(int i=0; i< waitcycles; i++) {
        if(pc.readable()) {
                flg=true;
                break;
        }

}



if(flg) {
        switch(pc.getc()) {
        case '1': mode=0x01;  break;
        case '2': mode=0x02;  break;
        case '3': mode=0x03;  break;
        case '4': mode=0x04;  break;
        Default: mode=DefaultMode; sysLog(0x0E,"[UI] Mode select recived but it wasn't a valid value. \n"); break;
        }
}else{
        mode=DefaultMode;
        sysLog(0x00,"Default Mode Playing...");
}
pc.printf("Playing mode '%d'.\n",mode);
switch (mode) {
case 0x01:  BTMaster(); break;
case 0x02:  BTSlave();  break;
case 0x03:  BTConfig(); break;
case 0x04:  Debug();break;
}


}

void sys::sysLog(unsigned char Code,char msg[]){
        status=0x00;
        pc.printf("log:[%d] %s\n",Code,msg);

}
