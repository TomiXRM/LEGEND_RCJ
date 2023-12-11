#include "setup_com.h"
#include "setup.h"

extern LGUI ui;
extern RawSerial pc;

void Body_US();

void initUsProcess(){
        ui.Entry(US,DONOTHING,callback(&Body_US),DONOTHING);
}
void Body_US(){
        pc.printf("Us Mode!!\r\n");
}
