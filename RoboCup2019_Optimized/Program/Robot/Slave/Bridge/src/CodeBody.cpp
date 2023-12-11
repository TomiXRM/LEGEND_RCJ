#include "mbed.h"
#include "CodeBody.hpp"
#include "SysDefs.hpp"
#include "SPP.hpp"
#include "sys.hpp"
#include "SPILib.hpp"

extern Serial pc;
extern Serial spp;
extern sys sysobj;

//Mode 1 runner
void BTMaster(){

        while(1) {
                sysobj.sysLog(0x00,"master ModeRunning");
        }
}

//Mode 2 runner
void BTSlave(){

        while(1) {
                sysobj.sysLog(0x00,"slave ModeRunning");
        }
}
//Mode 3 runner
void BTConfig(){
        sysobj.sysLog(0x00,"Confing Mode　has been called.");

        pc.printf("**************RN42 I/RM Config mode ****************\n In config mode,Bridge sends your terminal typing to RN42's UART directly.\n You can start with '$$$', and you should end with 'R,1' to restert RN42!\n ");
        while(1) {
                if(pc.readable()) {

                        char c_pc=pc.getc();
                        spp.putc(c_pc);
                        char msg[]="";
                        sprintf(msg,"SPP sent:%c",c_pc);
                        sysobj.sysLog(0x0C,msg);
                }

                if(spp.readable()) {
                        char c_spp=spp.getc();
                        char msg[]="";
                        sprintf(msg,"SPP recived:%c",c_spp);
                        sysobj.sysLog(0x0D,msg);
                        pc.putc(c_spp);

                }
        }
}

//Mode 4 runner
void Debug(){
        sysobj.sysLog(0x00,"Debug Mode has been caled.");
        while(1) {

        }
}
