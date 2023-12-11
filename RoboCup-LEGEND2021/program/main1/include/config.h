#ifndef _CONFIGURATION_H_
#define  _CONFIGURATION_H_
#include "setup.h"

#include "modes/mainMode.h"
#include "modes/compassMode.h"
#include "modes/ballMode.h"
#include "modes/lineThrMode.h"
#include "modes/testMode.h"
#include "modes/kickerMode.h"
#include "modes/motorMode.h"
#include "modes/camMode.h"
#include "modes/mouseMode.h"
#include "PID_v1.h"

// PID pidDirection(&sensors.dir,&sensors.turnDir,0, 1.4, 0, 0.04,DIRECT);//方向制御のPID

const LG_mode_t modes[]={mainMode,compassMode,ballMode,lineThrMode,mouseMode,camMode,motorMode,testMode,kickerMode};
const short modeLength=9; // modes[] の長さ

void getDxFromIMU(){
        if(imuDirEnable) {
                if(abs(sensors.calDir) < 30) {
                        pidDir.Kd = -0.0003;
                        pidDir.out = -0.7*degBetween_signed(sensors.calDir,pidDir.target);
                }else{
                        pidDir.Kd = -0.0006;
                        pidDir.out = -1*degBetween_signed(sensors.calDir,pidDir.target);
                }
                pidDir.currentData = sensors.calDir;
                pidDir.dx = pidDir.lastData - pidDir.currentData;
                pidDir.dt = pidDt.read_us();
                // pidDir.differential = pidDir.dx/pidDir.dt;
                pidDir.laterData = pidDir.target - pidDir.currentData + (pidDir.dx * pidDir.dt);

                pidDir.out = pidDir.out - pidDir.laterData*pidDir.Kd;
                if(abs(pidDir.out) > 50) {
                        if(pidDir.out > 0) {
                                pidDir.out = 50;
                        }else{
                                pidDir.out = -50;
                        }
                }

                if(degBetween(pidDir.target,sensors.calDir) < 2) {
                        sensors.turnDir = 0;
                }else{
                        sensors.turnDir = pidDir.out;
                }
                pidDir.lastData = pidDir.currentData;
        }
}



INLINE void modeRun(){
        btn.ReadLongPusshPlus(); btn.ReadLongPusshMinus();//長押し判断
        if(btn.getMode() < 0) {
                btn.setMode(modeLength-1);
        }else if(btn.getMode() >= modeLength) {
                btn.setMode(0);
        }
        uint8_t sw1State=btn.getSw1State();        //ボタンの状態取りに行く
        uint8_t modeIndex = btn.getMode()%modeLength;
        LG_mode_t target=modes[modeIndex];        //modeIndexで選ばれる、対象のモードのstructデータを保存

        if(lastSw1State==sw1State) {  //メニュースイッチング、前回ループで保存した番号と同じか照合
                if(btn.getSw1State()==1) { //真ん中ボタン押した？？
                        modes[runningMode].body(); // モードの実行中は該当モードのbodyを実行する
                }else{
                        LCD.clearDisplay();
                        LCD.menuText(target.name);  // メニュー画面
                        rgb.turnOn(target.modeColor);//LEDメニューカラー
                        pc.printf("modeIndex:%d\r\n",modeIndex);
                }
        }else{
                //メニュー番号が変化した場合の処理
                if(lastSw1State==0&&sw1State==1) {        //初期起動時の処理
                        // Start
                        runningMode=modeIndex;
                        target.before();
                        rgb.turnOff();
                }else{
                        // Stop
                        // runningMode=0;//メニュー戻ったときにどこに戻るのか。
                        MD.stopAll();
                        btn.setMode(runningMode);
                        target.after();
                        // pc.printf("LEGEND2020!!\r\n");
                        pc.printf("LEGEND2021 RCAP\r\n");
                }
        }
        lastSw1State=sw1State;         // 直前の状態をストア、保存
}


#endif