#ifndef _BALLMODE_H_
#define _BALLMODE_H_
#include "setup.h"

void before_Ball(){
        // ボールモニタ初期化処理
        pc.printf("start Ball mode!\r\n");
        LCD.menuText("Ball now\r\n");
}

void body_Ball(){
        // ボールモニタ表示処理
        sensors.irRecieved = IR.data; //取得開始
        sensors.irDeg  = IR.getDir();//角度代入
        sensors.irDist = IR.getDist(); //距離代

        uint8_t dist = (255-sensors.irDist)/4;
        dist = dist < 16 ? 16 : dist;

        LCD.Ballrt(dist,-sensors.irDeg);
        pc.printf("irRecieved:%d\tirDeg:%d\tirDist:%d\t",sensors.irRecieved,sensors.irDeg,sensors.irDist);
        pc.printf("%d\t%d\r\n",(uint16_t)(Photo1.read_u16()/65.535),(uint16_t)(Photo2.read_u16()/65.535));

        wait_ms(1);
}

void after_Ball(){
        // ボールモニタ終了処理
        pc.printf("After Ball\r\n");
}

const LG_mode_t ballMode={name:"Ball",before: callback(before_Ball),body: callback(body_Ball),after: callback(after_Ball),modeColor: red};
#endif