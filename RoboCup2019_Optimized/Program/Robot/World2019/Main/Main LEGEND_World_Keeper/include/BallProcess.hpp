#include "setup.h"
#include "setup_com.h"

extern LGUI ui;
extern RawSerial pc;
extern LGball IR;
extern AnalogIn Photo;
VectorXY IRvv;
VectorXY IRvDv;
void Body_Ball();

void initBallProcess() {
        ui.Entry(BALL, DONOTHING, callback(&Body_Ball), DONOTHING);
}
void Body_Ball() {
        IR.get();
        wait_ms(10);
        IRvv = makeXYFromRT(IR.dir == -5000 ? 0 : 100,
                            IR.dir); //角度をベクトル

        IRvDv = makeXYFromRT(IR.dist,
                             IR.dir); //角度をベクトル
        ui.putc('H');
        ui.putc(IR.data % 256);
        ui.putc(IR.data / 256);
        pc.printf("received:%d \tdegree;:%d \tdistance:%d \t Photo:%d \t x:%d\t "
                  "y:%d dx:%d\tdy:%d\t\r\n",
                  IR.data, IR.dir, IR.dist, Photo.read_u16(), IRvv.x, IRvv.y, IRvDv.x,
                  IRvDv.y);
}
