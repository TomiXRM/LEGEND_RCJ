#ifndef _LINEMODE_H_
#define  _LINEMODE_H_
#include "setup.h"

void before_Line(){
        // ボールモニタ初期化処理
        pc.printf("start Line mode!\r\n");
}

void body_Line(){

        //Line.readLine();
        // Line.lineManage();
        // for (size_t i = 0; i < 32; i++) {
        //   pc.printf("%d\t",Line.data[i]);
        // }
        // pc.printf("\r\n");
        //if(Line.sensorCount != 0 || Line.reverse_flag == true){
        //DriveV=makeXYFromRT(60,Line.result);
        //}else{
        //DriveV = SetZero();
        //}
        // pc.printf("R:%d\t%d\tB:%d\tF:%d\tC:%d\r\n", Line.result,Line.lineDegree,Line.sensorBegin,Line.sensorFinish,Line.sensorCount);
        LCD.menuText("Line now\r\n");
        wait_ms(1);
        //MD.driveVector(DriveV,0);
}

void after_Line(){
        // ボールモニタ終了処理
        pc.printf("After Line\r\n");
}

const LG_mode_t lineMode={name:"Line",before: callback(before_Line),body: callback(body_Line),after: callback(after_Line),modeColor: green};
#endif
