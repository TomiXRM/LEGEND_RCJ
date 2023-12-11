#include "setup_com.h"
#include "setup.h"

extern LGUI ui;
extern RawSerial pc;


void Before_Speed();
void Body_Speed();
void ReceiveSpeed();

void initSpeedProcess(){
        ui.Entry(SPEED,callback(&Before_Speed),callback(&Body_Speed),DONOTHING);
}

void Before_Speed(){
        //Ready for speed mode
        ui.data_length = 0;
        ui.attachRx(&ReceiveSpeed);
        ui.putc(ui.speed);
        pc.printf("Mode detected");//DEBUG

}

void Body_Speed(){
        pc.printf("Speed Mode!!...%d\r\n",ui.speed);
}




void ReceiveSpeed(){
        if(ui.data_length == 0)       {//スピードの受信
                ui.speed = ui.getc();
                ui.data_length += 1;
        }else if(ui.data_length == 1) {//モードの受信
                ui.mode = ui.getc();
                ui.data_length = 0; //リセット
        }
        pc.printf("Speed:%d\r\n",ui.speed);
}
