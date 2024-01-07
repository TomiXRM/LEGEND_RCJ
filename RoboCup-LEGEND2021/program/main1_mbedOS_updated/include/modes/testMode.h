#ifndef _TESTMODE_H_
#define _TESTMODE_H_
#include "setup.h"
Timer test;
void before_Test(){
        // ボールモニタ初期化処理
        pc.printf("start Test mode!\r\n");
        LCD.clearDisplay();
        LCD.menuText("Test now\r\n");
        mouse.reset();
        test.start();
        test.reset();
}


void body_Test(){
        DriveV = setZero();
        getSensors();
        Line.lineConpute(sensors.dir);
        if(test.read_ms() < 1000) {
                pidDir.target = 0;
        }else if(test.read_ms() < 2000) {
                pidDir.target = 270;
        }else if(test.read_ms() < 3000) {
                pidDir.target = 60;
        }else{
                test.reset();
        }
        MD.driveVector(DriveV,sensors.turnDir);
        pc.printf("%d \r\n",degBetween_signed(pidDir.target,sensors.calDir));
}

void after_Test(){
        // ボールモニタ終了処理
        pc.printf("After Test\r\n");
}

const LG_mode_t testMode={name:"Test",before: callback(before_Test),body: callback(body_Test),after: callback(after_Test),modeColor: red};
#endif