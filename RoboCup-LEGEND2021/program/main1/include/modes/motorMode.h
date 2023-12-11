#ifndef _MOTORMODEH_
#define  _MOTORMODEH_
#include "setup.h"


void before_Motor(){
        // 試合開始時にする処理
        pc.printf("Before Motor\r\n");
        btn.stateCenterLongPush = 0;
        MD.enable();
        wait_ms(700);
}

void body_Motor(){
        if(btn.stateCenterLongPush == 1) {
                MD.disable();
                btn.lastSPM = 0;
                btn.setSw1(0);
        }else{
                MD.enable();
                uint8_t i = 1;
                Timer t;
                t.start();
                while(btn.getSw1State() == 1) {
                        char s[8] = {0,0,0,0,0,0,0,0};
                        MD.run(i,50);
                        if(t.read_ms() > 1000) {
                                i++;
                                MD.stopAll();
                                t.reset();
                        }
                        if(i > 4) i = 1;
                        sprintf(s,"Motor:%d",i);
                        LCD.clearDisplay();
                        LCD.menuText(s);
                }
        }
}

void after_Motor(){
        // 停止時の処理
        btn.stateCenterLongPush = 0;
        pc.printf("After Motor\r\n");
}



const LG_mode_t motorMode={name:"Motor",before: callback(before_Motor),body: callback(body_Motor),after: callback(after_Motor),modeColor: cyan};
#endif