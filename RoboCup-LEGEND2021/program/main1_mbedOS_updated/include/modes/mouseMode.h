#ifndef _MOUSEMODE_H_
#define _MOUSEMODE_H_
#include "setup.h"

void before_mouse(){
        pc.printf("start mouse mode!\r\n");
        LCD.clearDisplay();
        LCD.menuText("Mouse test\r\n");
        mouse.reset();
}

void body_mouse(){
        // ボールモニタ表示処理
        DriveV = setZero();
        getSensors();
        DriveV.x = -mouse.x * 3;
        DriveV.y = -mouse.y * 3;
        if(DriveV.x > 70) { DriveV.x = 60;}else if(DriveV.x < -70) {DriveV.x = -70;}
        if(DriveV.y > 70) { DriveV.y = 70;}else if(DriveV.y < -70) {DriveV.y = -70;}
        MD.driveVector(DriveV,sensors.turnDir);
        pc.printf("X:%ld\tY:%ld\tMX:%d\tMY:%d\r\n",mouse.x,mouse.y,mouse.mouseX,mouse.mouseY);
}

void after_mouse(){
        // ボールモニタ終了処理
        pc.printf("After mouse\r\n");
}

const LG_mode_t mouseMode={name:"mouse",before: callback(before_mouse),body: callback(body_mouse),after: callback(after_mouse),modeColor: cyan};
#endif