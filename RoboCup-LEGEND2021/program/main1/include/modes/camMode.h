#ifndef _CAMMODE_H_
#define _CAMMODE_H_
#include "setup.h"

void before_cam(){
        pc.printf("start cam mode!\r\n");
}

void body_cam(){
        // ボールモニタ表示処理
        getSensors();
        if(camera.OpsW != 0 & camera.OwnW != 0) {
                rgb.on(GREEN_L);
        }else{
                if(camera.OpsW != 0) {
                        rgb.on(MAGENTA_L);
                }else if(camera.OwnW != 0) {
                        rgb.on(CYAAN_L);
                }else{
                        rgb.turnOff();
                }
        }
        pc.printf("OwnX:%d\tOwnY:%d\tOwnW:%d\tOpsX:%d\tOpsY:%d\tOpsW:%d\tOpsDir:%d\tOwnDir:%d\tOwnDist:%d\tOpsDist:%d\r\n",camera.OwnX,camera.OwnY,camera.OwnW,camera.OpsX,camera.OpsY,camera.OpsW,camera.OpsDir,camera.OwnDir,camera.OpsDist,camera.OwnDist);
        wait_ms(10);
}

void after_cam(){
        // ボールモニタ終了処理
        pc.printf("After cam\r\n");
}

const LG_mode_t camMode={name:"cam",before: callback(before_cam),body: callback(body_cam),after: callback(after_cam),modeColor: yellow};
#endif