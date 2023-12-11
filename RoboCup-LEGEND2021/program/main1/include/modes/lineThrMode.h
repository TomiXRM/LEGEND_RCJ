#ifndef _ANGELTHEMODEH_
#define  _ANGELTHEMODEH_
#include "setup.h"

void before_lineThr(){
        // pc.printf("start lineThr mode!\r\n");
        btn.stateCenterLongPush = 0;
}

void body_lineThr(){
        // ボールモニタ表示処理
        // btn.ReadLongPusshCenter();
        if(btn.stateCenterLongPush == 1) {
                for (size_t i = 0; i < 32; i++) {
                        dataThr[i] = 0;
                }

                while(btn.getSw1State() == 1) {
                        Line.checkThr();
                        LCD.menuText("ThrUpdate"); // メニュー画面
                        pc.printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n",Line.data[0],Line.data[1],Line.data[2],Line.data[3],Line.data[4],Line.data[5],Line.data[6],Line.data[7],Line.data[8],Line.data[9],Line.data[10],Line.data[11],Line.data[12],Line.data[13],Line.data[14],Line.data[15],Line.data[16],Line.data[17],Line.data[18],Line.data[19],Line.data[20],Line.data[21],Line.data[22],Line.data[23],Line.data[24],Line.data[25],Line.data[26],Line.data[27],Line.data[28],Line.data[29],Line.data[30],Line.data[31]);
                        rgb.on(GREEN_L);
                }
                // wait_ms(1);
        }else{
                Line.readLine();
                rgb.turnOff();
                pc.printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n",Line.data[0],Line.data[1],Line.data[2],Line.data[3],Line.data[4],Line.data[5],Line.data[6],Line.data[7],Line.data[8],Line.data[9],Line.data[10],Line.data[11],Line.data[12],Line.data[13],Line.data[14],Line.data[15],Line.data[16],Line.data[17],Line.data[18],Line.data[19],Line.data[20],Line.data[21],Line.data[22],Line.data[23],Line.data[24],Line.data[25],Line.data[26],Line.data[27],Line.data[28],Line.data[29],Line.data[30],Line.data[31]);
                wait_ms(10);
        }

}

void after_lineThr(){
        // ボールモニタ終了処理
        // pc.printf("After lineThr\r\n");
        if(btn.stateCenterLongPush == 1) {
                pc.printf("Threshold list\r\n");
                pc.printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n\r\n",dataThr[0],dataThr[1],dataThr[2],dataThr[3],dataThr[4],dataThr[5],dataThr[6],dataThr[7],dataThr[8],dataThr[9],dataThr[10],dataThr[11],dataThr[12],dataThr[13],dataThr[14],dataThr[15],dataThr[16],dataThr[17],dataThr[18],dataThr[19],dataThr[20],dataThr[21],dataThr[22],dataThr[23],dataThr[24],dataThr[25],dataThr[26],dataThr[27],dataThr[28],dataThr[29],dataThr[30],dataThr[31]);
        }
        btn.lastSPM = 0;//ボタンがなってくれないのでラストこれしてリセットしようね
        btn.stateCenterLongPush = 0;//真ん中ロングプッシュがある場合はこれ推奨
}

const LG_mode_t lineThrMode={name:"lineThr",before: callback(before_lineThr),body: callback(body_lineThr),after: callback(after_lineThr),modeColor: green};
#endif