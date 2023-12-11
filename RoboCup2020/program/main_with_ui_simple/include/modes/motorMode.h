#ifndef _MOTORMODEH_
#define  _MOTORMODEH_
#include "setup.h"


void before_Motor(){
        // 試合開始時にする処理
        pc.printf("Before A\r\n");
}

void body_Motor(){
        pc.printf("Motor test\r\n");
        LCD.menuText("Motor Test\r\n");
        // MD.drive(0,40,0);
        // wait_ms(800);
        // for (size_t i = 1; i <= 4; i++)
        // {
        //         MD.run(i,30);
        //         wait_ms(400);
        // }
        MD.stopAll(); wait(3);
}

void after_Motor(){
        // 停止時の処理
        pc.printf("After A\r\n");
}



const LG_mode_t motorMode={name:"Motor",before: callback(before_Motor),body: callback(body_Motor),after: callback(after_Motor),modeColor: red};
#endif