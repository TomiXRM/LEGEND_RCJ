#ifndef _MAINMOEDEH_
#define _MAINMOEDEH_
#include "setup.h"

#define IS_BALL_FOUND (sensors.irDeg != -5000)
#define IS_BALL_IN_FRONT_OF_ME (abs(sensors.irDeg) <= 30)
int16_t TrackingDeg;
uint16_t PhotoF,PhotoB;


void before_Main(){
        // 試合開始時にする処理
        // pc.printf("Before A\r\n");
}

INLINE void getSensors(){
        IMU.get();
        Line.readLine();
        sensors.irRecieved = IR.get();    //IR取得開始
        sensors.irDegLast  = sensors.irDeg; //IR前回の角度
        sensors.irDistLast = sensors.irDist; //IR前回の距離
        sensors.irDeg  = IR.getDir();//IR角度代入
        sensors.irDist = IR.getDist(); //IR距離代
        // sensors.irDist *= 0.8;
        if(sensors.irDist > 190) {sensors.irDist = 190;}//IR距離調整

        IRv = makeXYFromRT(sensors.irDeg == -5000 ? 0 : 100, sensors.irDeg);//角度をベクトル
        sensors.dir = IMU.dir; //角度
        sensors.velocity = IMU.velocity; //速度
        sensors.calDir = (float)sensors.dir;
        PhotoF = (uint16_t)(Photo1.read_u16()/65.535);
        PhotoB = (uint16_t)(Photo2.read_u16()/65.535);
        // pc.printf("%d\t%d\r\n",(uint16_t)(Photo1.read_u16()/65.535),(uint16_t)(Photo2.read_u16()/65.535));
        cam.recognizeCheck();//ゴールが見えているかいないか
        camera.OwnX = cam.OwnX; //自陣ゴールX座標
        camera.OwnY = cam.OwnY; //自陣ゴールY座標
        camera.OpsX = cam.OpsX; //敵陣ゴールX座標
        camera.OpsY = cam.OpsY; //敵陣ゴールY座標
        camera.OwnW = cam.OwnW; //自陣ゴール幅
        camera.OpsW = cam.OpsW; //敵陣ゴール幅
        camera.OpsDir = -cam.calDegree(1); //敵陣ゴールの角度
        camera.OwnDir = -cam.calDegree(0); //自陣ゴールの角度
        camera.OwnDist = sqrt(camera.OwnX*camera.OwnX+camera.OwnY*camera.OwnY); //敵陣ゴールの距離(カメラの値上)
        camera.OpsDist = sqrt(camera.OpsX*camera.OpsX+camera.OpsY*camera.OpsY); //自陣ゴールの距離(カメラの値上)

//カメラで幅と端を書き込むプログラム
        camera.OwnGoalRightEnd = cam.OwnGoalRightEnd;
        camera.OwnGoalLeftEnd = cam.OwnGoalLeftEnd;
        camera.OpsGoalRightEnd = cam.OpsGoalRightEnd;
        camera.OpsGoalLeftEnd = cam.OpsGoalLeftEnd;
        // pidDir.Compute();
}


INLINE void GotToCenter(){
        if(camera.OpsW != 0) {
                DriveV.x = camera.OpsX*5;
                if(abs(DriveV.x) > 100) {
                        if(DriveV.x < 0) {
                                DriveV.x = -100;
                        }else{
                                DriveV.x = 100;
                        }
                }

        }else{
                if(camera.OwnW != 0) {
                        DriveV.x = camera.OwnX*5;
                        if(DriveV.x < 0) {
                                DriveV.x = -100;
                        }else{
                                DriveV.x = 100;
                        }
                }else{
                        DriveV = setZero();
                }
        }
        if(moveBackTimer.read_ms() < 500) {
                DriveV.y=-70;
        }else{
                DriveV.y=0;
        }
}


INLINE void BallTracking(uint8_t MovingSpeed){
        if(IS_BALL_FOUND) {
                if(abs(sensors.irDeg) <= 25) {
                        if(sensors.irDist < 180) {
                                TrackingDeg = sensors.irDeg*1.8;
                        }else{
                                TrackingDeg = sensors.irDeg*3;
                        }
                        DriveV = makeXYFromRT(MovingSpeed,TrackingDeg);
                }else if(abs(sensors.irDeg) >= 120) {
                        if(abs(sensors.irDeg) < 111&&abs(sensors.irDeg) <140) {
                                if(sensors.irDeg > 0) {
                                        //TrackingDeg = pow(abs(sensors.irDeg),1.3);
                                        TrackingDeg = (sensors.irDeg+60)+asin(50/(255 - sensors.irDist))/PI*180.0;
                                }else{
                                        //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                                        TrackingDeg = (sensors.irDeg-60)-asin(50/(255 - sensors.irDist))/PI*180.0;
                                }
                        }else{
                                if(sensors.irDist < 170) {
                                        if(abs(sensors.irDeg) == 180) {
                                                TrackingDeg = sensors.irDeg+30;//ない
                                        }else{
                                                if(sensors.irDeg > 0) {
                                                        //TrackingDeg = pow(abs(sensors.irDeg),1.3);
                                                        TrackingDeg = sensors.irDeg+23;
                                                }else{
                                                        //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                                                        TrackingDeg = sensors.irDeg-23;
                                                }
                                        }
                                }else{
                                        if(sensors.irDeg > 0) {
                                                //TrackingDeg = pow(abs(sensors.irDeg),1.3);
                                                TrackingDeg = (sensors.irDeg+60)+asin(60/(255 - sensors.irDist))/PI*180.0;
                                        }else{
                                                //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                                                TrackingDeg = (sensors.irDeg-60)-asin(60/(255 - sensors.irDist))/PI*180.0;
                                        }
                                }
                        }
                        DriveV = makeXYFromRT(MovingSpeed,TrackingDeg);
                }else{//120みまん
                        if(sensors.irDeg > 0) {
                                //TrackingDeg = pow(abs(sensors.irDeg),1.3);
                                if(sensors.irDist < 190) {
                                        TrackingDeg = sensors.irDeg+15;
                                }else{
                                        TrackingDeg = (sensors.irDeg+60)+asin(60/(255 - sensors.irDist))/PI*180.0;
                                }
                        }else{
                                //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                                if(sensors.irDist < 180) {
                                        TrackingDeg = sensors.irDeg-15;
                                }else{
                                        TrackingDeg = (sensors.irDeg-60)-asin(60/(255 - sensors.irDist))/PI*180.0;
                                }

                        }
                        DriveV = makeXYFromRT(MovingSpeed,TrackingDeg);
                }
                // if(sensors.irDeg < 0&& sensors.irDeg >= 90) {
                //         DriveV.x -= 7;
                // }
                // if(abs(sensors.irDeg) <= 30 ) {
                //         DriveV.y +=40;
                // }
        }else{
                // DriveV.x = 0; DriveV.y = 0;
                GotToCenter();
        }
}


// INLINE void

// 試合中の処理
void body_Main(){
        getSensors();
        Line.lineConpute();
        BallTracking((80+(255-sensors.irDist)*0.1));

        if(PhotoF < 200 && PhotoB < 200 && IS_BALL_IN_FRONT_OF_ME) Kicker.Kick(); 
        btn.soundIndex(!Line.isOnLine*7);
        if(abs(sensors.irDeg) < 80) {
                if(IRv.x > 0 && DriveV.x > 0 && Line.currentLineVPer45.x > 0 && sensors.irDist < 190) DriveV.y = 70;
                if(IRv.x < 0 && DriveV.x < 0 && Line.currentLineVPer45.x < 0 && sensors.irDist < 190) DriveV.y = 70;

        }else if(abs(sensors.irDeg) > 100) {
                if(IRv.x > 0 && DriveV.x > 0 && Line.currentLineVPer45.x > 0 && sensors.irDist < 190) DriveV.y = -70;
                if(IRv.x < 0 && DriveV.x < 0 && Line.currentLineVPer45.x < 0 && sensors.irDist < 190) DriveV.y = -70;
        }
        Line.manage(&DriveV,&IRv,sensors.irDeg,sensors.dir);
        if(IS_BALL_FOUND && !Line.isHalfOut && abs(sensors.irDeg) < 100 && abs(sensors.irDeg) > 25 && abs(Line.lineDegree_CompassRotate) > 150) {
                DriveV.x *=50; DriveV.y = 0;
                if(sensors.irDeg > 0) {
                        DriveV.x = -100;
                }else if(sensors.irDeg < 0) {
                        DriveV.x = 100;
                }
        }
        MD.driveVector(DriveV,sensors.turnDir);
        // pc.printf("WhiteQty:%d\tCurrentDeg:%d\r\n",Line.onWhiteSensorQty,Line.lineCurrentDegree);
}

void after_Main(){
        // 停止時の処理
        btn.soundIndex(2);
        // pc.printf("After A\r\n");
}

const LG_mode_t mainMode={name:"Main",before: callback(before_Main),body: callback(body_Main),after: callback(after_Main),modeColor: white};

#endif
