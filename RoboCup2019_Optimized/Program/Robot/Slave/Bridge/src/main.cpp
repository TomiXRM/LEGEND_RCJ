#include "mbed.h"

//Including
#include "SysDefs.hpp"
#include "SPP.hpp"
#include "sys.hpp"
#include "SPILib.hpp"
#include "CodeBody.hpp"
//Port Initialize
Serial pc(USB_TX, USB_RX);     //Serial
Serial spp(SPPTX,SPPRX);
sys sysobj;
//===================================


int main() {
//Main has no code but it is fune. The process body is  at CodeBody functions at CodeBody.cpp at  sys's constructor

}
