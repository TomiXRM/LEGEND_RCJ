#include "setup_com.h"
#include "setup.h"

extern LGUI ui;
extern RawSerial pc;
extern LGball IR;

void Body_Ball();

void initBallProcess(){
        ui.Entry(BALL,DONOTHING,callback(&Body_Ball),DONOTHING);
}
void Body_Ball(){
        IR.get();
        wait_ms(10);
        ui.putc('H');
        ui.putc(IR.data % 256);
        ui.putc(IR.data / 256);
        pc.printf("received:%d \tdegree;:%d \tdistance:%d\r\n",IR.data,IR.dir,IR.dist);
}
