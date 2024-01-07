#ifndef _KICKERMODE_H_
#define _KICKERMODE_H_
#include "setup.h"


void before_kicker(){
        pc.printf("start kicker mode!\r\n");
        btn.stateCenterLongPush = 0;
}

void body_kicker(){
        if(btn.stateCenterLongPush == 1) {
                //     float power;
                //     btn.setMode((int16_t)Kicker.power*10);
                //     while(btn.getSw1State() == 1) {
                //             rgb.on(MAGENTA_L);
                //             char s[2] = {0,0};
                //             if(btn.getMode() < 0) {
                //                     btn.setMode(0);
                //             }else if(btn.getMode() > 10) {
                //                     btn.setMode(10);
                //             }
                //             power = (float)btn.getMode();
                //             power /= 10;
                //             Kicker.setPower(power);
                //             sprintf(s,"%d",(int16_t)power*10);
                //             LCD.menuText(s);
                //             pc.printf("Power Update!!\r\n");
                //     }
                //     btn.setMode(runningMode);
        }else{
                Kicker.setPower(10000);
                Kicker.Kick();
                rgb.on(RED_L);
                btn.setSw1(0);
        }
}

void after_kicker(){
        pc.printf("After kicker\r\n");
        Kicker.setPower();
        btn.lastSPM = 0;         //ボタンがなってくれないのでラストこれしてリセットしようね
        btn.stateCenterLongPush = 0;         //真ん中ロングプッシュがある場合はこれ推奨
}


const LG_mode_t kickerMode={name:"kicker",before: callback(before_kicker),body: callback(body_kicker),after: callback(after_kicker),modeColor: magenta};
#endif