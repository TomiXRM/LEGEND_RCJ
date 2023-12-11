#include "mbed.h"
#include "setup_com.h"
#include "setup.h"



#define CAMDIR_SET
// #define DRIBRER
//prototypes
INLINE void Body_Main();
INLINE void GoalTracking(uint8_t MovingSpeed);
INLINE void DecideFrontDegree(bool set);


Timer moveBackTimer;
Timer goalDegTimer;
Timer BallHoldingT;

Timer DribOn;
char goalDegTweekFlag;
int8_t BallPlusDeg;
bool camNone;//カメラ見えないフラグ
bool frontGoalDegF;
Timer CAM_Timer;
uint8_t DribPower;

uint8_t brib_kind;
//Functions

void after_Main(){
        DRIB.write(0.0);
}
INLINE void getSensors(){
        sensors.irRecieved = IR.get();  //取得開始
        cam.get();                      //取得開始
        IMU.get();
        sensors.PhotoSensor1 = Photo1.read_u16();
        sensors.PhotoSensor2 = Photo2.read_u16();
        // sensors.dir = (0.5-IMU.read()* -360)-180;//向いている角度の取得
        sensors.irDegLast  = sensors.irDeg;//前回の角度
        sensors.irDistLast = sensors.irDist;//前回の距離

        sensors.irDeg  = IR.getDir();//角度代入
        sensors.irDist = IR.getDist();//距離代
        if(sensors.irDist > 190) {
                sensors.irDist = 190;
        }
        // sensors.irDegTweak = sensors.irDeg;//計算ようの変数に代入
        // if(abs(sensors.irDeg  - sensors.irDegLast ) <= 23) { sensors.irDegSame = 1;}else{sensors.irDegSame = 0;}//前回と一緒判定
        // if(abs(sensors.irDist - sensors.irDistLast) <=  1) {sensors.irDistSame = 1;}else{sensors.irDistSame = 0;}//前回と一緒判定
        //IRv = makeXYFromRT(sensors.irDeg == 5000 ? 0 : 100, sensors.irDeg);//角度をベクトルに
        IRv = makeXYFromRT(sensors.irDeg == -5000 ? 0 : 100, sensors.irDeg);//角度をベクトル

        //sensors.DistB = (uint16_t)(smaDistB.updateData((float)(PSB_B.read()*260)));

        cam.recognizeCheck();//ゴールが見えているかいないか
        camera.OwnX = cam.OwnX;//自陣ゴールX座標
        camera.OwnY = cam.OwnY;//自陣ゴールY座標
        camera.OpsX = cam.OpsX;//敵陣ゴールX座標
        camera.OpsY = cam.OpsY;//敵陣ゴールY座標
        camera.OwnW = cam.OwnW;//自陣ゴール幅
        camera.OpsW = cam.OpsW;//敵陣ゴール幅
        camera.OpsDir = -cam.calDegree(1);//敵陣ゴールの角度
        camera.OwnDir = -cam.calDegree(0);//自陣ゴールの角度
        camera.OwnDist = sqrt(camera.OwnX*camera.OwnX+camera.OwnY*camera.OwnY);//敵陣ゴールの距離(カメラの値上)
        camera.OpsDist = sqrt(camera.OpsX*camera.OpsX+camera.OpsY*camera.OpsY);//自陣ゴールの距離(カメラの値上)

        //カメラで幅と端を書き込むプログラム
        camera.OwnGoalRightEnd = cam.OwnGoalRightEnd;
        camera.OwnGoalLeftEnd = cam.OwnGoalLeftEnd;
        camera.OpsGoalRightEnd = cam.OpsGoalRightEnd;
        camera.OpsGoalLeftEnd = cam.OpsGoalLeftEnd;



        sensors.dir = (float)IMU.dir;//角度
        sensors.velocity = IMU.velocity;//速度
        pidDir.Compute();

}

// INLINE void BallTracking(uint8_t MovingSpeed){
//         if(sensors.irDeg != -5000) {
//                 if(abs(sensors.irDeg) <= 30) {
//                         //TrackingDeg = IR.adjustment(sensors.irDeg,70,60,30,0,0);
//                         if(sensors.irDist < 190) {
//                                 TrackingDeg = sensors.irDeg*1.5;
//                         }else{
//                                 TrackingDeg = sensors.irDeg*2.8;
//                         }
//                         DRIB.write(0.7);
//                         if(sensors.PhotoSensor1 < 20000) {
//                                 // if(BallHoldingT.read_ms() > 500) {
//                                 //         BallHoldingT.reset();
//                                 //         ui.SetKICKER(1);
//                                 // }else{
//                                 //         ui.SetKICKER(0);
//                                 // }
//                                 ui.SetKICKER(1);
//                         }else{
//                                 BallHoldingT.reset();
//                                 ui.SetKICKER(0);
//                         }
//
//
//                         // if(sensors.Photo)
//                         //         DRIB.write(0.9);
//
// if(abs(sensors.irDeg)<30 && sensors.irDist > 160 && sensors.PhotoSensor < 61000) {
//         if(camera.OpsW != 0) {
//                 if(camera.OpsX >20) {
//                         goalDegTweekFlag = 1;
//                         sensors.turnDir += 50;
//                 }else if(camera.OpsX < -20) {
//                         goalDegTweekFlag = 2;
//                         sensors.turnDir -= 50;
//                 }else{
//                         goalDegTweekFlag = 0;
//                 }
//         }else{
//                 if(goalDegTimer.read_ms() < 200) {
//                         if(goalDegTweekFlag == 1) {
//                                 sensors.turnDir += 50;
//                         }else if(goalDegTweekFlag == 2) {
//                                 sensors.turnDir -= 50;
//                         }
//                 }else{
//                         goalDegTweekFlag = 0;
//                 }
//         }
//         ui.SetKICKER(1);
// }else{
//         ui.SetKICKER(0);
// }
//                 }else{
//                         DRIB.write(0);
//                         if(abs(sensors.irDeg) >=113) {
//                                 //90~180
//                                 // if(abs(sensors.irDeg) > 90 && abs(sensors.irDeg) >113) {
//                                 //         // BallPlusDeg = asin(60/(190 - sensors.irDist))/PI*180.0;
//                                 // }else
//                                 if(abs(sensors.irDeg) >= 113 && abs(sensors.irDeg) >135) {
//                                         BallPlusDeg = asin(60/(190 - sensors.irDist))/PI*180.0;
//                                 }else if(abs(sensors.irDeg) >= 135 && abs(sensors.irDeg) >158) {
//                                         BallPlusDeg = asin(60/(190 - sensors.irDist))/PI*180.0;
//                                 }else if(abs(sensors.irDeg) >= 158 && abs(sensors.irDeg) >= 180) {
//                                         BallPlusDeg = asin(90/(190 - sensors.irDist))/PI*180.0;
//                                 }
//                                 if(BallPlusDeg > 90) {BallPlusDeg = 90;}
//                                 if(sensors.irDeg > 0) {
//                                         //TrackingDeg = pow(abs(sensors.irDeg),1.3);
//                                         TrackingDeg = (sensors.irDeg+30)+BallPlusDeg;
//                                 }else{
//                                         //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
//                                         TrackingDeg = (sensors.irDeg-30)-BallPlusDeg;
//                                 }
//                         }else{
//                                 //40~90
//
//                                 if(sensors.irDeg > 0) {
//                                         //TrackingDeg = pow(abs(sensors.irDeg),1.3);
//                                         TrackingDeg = (sensors.irDeg+40)+asin(60/(190 - sensors.irDist))/PI*180.0;
//                                 }else{
//                                         //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
//                                         TrackingDeg = (sensors.irDeg-40)-asin(60/(190 - sensors.irDist))/PI*180.0;
//                                 }
//
//
//                         }
//
//                         // if(sensors.irDeg > 0) {
//                         //         //TrackingDeg = pow(abs(sensors.irDeg),1.3);
//                         //         TrackingDeg = (sensors.irDeg+40)+asin(45/(255 - sensors.irDist))/PI*180.0;
//                         // }else{
//                         //         //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
//                         //         TrackingDeg = (sensors.irDeg-40)-asin(45/(255 - sensors.irDist))/PI*180.0;
//                         // }
//                         //MovingSpeed = 0;
//                         ui.SetKICKER(0);
//                 }
//                 DriveV = makeXYFromRT(MovingSpeed,TrackingDeg);
//                 // if(sensors.irDeg < 0&& sensors.irDeg >= 90) {
//                 //         DriveV.x -= 7;
//                 // }
//                 if(abs(sensors.irDeg) <= 25) {
//                         DriveV.y +=30;
//                 }
//                 if(abs(sensors.irDeg) <= 90 &&abs(sensors.irDeg) > 22 && sensors.irDist < 170) {
//                         if(sensors.irDeg > 0) {
//                                 //TrackingDeg = pow(abs(sensors.irDeg),1.3);
//                                 DriveV.x = -140;
//                         }else{
//                                 //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
//                                 DriveV.x = 140;
//                         }
//                 }
//                 moveBackTimer.reset();
//         }else{
//                 ui.SetKICKER(0);
//                 if(camera.OwnW != 0) {
//                         DriveV.x = camera.OpsX*5;
//                 }else{
//                         DriveV = SetZero();
//                 }
//                 if(moveBackTimer.read_ms() < 500) {
//                         DriveV.y=-50;
//                 }
//
//         }
// }


INLINE void BallTracking(uint8_t MovingSpeed){
        frontGoalDegF = 0;
        if(sensors.irDeg != -5000) {
                // if(abs(sensors.irDeg) <= 90) {
                //         brib_kind = 9;
                //         frontGoalDegF = 1;
                // }else{
                //         brib_kind = 0;
                // }
                if(abs(sensors.irDeg) <= 25) {
                        //TrackingDeg = IR.adjustment(sensors.irDeg,70,60,30,0,0);

                        if(sensors.irDist < 180) {
                                TrackingDeg = sensors.irDeg*1.8;
                        }else{
                                TrackingDeg = sensors.irDeg*2.7;
                        }




                        DriveV = makeXYFromRT(MovingSpeed,TrackingDeg);







#ifdef DRIBRER
                        if(abs(sensors.irDeg)<30 && sensors.irDist > 160 && sensors.PhotoSensor1 < 1000||sensors.PhotoSensor2 < 1000) {
                                //ボールをホールド
                                if(camera.OpsW != 0) {
                                        if(abs(camera.OpsX) > 30 && sensors.PhotoSensor2 < 1000) {
                                                ui.SetKICKER(0);
                                                if(sensors.PhotoSensor2 < 1000) {
                                                        DriveV.x = camera.OpsX*3.6;
                                                        if(camera.OpsX > 0) {
                                                                // DriveV.x = 100;
                                                                DriveV.y = 120;
                                                        }else{

                                                                DriveV.y = 120;
                                                        }
                                                }
                                        }else{//どうかな
                                                if(BallHoldingT.read_ms() > 200) {
                                                        BallHoldingT.reset();
                                                        ui.SetKICKER(1);
                                                }else{
                                                        ui.SetKICKER(0);
                                                }
                                        }


                                        // if(sensors.velocity < 2) {
                                        //         BallHoldingT.start();
                                        //         DriveV.y = -60;
                                        // }else{
                                        //         if(BallHoldingT.read_ms() < 1000) {
                                        //                 DriveV.y = -60;
                                        //                 brib_kind = 9;
                                        //         }else{
                                        //                 BallHoldingT.reset();
                                        //         }
                                        // }
                                }else{
                                        if(sensors.PhotoSensor1 < 1000) {
                                                ui.SetKICKER(1);
                                        }else{
                                                ui.SetKICKER(0);
                                        }
                                }

                        }else{
                                ui.SetKICKER(0);
                        }
#else
                        if(sensors.irDist > 170 &&sensors.PhotoSensor2 < 1000) {
                                frontGoalDegF = 1;

                        }
                        if(abs(sensors.irDeg)<30 && sensors.irDist > 160 && sensors.PhotoSensor1 < 1000) {
                                if(sensors.velocity < 2) {
                                        //止まってる
                                        ui.SetKICKER(0);
                                        ui.SetBZ(3);

                                        // if((sensors.irDeg - camera.OwnDir) < 0) {
                                        //         sensors.irDeg += 50;
                                        // }else{
                                        //         sensors.irDeg -= 50;
                                        // }
                                }else{

                                        if(camera.OpsW =!0) {
                                                if(abs(camera.OpsX) < 15) {
                                                        ui.SetKICKER(1);
                                                }else{
                                                        ui.SetKICKER(0);
                                                }
                                        }else{
                                                ui.SetKICKER(1);
                                        }

                                }
                        }else{
                                ui.SetKICKER(0);
                        }




#endif




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
                                if(sensors.irDist < 115) {
                                        if(abs(sensors.irDeg) == 180) {
                                                TrackingDeg = sensors.irDeg+30;
                                        }else{
                                                if(sensors.irDeg > 0) {
                                                        //TrackingDeg = pow(abs(sensors.irDeg),1.3);
                                                        TrackingDeg = sensors.irDeg+20;
                                                }else{
                                                        //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                                                        TrackingDeg = sensors.irDeg-20;
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

                        ui.SetKICKER(0);
                        DriveV = makeXYFromRT(MovingSpeed,TrackingDeg);
                }else{

                        if(sensors.irDist < 170) {
                                if(sensors.irDeg > 0) {
                                        //TrackingDeg = pow(abs(sensors.irDeg),1.3);
                                        TrackingDeg = sensors.irDeg+15;
                                }else{
                                        //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                                        TrackingDeg = sensors.irDeg-15;
                                }
                        }else{
                                if(sensors.irDeg > 0) {
                                        //TrackingDeg = pow(abs(sensors.irDeg),1.3);
                                        TrackingDeg = (sensors.irDeg+60)+asin(50/(255 - sensors.irDist))/PI*180.0;
                                }else{
                                        //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                                        TrackingDeg = (sensors.irDeg-60)-asin(50/(255 - sensors.irDist))/PI*180.0;
                                }
                        }
                        ui.SetKICKER(0);
                        DriveV = makeXYFromRT(MovingSpeed,TrackingDeg);
                }
                if(sensors.irDeg < 0&& sensors.irDeg >= 90) {
                        DriveV.x -= 7;
                }
                if(abs(sensors.irDeg) <= 30) {
                        DriveV.y +=40;
                }
                // if(abs(sensors.irDeg) <= 90 &&abs(sensors.irDeg) > 22 && sensors.irDist < 170) {
                //         if(sensors.irDeg > 0) {
                //                 //TrackingDeg = pow(abs(sensors.irDeg),1.3);
                //                 DriveV.x = -140;
                //         }else{
                //                 //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                //                 DriveV.x = 140;
                //         }
                // }

        }else{
                ui.SetKICKER(0);
                if(camera.OwnW != 0) {
                        DriveV.x = camera.OpsX*5;
                }else{
                        DriveV = SetZero();
                }
                if(moveBackTimer.read_ms() < 500) {
                        DriveV.y=-70;
                }
        }

        DecideFrontDegree(frontGoalDegF);
}

INLINE void GoalTracking(uint8_t MovingSpeed){
        if(camera.OpsW != 0) {

        }else{
                //can't find
        }
}

INLINE void DecideFrontDegree(bool set){
  #ifdef CAMDIR_SET
        if(sensors.irDeg != -5000 &&  set == 1) {//abs(sensors.irDeg - camera.OpsDir) < 70
                if (camera.OpsW != 0) {
                        camNone = 0;
                        if (abs(sensors.dir) < 50) {
                                if(abs((int)sensors.irDeg+camera.OpsDir) < 80) {
                                        sensors.turnDir = -camera.OpsX*1.5;
                                }else{
                                        // sensors.turnDir = -camera.OpsX*0.7;
                                        sensors.calDir = sensors.dir;
                                }

                        }

                } else {
                        if (camNone == 1) {
                                if (CAM_Timer.read_ms() > 300) {
                                        camNone = 1;
                                        sensors.calDir = sensors.dir;
                                }
                        } else {
                                CAM_Timer.reset();
                                camNone = 1;
                                if (abs(sensors.dir) < 60) {
                                        sensors.turnDir = -camera.OpsX*1.5;
                                } else {
                                        sensors.calDir = sensors.dir;
                                }

                        }
                }
        }else{
                sensors.calDir = sensors.dir;
        }


  #else
        sensors.calDir = sensors.dir;
  #endif
}

INLINE void Body_Main(){
        getSensors();



        getSensors();
        if(Line.OutFlag == 1) {ui.SetBZ(0);}else{ui.SetBZ(7); /*0~7（7は音を鳴らさない）*/}//buzzer
        IRv = makeXYFromRT(sensors.irDeg == -5000 ? 0 : 100, sensors.irDeg);
        BallTracking(ui.speed);
        Bound = makeXYFromRT(100,Line.LastRaw); //前回ラインを踏んでいた
        Line.SetLine(&DriveV,&IRv,&Bound); //フィルタリング処理
        MD.driveVector(DriveV,sensors.turnDir);

        // //if(sensors.velocity < 2) {ui.SetBZ(0);}else{ui.SetBZ(7);}
        ui.putc(ui.state);//キッカー、LED、ブザーの情報をなげる

        switch (brib_kind) {
        case 0: DRIB.write(0); break;
        case 4: DRIB.write(0.4); break;
        case 5: DRIB.write(0.5); break;
        case 9: DRIB.write(0.9); break;
        default: DRIB.write(0.9); break;

        }



        // pc.printf("irDeg:%d\tTrackingDeg:%d\n",sensors.irDeg,TrackingDeg);
        // pc.printf("Order:%d\tRaw:%d\t",Line.order,Line.Raw);//ラインを踏んだ順番と現在踏んでいる角度
        // pc.printf("OwnX:%d\tOwnY:%d\tOpsX:%d\tOpsY:%d\tOpsDir:%d\tOwnDir:%d\t",camera.OwnX,camera.OwnY,camera.OpsX,camera.OpsY,camera.OpsDir,camera.OwnDir);//カメラ
}


INLINE void Body_Debug(){
        // IMU.get();
        // pc.printf("%d\t%d\t%d\r\n",Photo1.read_u16(),Photo2.read_u16(),IMU.velocity);
        getSensors();

        // if(camera.OpsDist < 60) {
        //         pc.printf("dist:%d\tdir:%d\tNeer!!\r\n",camera.OpsDist,camera.OpsDir);
        // }else{
        //         pc.printf("dist:%d\tdir:%d\tFar!!\r\n",camera.OpsDist,camera.OpsDir);
        // }
        // getSensors();
        // if(camera.OpsDir != 5000) {
        //         MD.drive(0, 0, camera.OpsDir*0.7);
        // }else{
        //         MD.drive(0, 0, 0);
        // }
        pc.printf("dist:%d,deg:%d\tdifDeg:%d\r\n",camera.OpsDist,camera.OpsDir,(camera.OpsDir - (int)(sensors.dir)));
        // MD.run(60,60,60,60);

}


void setup(){
        pc.baud(230400);// パソコン,IRのボーレート
        ui.baud(BAUDRATE);  //UIとのボーレートBAUDRATE

        pidDir.SetMode(AUTOMATIC);
        pidDir.SetOutputLimits(-100,100);
        pidDir.SetSampleTime(9);  //5


        ui.speed = 80;  //デフォルトスピード

        camera.useCamDirection = 1;
        DRIB.write(0.0);  wait_ms(100);
        DRIB.write(1.0);  wait_ms(600);
        DRIB.write(0.0);  wait_ms(600);


        //Mode enrties.
        ui.Entry(MAIN,DONOTHING,callback(&Body_Main),callback(&after_Main));  //callback(&stopAll)
        ui.Entry(DEBUG,DONOTHING,callback(&Body_Debug),DONOTHING);
        ui.Entry(SPEED,callback(&Before_Speed),callback(&Body_Speed),DONOTHING);

        initSpeedProcess();
        initBallProcess();
        initCompassProcess();
        initLineProcess();
        initCameraProcess();
        initUsProcess();


        moveBackTimer.start();
        goalDegTimer.start();
        BallHoldingT.start();
        CAM_Timer.start();
        DribOn.start();

        camNone = 0;


}


int main() {
        setup();
        pc.printf("\r\nHello world!! LEGEND\r\n");
        pc.printf("This STM32 works with %d MHz clock\r\n", (SystemCoreClock / 1000000));
        while(1) {
                ui.Run();
                //MD.run(60,60,60,60);
                // pc.printf("%d\r\n",IR.get());
                // wait_ms(100);
        }
}
