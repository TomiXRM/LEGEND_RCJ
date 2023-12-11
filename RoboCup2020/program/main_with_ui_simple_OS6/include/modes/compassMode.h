#ifndef _COMPASSMODE_H_
#define _COMPASSMODE_H_
#include "setup.h"

void before_Compass(){
        pc.printf("start Compass mode!\r\n");
        LCD.menuText("COMPASS");
}

void body_Compass(){
        // char str[5]={0,0,0,0,0};
        IMU.get();
        sensors.calDir = (float)IMU.dir;
        // wait_ms(5);
        DriveV = setZero();
        MD.driveVector(DriveV,sensors.turnDir);
        // sprintf(str,"%d",IMU.dir);
        pc.printf("dir:%d\tvelocity:%d\tturn:%d\r\n",IMU.dir,IMU.velocity,(int)sensors.turnDir);
        // LCD.menuText(str);
}

void after_Compass(){

        pc.printf("After Compass\r\n");
}

const LG_mode_t compassMode={name:"Compass",before: callback(before_Compass),body: callback(body_Compass),after: callback(after_Compass),modeColor: blue};
#endif