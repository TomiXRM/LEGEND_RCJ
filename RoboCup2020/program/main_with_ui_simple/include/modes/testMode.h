#ifndef _TESTMODE_H_
#define _TESTMODE_H_
#include "setup.h"

void before_Test(){
        // ボールモニタ初期化処理
        pc.printf("start Test mode!\r\n");
        LCD.menuText("Test now\r\n");
}

void body_Test(){
        pc.printf("%d\t%d\r\n",(uint16_t)(Photo1.read_u16()/65.535),(uint16_t)(Photo2.read_u16()/65.535));
        wait_ms(1);
}

void after_Test(){
        // ボールモニタ終了処理
        pc.printf("After Test\r\n");
}

const LG_mode_t testMode={name:"Test",before: callback(before_Test),body: callback(body_Test),after: callback(after_Test),modeColor: red};
#endif