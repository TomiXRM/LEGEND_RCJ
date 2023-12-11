#include "setup_com.h"
#include "setup.h"

extern LGUI ui;
extern RawSerial pc;
extern LGball IR;
extern AnalogIn Photo1;
extern AnalogIn Photo2;
void Body_Ball();

void initBallProcess(){
        ui.Entry(BALL,DONOTHING,callback(&Body_Ball),DONOTHING);
}
void Body_Ball(){
        IR.get();
        wait_us(10000);
        ui.putc('H');
        ui.putc(IR.data % 256);
        ui.putc(IR.data / 256);
        pc.printf("received:%d \tdegree;:%d \tdistance:%d\t%d\t%d\r\n",IR.data,IR.dir,IR.dist,Photo1.read_u16(),Photo2.read_u16());
}
