#include "setup.h"
#include "setup_com.h"

// Process initする(main.cpp)
// setup.cppにこのファイルをinclude
extern LGUI ui;
extern RawSerial pc;

void Before_OwnGoal();
void Body_OwnGoal();
void ReceiveOwnGoal();

void initOwnGoalProcess() {
        ui.Entry(OWNGOAL, callback(&Before_OwnGoal), callback(&Body_OwnGoal),
                 DONOTHING);
}

void Before_OwnGoal() {
        // Ready for speed mode
        ui.data_length = 0;
        ui.attachRx(&ReceiveOwnGoal);
        ui.putc(ui.OwnGoal);
        pc.printf("Mode detected"); // DEBUG
}

void Body_OwnGoal() {
        pc.printf("OwnGoal Mode!!...%d\r\n", ui.OwnGoal);
}

void ReceiveOwnGoal() {
        if (ui.data_length == 0) { //スピードの受信
                ui.OwnGoal = ui.getc();
                ui.data_length += 1;
        } else if (ui.data_length == 1) { //モードの受信
                ui.mode = ui.getc();
                ui.data_length = 0; //リセット
        }
        pc.printf("OwnGoal:%d\r\n", ui.OwnGoal);
}
