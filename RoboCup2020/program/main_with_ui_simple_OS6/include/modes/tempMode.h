#ifndef _TEMPMODE_H_
#define _TEMPMODE_H_
#include "setup.h"

void before_temp(){
        pc.printf("start temp mode!\r\n");
}

void body_temp(){
        // ボールモニタ表示処理
        pc.printf("temp mode!\r\n");
}

void after_temp(){
        // ボールモニタ終了処理
        pc.printf("After temp\r\n");
}

const LG_mode_t tempmode={name:"temp",before: callback(before_temp),body: callback(body_temp),after: callback(after_temp),modeColor: white};
#endif