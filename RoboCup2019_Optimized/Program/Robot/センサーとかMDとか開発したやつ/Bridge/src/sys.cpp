#include "sys.hpp"
#include "mbed.h"


//Sys.h's body.
//The defines are at Sys.h

extern Serial pc;

sys::sys(){
status=0x01;


}

void sys::sysLog(unsigned char Code,char msg[]){
status=0x00;
pc.printf("log:[%d] %s",Code,msg);

}
