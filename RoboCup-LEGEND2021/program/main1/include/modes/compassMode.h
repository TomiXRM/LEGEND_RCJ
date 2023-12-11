#ifndef _COMPASSMODE_H_
#define _COMPASSMODE_H_
#include "setup.h"

void before_Compass(){
        pc.printf("start Compass mode!\r\n");
        LCD.clearDisplay();
        LCD.menuText("COMPASS");
        btn.stateCenterLongPush = 0;
        IMU.setZero();
}

void body_Compass(){
        if(btn.stateCenterLongPush == 1) {
                sensors.calDir = (float)IMU.getDeg();
                DriveV = setZero();
                MD.driveVector(DriveV,sensors.turnDir);
                sensors.velocity = IMU.getVelocity();
                pc.printf("dir:%d\tvelocity:%d\tturn:%d\r\n",(int16_t)sensors.calDir,sensors.velocity,(int)sensors.turnDir);
        }else{

                char str[4]={0,0,0,0};
                sprintf(str,"%d",IMU.getDeg());
                wait_ms(1);
                LCD.clearDisplay();
                LCD.setTextSize(2);
                LCD.setTextCursor(CenterX(2,8,64),35);
                LCD.printf ("Set front");
                LCD.menuText(str);
        }

}

void after_Compass(){
        //   if(btn.stateCenterLongPush == 0)
        pc.printf("After Compass\r\n");
        btn.stateCenterLongPush = 0;
        btn.lastSPM = 0;
}

const LG_mode_t compassMode={name:"Compass",before: callback(before_Compass),body: callback(body_Compass),after: callback(after_Compass),modeColor: blue};
#endif