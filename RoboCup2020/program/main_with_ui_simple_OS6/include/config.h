#ifndef _CONFIGURATION_H_
#define  _CONFIGURATION_H_
#include "setup.h"

#include "modes/mainMode.h"
#include "modes/ballMode.h"
#include "modes/compassMode.h"
// #include "modes/lineMode.h"
#include "modes/lineThrMode.h"
#include "modes/testMode.h"


#include "PID_v1.h"

// PID pidDirection(&sensors.dir,&sensors.turnDir,0, 1.4, 0, 0.04,DIRECT);//方向制御のPID

const LG_mode_t modes[]={mainMode,ballMode,lineThrMode,compassMode,testMode};
const short modeLength=5; // modes[] の長さ

void getDxFromIMU(){
        pidDir.target = 0;
        if(abs(sensors.calDir) < 45) {
                pidDir.Kd = 0.0001;
                pidDir.out = -0.7*sensors.calDir;
        }else{
                pidDir.Kd = 0.0003;
                pidDir.out = -1.7*sensors.calDir;
        }
        pidDir.currentData = sensors.calDir;
        pidDir.dx = pidDir.lastData - pidDir.currentData;
        pidDir.dt = pidDt.read_us();
        // pidDir.differential = pidDir.dx/pidDir.dt;
        pidDir.laterData = pidDir.target - pidDir.currentData + (pidDir.dx * pidDir.dt);

        pidDir.out = pidDir.out - pidDir.laterData*pidDir.Kd;
        if(abs(pidDir.out) > 100) {
                if(pidDir.out > 0) {
                        pidDir.out = 70;
                }else{
                        pidDir.out = -70;
                }
        }

        if(abs(pidDir.target - sensors.calDir) < 3) {
                sensors.turnDir = 0;
        }else{
                sensors.turnDir = pidDir.out;
        }
        pidDir.lastData = pidDir.currentData;
        // pidDirection.Compute();
}



INLINE void modeRun(){
        btn.ReadLongPusshPlus(); btn.ReadLongPusshMinus();//長押し判断
        uint8_t sw1State=btn.getSw1State();        //ボタンの状態取りに行く
        uint8_t modeIndex=abs(btn.getMode()) % modeLength;  //モードとる
        LG_mode_t target=modes[modeIndex];        //modeIndexで選ばれる、対象のモードのstructデータを保存

        if(lastSw1State==sw1State) {  //メニュースイッチング、前回ループで保存した番号と同じか照合
                if(btn.getSw1State()==1) { //真ん中ボタン押した？？
                        modes[runningMode].body(); // モードの実行中は該当モードのbodyを実行する
                }else{
                        LCD.menuText(target.name);  // メニュー画面
                        rgb.turnOn(target.modeColor);//LEDメニューカラー
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
                }
        }
        lastSw1State=sw1State;         // 直前の状態をストア、保存
}

#endif