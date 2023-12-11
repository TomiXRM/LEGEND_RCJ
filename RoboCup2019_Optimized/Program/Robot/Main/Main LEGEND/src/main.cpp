#include "mbed.h"
#include "setup_com.h"
#include "setup.h"
#include "moving_average.h"

#define OWN_GOAL_IS_FOUND camera.OwnW!=0
#define BALL_IS_FOUND   sensors.irDeg != 5000
#define ROBOT_IS_IN_FRONT_OF_THE_GOAL camera.OwnGoalRightEnd >= 15 && camera.OwnGoalLeftEnd >= 15
#define ROBOT_IS_IN_RIGHT_OF_THE_GOAL camera.OwnGoalRightEnd < 15
#define ROBOT_IS_IN_LEFT_OF_THE_GOAL camera.OwnGoalLeftEnd < 15
#define BALL_IS_ON_THE_RIGHT_SIDE sensors.irDeg > 0
#define BALL_IS_ON_THE_LEFT_SIDE sensors.irDeg < 0


MovingAverage smaDistB(10);

//prototypes
INLINE void getSensors();
INLINE void sensorMonitor();
INLINE void Body_Main();
INLINE void Body_Debug();
INLINE void BallTracking(uint8_t MovingSpeed);
INLINE void GoBackToHome(uint8_t MovingSpeed);



short corner_counter=0;
short corner_runs_counter=0;
short escape_counter=0;
short escape_runs_counter=0;


//Functions
INLINE void getSensors(){
        sensors.irRecieved = IR.get();  //取得開始
        cam.get();                      //取得開始
        sensors.dir = (0.5-IMU.read()* -360)-180;//向いている角度の取得
        sensors.PhotoSensor = Photo.read_u16();
        if(abs(sensors.dir) < 4) {sensors.dir = 0;}//正面の微調整
        sensors.irDegLast  = sensors.irDeg;//前回の角度
        sensors.irDistLast = sensors.irDist;//前回の距離
        sensors.irDeg  = IR.getDir();//角度代入
        sensors.irDist = IR.getDist();//距離代入
        sensors.irDegTweak = sensors.irDeg;//計算ようの変数に代入
        if(abs(sensors.irDeg  - sensors.irDegLast ) <= 23) { sensors.irDegSame = 1;}else{sensors.irDegSame = 0;}//前回と一緒判定
        if(abs(sensors.irDist - sensors.irDistLast) <=  1) {sensors.irDistSame = 1;}else{sensors.irDistSame = 0;}//前回と一緒判定
        //IRv = makeXYFromRT(sensors.irDeg == 5000 ? 0 : 100, sensors.irDeg);//角度をベクトルに
        IRv = makeXYFromRT(sensors.irDeg == -5000 ? 0 : 100, sensors.irDeg);//角度をベクトル

        sensors.DistB = 5.3444 * pow(double(PSB_B.read()), -1.736);
        //sensors.DistB = (uint16_t)(smaDistB.updateData((float)(PSB_B.read()*260)));

        cam.recognizeCheck();//ゴールが見えているかいないか
        camera.OwnX = cam.OwnX;//自陣ゴールX座標
        camera.OwnY = cam.OwnY;//自陣ゴールY座標
        camera.OpsX = cam.OpsX;//敵陣ゴールX座標
        camera.OpsY = cam.OpsY;//敵陣ゴールY座標
        camera.OwnW = cam.OwnW;//自陣ゴール幅
        camera.OpsW = cam.OpsW;//敵陣ゴール幅
        camera.OpsDir = cam.calDegree(1);//敵陣ゴールの角度
        camera.OwnDir = cam.calDegree(0);//自陣ゴールの角度
        camera.OwnDist = sqrt(camera.OwnX*camera.OwnX+camera.OwnY*camera.OwnY);//敵陣ゴールの距離(カメラの値上)
        camera.OpsDist = sqrt(camera.OpsX*camera.OpsX+camera.OpsY*camera.OpsY);//自陣ゴールの距離(カメラの値上)

        //カメラで幅と端を書き込むプログラム
        camera.OwnGoalRightEnd = cam.OwnGoalRightEnd;
        camera.OwnGoalLeftEnd = cam.OwnGoalLeftEnd;
        camera.OpsGoalRightEnd = cam.OpsGoalRightEnd;
        camera.OpsGoalLeftEnd = cam.OpsGoalLeftEnd;


        sensors.PhotoSensor = Photo.read_u16();


        pidDir.Compute();
        //↓使う角度をカメラと方位センサから選ぶ
        /*if(camera.OpsW != 0) {//相手ゴールが見えている時
                //if(abs(sensors.dir) < 80) {
                sensors.camDir = ((float)(camera.OpsX)* -0.3);
                camera.useCamDirection = 1;
                directionOpsCtrl.reset();
                pidCamDir.Compute();
           }else{
                camera.useCamDirection = 0;
                pidDir.Compute();
           } */
}
INLINE void sensorMonitor(){
        //  pc.printf("Front:%d Right:%d Back:%d Left:%d\t",Line.front,Line.right,Line.back,Line.left);//ラインが触れているか
        //pc.printf("Order:%d\tRaw:%d\tLastRaw:%d\t",Line.order,Line.Raw,Line.LastRaw);   //ラインを踏んだ順番と現在踏んでいる角度

        pc.printf("IMUDir:%d\t",(short)(sensors.dir));
        //pc.printf("IRrecv:%d \tIRdeg:%d \tIRdist%d\tIRx:%d\tIRy:%d\t",sensors.irRecieved,sensors.irDeg,sensors.irDist,((short)(IRv.x)),((short)(IRv.y)));//IR
        //  pc.printf("OwnX:%d\tOwnY:%d\tOpsX:%d\tOpsY:%d\tOpsDir:%d\tOwnDir:%d\t",camera.OwnX,camera.OwnY,camera.OpsX,camera.OpsY,camera.OpsDir,camera.OwnDir);//カメラ
        //pc.printf("OwnDist:%d\tOpsDist%d\t",camera.OwnDist,camera.OpsDist);
        pc.printf("OwnRightEnd:%d\tOwnLeftEnd:%d\t",camera.OwnGoalRightEnd,camera.OwnGoalLeftEnd);

        pc.printf("Back:%d\t",sensors.DistB);
        pc.printf("\r\n");
}

INLINE void GoBackToHome(uint8_t MovingSpeed){
        if(camera.OwnW != 0) {
                if(sensors.DistB < 60) {
                        if(abs(camera.OwnX) < 5) {
                                //センター
                                if(sensors.DistB < 40) {
                                        ui.SetLED(0,0,0);//消灯
                                        DriveV = SetZero();
                                }else{
                                        //BackingToHomeDegree = camera.OwnDir;
                                        ui.SetLED(1,1,1);//しろ
                                        DriveV = makeXYFromRT(sensors.DistB,180);
                                }
                                ui.SetBZ(7);
                        }else{
                                if(camera.OwnX >= 5) {
                                        //右側
                                        if(camera.OwnGoalRightEnd < 0) {
                                                //ゴールよりさらに右側
                                                if(sensors.DistB < 66 && camera.OwnY > -60) {
                                                        ui.SetLED(1,0,0);//赤
                                                        DriveV = makeXYFromRT(MovingSpeed,60);
                                                }else{
                                                        ui.SetLED(1,1,0);//黄色
                                                        DriveV = makeXYFromRT(MovingSpeed,camera.OwnDir);
                                                }
                                                ui.SetBZ(0);
                                        }else{
                                                if(sensors.DistB < 40) {
                                                        ui.SetLED(1,0,0);//赤
                                                        DriveV = makeXYFromRT(MovingSpeed,90);
                                                }else{
                                                        ui.SetLED(1,1,0);//黄色
                                                        DriveV = makeXYFromRT(MovingSpeed,camera.OwnDir);
                                                }
                                                ui.SetBZ(7);
                                        }
                                }else if(camera.OwnX <= -5) {
                                        //左側
                                        if(camera.OwnGoalLeftEnd < 0) {
                                                //ゴールよりさらに左側
                                                if(sensors.DistB < 66 && camera.OwnY > -60) {
                                                        ui.SetLED(0,1,0);//緑
                                                        DriveV = makeXYFromRT((float)MovingSpeed,-60);
                                                }else{
                                                        ui.SetLED(0,1,1);//シアン
                                                        DriveV = makeXYFromRT(MovingSpeed,camera.OwnDir);
                                                }
                                                ui.SetBZ(0);
                                        }else{
                                                if(sensors.DistB < 40) {
                                                        ui.SetLED(0,1,0);//緑
                                                        DriveV = makeXYFromRT((float)MovingSpeed,-90);
                                                }else{
                                                        ui.SetLED(0,1,1);//シアン
                                                        DriveV = makeXYFromRT((float)MovingSpeed,camera.OwnDir);
                                                }
                                                ui.SetBZ(7);
                                        }
                                }
                        }
                }else{
                        DriveV = makeXYFromRT((float)MovingSpeed,camera.OwnDir);
                }
        }else{
                DriveV = makeXYFromRT((float)MovingSpeed,180);
        }
        //DriveV.x=0; DriveV.y = 0;
        //DriveV = makeXYFromRT((float)MovingSpeed,TrackingDeg);
        // DriveV = SetZero();

}

INLINE void GoBackToHome2(uint8_t MovingSpeed){

        if(camera.OwnW != 0) {
                //ゴールが後ろにある
                if(sensors.DistB >= 25) {
                        DriveV.y = -100;
                }else if(sensors.DistB < 25) {
                        DriveV.y = 0;
                }
        }else{

                //ゴールが後ろにない
                if(sensors.DistB >= 25) {
                        DriveV.y = -100;
                }else if(sensors.DistB < 25) {
                        DriveV.y = 0;
                }
        }
}

/*INLINE void1 Keeper(uint8_t MovingSpeed){
        if(sensors.irDeg != -5000) {
                if(camera.OwnW!=0) {
                        //カメラ見える
                        if(camera.OwnGoalRightEnd >= 15 && camera.OwnGoalLeftEnd >= 15) {
                                //ゴール上
                                if(abs(sensors.irDeg) < 5) {
                                        //ボール真ん中
                                        DriveV.x = IRv.x;
                                }else if(sensors.irDeg >= 5 && abs(sensors.irDeg) < 90) {
                                        //左
                                        DriveV.x = -MovingSpeed;
                                }else if(sensors.irDeg <= -5 && abs(sensors.irDeg) < 90) {
                                        //右
                                        DriveV.x = MovingSpeed;
                                }else{
                                        BallTracking(MovingSpeed);
                                }

                                if(sensors.DistB <= 50) {
                                        DriveV.y = 25 - sensors.DistB;
                                }else{
                                        //BallTracking(MovingSpeed);
                                        DriveV.y = -50;
                                }
                        }else{
                                if(camera.OwnGoalRightEnd < 15 && camera.OwnGoalRightEnd >= 0) {
                                        //右はし&ゴール上
                                        if(abs(sensors.irDeg) < 5) {
                                                //ボール真ん中
                                                DriveV.x = IRv.x;
                                        }else if(sensors.irDeg >= 5 && abs(sensors.irDeg) < 90) {
                                                //左
                                                DriveV.x = -MovingSpeed;
                                        }else if(sensors.irDeg <= -5 && abs(sensors.irDeg) < 90) {
                                                //右
                                                DriveV.x = MovingSpeed;
                                        }else{
                                                BallTracking(MovingSpeed);
                                        }
                                        if(sensors.DistB <= 50) {
                                                DriveV.y = 25 - sensors.DistB;
                                        }else{
                                                DriveV.y = -50;
                                        }
                                        DriveV.x = 0;
                                }else if(abs(camera.OwnGoalLeftEnd) < 15) {
                                        //左端
                                        if(abs(sensors.irDeg) < 5) {
                                                //ボール真ん中
                                                DriveV.x = IRv.x;
                                        }else if(sensors.irDeg >= 5 && abs(sensors.irDeg) < 90) {
                                                //左
                                                DriveV.x = -MovingSpeed;
                                        }else if(sensors.irDeg <= -5 && abs(sensors.irDeg) < 90) {
                                                //右
                                                DriveV.x = MovingSpeed;
                                        }else{
                                                BallTracking(MovingSpeed);
                                        }
                                        if(sensors.DistB <= 50) {
                                                DriveV.y = 25 - sensors.DistB;
                                        }else{
                                                DriveV.y = -50;
                                        }
                                        DriveV.x = 0;
                                }else{
                                        //ゴールがい
                                        if(abs(sensors.irDeg) >= 90) {
                                                BallTracking(MovingSpeed);
                                        }else{
                                                GoBackToHome(MovingSpeed);
                                        }
                                }
                        }
                }else{
                        //カメラ見えない
                        BallTracking(MovingSpeed);
                }
        }else{
                GoBackToHome(MovingSpeed);
        }
   }*/

INLINE void Keeper(uint8_t MovingSpeed){
        if(sensors.irDeg != -5000) {
                if(abs(sensors.irDeg)<2&&sensors.irDist>140) {
                        DriveV.x = 0;
                        DriveV.y = 30;

                }else{
                        if(abs(sensors.irDeg) <= 90) {
                                if(abs(sensors.irDeg) < 30) {
                                        if(sensors.irDeg < 0) {
                                                DriveV.x = 100;
                                                DriveV.y = 0;
                                        }else{
                                                DriveV.x = -100;
                                                DriveV.y = 0;
                                        }
                                }else{
                                        if(sensors.irDeg < 0) {
                                                DriveV.x = 140;
                                                DriveV.y = 0;
                                        }else{
                                                DriveV.x = -140;
                                                DriveV.y = 0;
                                        }
                                }
                        }else{
                                if(abs(sensors.irDeg) >= 135 && sensors.irRecieved/1000 <=3) {
                                        if(sensors.irDeg < 0) {
                                                DriveV=makeXYFromRT((float)MovingSpeed,sensors.irDeg - 80);
                                        }else{
                                                DriveV=makeXYFromRT((float)MovingSpeed,sensors.irDeg + 80);
                                        }
                                }else{
                                        DriveV.x = 0;
                                        DriveV.y = -100;
                                }
                        }
                }



                //ゴールフィルター
                if(camera.OwnW != 0) {         //カメラが見えている時
                        if(camera.OwnGoalRightEnd  < 50 && DriveV.x>0) {
                                DriveV.x *= 0.6;
                        }
                        if(camera.OwnGoalLeftEnd  < 50 && DriveV.x<0) {
                                DriveV.x *= 0.6;
                        }
                        if(camera.OwnGoalRightEnd <= 35 && DriveV.x >=0) {
                                if(camera.OwnGoalRightEnd < 10) {
                                        DriveV.x = -2*(10-camera.OwnGoalRightEnd);
                                }else{
                                        DriveV.x = 0;
                                }
                        }
                        if(camera.OwnGoalLeftEnd <= 35 && DriveV.x <=0) {
                                if(camera.OwnGoalLeftEnd < 10) {
                                        DriveV.x = 2*(10-camera.OwnGoalLeftEnd);
                                }else{
                                        DriveV.x = 0;
                                }
                        }
                }

                if(sensors.PhotoSensor < 40000&&abs(sensors.irDeg)<22) {
                        //ui.SetBZ(0);
                        //DriveV.x = -camera.OpsX*2;
                        ui.SetKICKER(1);

                }else{
                        ui.SetBZ(7);
                        ui.SetKICKER(0);
                        /*if(abs(sensors.irDeg) < 5) {
                                DriveV.y *= 0.5;
                           }*/
                }


        }else{
                GoBackToHome(MovingSpeed);
                //DriveV = SetZero();
        }


//近い。　ゴール付近でボールがこの値群より近かったら動かない
        short borderline=150;
        if(sensors.irDeg== -112) {
                borderline=150;
        }else if(sensors.irDeg==-135) {
                borderline=190;
        }else if(sensors.irDeg==-157) {
                borderline=160;
        }else if(sensors.irDeg==180) {
                borderline=150;
        }else if(sensors.irDeg==157) {
                borderline=150;
        }else if(sensors.irDeg==135) {
                borderline=187;
        }else if(sensors.irDeg==112) {
                borderline=150;

        }




        //上の値で判定して動かない部分
        if(sensors.DistB<20&& abs(sensors.irDeg)>110 && sensors.irDist>borderline) {
                DriveV=SetZero();
        }

        //前から出さない。
        // if(sensors.DistB<80&&camera.OwnGoalRightEnd>0&&camera.OwnGoalLeftEnd>0) {
        //         DriveV.y=-80;
        // }


}





//端っこで止まるのを書く
//角度が大きい時は150で横移動
//距離が遠い　かつ　角度が急なら止まる? 動きすぎたらブレまくる　-> アレイつかう？

INLINE void KeeperFoundTheBallInFrontOfTheGoal(){
        if(abs(sensors.irDeg) < 10) {
                //ボール正面
        }else if(BALL_IS_ON_THE_RIGHT_SIDE) {
                //右(=irDeg < 0)
                if(sensors.irDeg < -90) {
                        //右前
                }else{
                        //右後ろ
                }
        }else if(BALL_IS_ON_THE_LEFT_SIDE) {
                //左(=irDeg > 0)
        }else{
                //error
                pc.printf("Error:Ball_ROBOT_IS_IN_FRONT_OF_THE_GOAL");
        }
}

INLINE void Keeper_TomiX(uint8_t MovingSpeed){
        if(OWN_GOAL_IS_FOUND) {//自ゴールが見える
                if(BALL_IS_FOUND) {//ボールが見える
                        if(ROBOT_IS_IN_FRONT_OF_THE_GOAL) {
                                //ゴール上
                        }else if(ROBOT_IS_IN_RIGHT_OF_THE_GOAL) {
                                //ゴールより右
                        }else if(ROBOT_IS_IN_LEFT_OF_THE_GOAL) {
                                //ゴールより左
                        }else{
                                //error
                                pc.printf("Error:GOAL");
                        }
                }else{
                        //ボールが見えない
                }
        }else{
                if(BALL_IS_FOUND) {
                        //ボールが見えて自ゴールが見えない
                }else{
                        //ボールが見えなくて自ゴールが見えない
                }
        }
}






INLINE void BallTracking(uint8_t MovingSpeed){
        // if(sensors.irDeg != -5000) {
        //         if(abs(sensors.irDeg) < 22) {
        //                 TrackingDeg = sensors.irDeg*2;
        //                 DriveV = makeXYFromRT((float)MovingSpeed,TrackingDeg);
        //                 if(sensors.PhotoSensor < 5000) {
        //                         //ui.SetBZ(0);
        //                         //DriveV.x = -camera.OpsX*2;
        //                         ui.SetKICKER(1);
        //
        //                 }else{
        //                         ui.SetBZ(7);
        //                         ui.SetKICKER(0);
        //                         /*if(abs(sensors.irDeg) < 5) {
        //                                 DriveV.y *= 0.5;
        //                            }*/
        //                 }
        //         }else{
        //                 KickerCtrl.reset();
        //                 ui.SetKICKER(0);
        //                 if(abs(sensors.irDeg) >= 135) {
        //                         TrackingDeg = IR.adjustment(sensors.irDeg,90,70,40,0,0);
        //                 }else{
        //                         TrackingDeg = IR.adjustment(sensors.irDeg,70,50,30,0,0);
        //                         DriveV = makeXYFromRT((float)MovingSpeed,TrackingDeg);
        //                         if(IRv.y >0 && sensors.irRecieved/1000 <= 2 &&abs(sensors.irDeg) != 67) {
        //                                 DriveV.y += 20;
        //                         }
        //                 }
        //         }
        // }else{
        //         DriveV = SetZero();
        //         //GoBackToHome(MovingSpeed);
        // }
        if(sensors.irDeg != -5000) {
                if(abs(sensors.irDeg) <= 40) {
                        //TrackingDeg = IR.adjustment(sensors.irDeg,70,60,30,0,0);
                        if(sensors.irDist < 130) {
                                TrackingDeg = sensors.irDeg*1.5;
                        }else{
                                TrackingDeg = sensors.irDeg*2.7;
                        }
                        if(abs(sensors.irDeg)<30 && sensors.irDist > 160 && sensors.PhotoSensor < 61000) {
                                ui.SetKICKER(1);
                        }else{
                                ui.SetKICKER(0);
                        }
                }else if(abs(sensors.irDeg) >= 120) {
                        if(sensors.irDist < 115) {
                                if(abs(sensors.irDeg) == 180) {
                                        TrackingDeg = sensors.irDeg+60;
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
                                        TrackingDeg = (sensors.irDeg+40)+asin(70/(230 - sensors.irDist))/PI*180.0;
                                }else{
                                        //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                                        TrackingDeg = (sensors.irDeg-40)-asin(70/(230 - sensors.irDist))/PI*180.0;
                                }
                        }
                        ui.SetKICKER(0);
                }else{
                        if(sensors.irDist < 120) {
                                if(sensors.irDeg > 0) {
                                        //TrackingDeg = pow(abs(sensors.irDeg),1.3);
                                        TrackingDeg = sensors.irDeg+10;
                                }else{
                                        //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                                        TrackingDeg = sensors.irDeg-10;
                                }
                        }else{
                                if(sensors.irDeg > 0) {
                                        //TrackingDeg = pow(abs(sensors.irDeg),1.3);
                                        TrackingDeg = (sensors.irDeg+40)+asin(35/(200 - sensors.irDist))/PI*180.0;
                                }else{
                                        //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                                        TrackingDeg = (sensors.irDeg-40)-asin(25/(200 - sensors.irDist))/PI*180.0;
                                }
                        }
                        ui.SetKICKER(0);
                }
                DriveV = makeXYFromRT(MovingSpeed,TrackingDeg);
                if(sensors.irDeg < 0&& sensors.irDeg >= 90) {
                        DriveV.x -= 7;
                }
                if(abs(sensors.irDeg) <= 30) {
                        DriveV.y +=30;
                }
                if(abs(sensors.irDeg) <= 90 &&abs(sensors.irDeg) > 22 && sensors.irDist < 138) {
                        if(sensors.irDeg > 0) {
                                //TrackingDeg = pow(abs(sensors.irDeg),1.3);
                                DriveV.x = -130;
                        }else{
                                //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                                DriveV.x = 130;
                        }
                }

        }else{
                //DriveV = SetZero();
                GoBackToHome(MovingSpeed);
        }
}

INLINE void Body_Main(){
        ui.SetBZ(7);
        getSensors();
        BallTracking(ui.speed);
        //Keeper(ui.speed);
        /*if(DriveV.y > 0) {
                if(IRv.y >= 0) {

                        if(sensors.DistB > 60) {
                                DriveV.y = 0;
                        }else if(sensors.DistB > 90 || camera.OwnY > 76) {
                                DriveV.y = -50;
                        }
                        if(IRv.x > 0) {
                                DriveV.x = 100;
                        }else if(IRv.x < 0) {
                                DriveV.x = -100;
                        }else{
                                DriveV.x = 0;
                        }
                }
           }

           if(camera.OwnGoalRightEnd < 0) {
                DriveV.x = -50;
           }

           if(camera.OwnGoalLeftEnd < 0) {
                DriveV.x = 50;
           }*/
        //DriveV = SetZero();




        Bound = makeXYFromRT(100,Line.LastRaw);//前回ラインを踏んでいた

        Line.SetLine(&DriveV,&IRv,&Bound);//フィルタリング処理

        MD.driveVector(DriveV,sensors.turnDir);

        if(Line.OnFlag == 1) {ui.SetBZ(0);} /*0~7（7は音を鳴らさない）*///buzzer
        ui.putc(ui.state);//キッカー、LED、ブザーの情報をなげる
        //pc.printf("Order:%d\tRaw:%d\tLastRaw:%d\t",Line.order,Line.Raw,Line.LastRaw);//ラインを踏んだ順番と現在踏んでいる角度
        pc.printf("OwnX:%d\tOwnY:%d\tOwnW:%d\tOwnDir:%d\tOwnRigh:%d\tOwnLeft:%d\tBack:%d\tPhoto:%d",camera.OwnX,camera.OwnY,camera.OwnW,camera.OwnDir,camera.OwnGoalRightEnd,camera.OwnGoalLeftEnd,sensors.DistB,sensors.PhotoSensor);
        //pc.printf("FirstWhiteSensor:%d\tLastWhiteSensor:%d\torder:%d\5Raw:%d",Line.FirstWhiteSensor,Line.LastWhiteSensor,Line.order,Line.Raw);

        //pc.printf("Order:%d\tRaw:%d\tOutF:%d",Line.order,Line.Raw,Line.OutFlag); //ラインを踏んだ順番と現在踏んでいる角度

        //pc.printf("OwnX:%d\tOwnY:%d\tOpsX:%d\tOpsY:%d\tOpsDir:%d\tOwnDir:%d\tCamDir:%d\tturnDir:%d",camera.OwnX,camera.OwnY,camera.OpsX,camera.OpsY,camera.OpsDir,camera.OwnDir,(short)(sensors.camDir*100),(short)(sensors.turnDir*100));//カメラ
        pc.printf("\r\n");
}

INLINE void Body_Debug(){
        //Body_Main();
        getSensors();
        Line.SetLine(&DriveV,&IRv,&Bound);//フィルタリング処理
        sensorMonitor();
}


void setup(){
        pc.baud(230400);// パソコン,IRのボーレート
        ui.baud(BAUDRATE);//UIとのボーレート

        pidDir.SetMode(AUTOMATIC);
        pidDir.SetOutputLimits(-100,100);
        pidDir.SetSampleTime(9);//5
        pidCamDir.SetMode(AUTOMATIC);
        pidCamDir.SetOutputLimits(-60,60);
        pidCamDir.SetSampleTime(5);//5

        ui.speed = 100;//デフォルトスピード
        camera.useCamDirection = 1;
        _imu_.mode(PullDown);
        _MotoSLP.mode(PullDown);
        wait(1);
        //Mode enrties.
        ui.Entry(MAIN,DONOTHING,callback(&Body_Main),DONOTHING);//callback(&stopAll)
        ui.Entry(DEBUG,DONOTHING,callback(&Body_Debug),DONOTHING);

        initSpeedProcess();
        initBallProcess();
        initCompassProcess();
        initLineProcess();
        initCameraProcess();
        initUsProcess();
        KickerCtrl.start();

        ui.Entry(SPEED,callback(&Before_Speed),callback(&Body_Speed),DONOTHING);
}


int main() {
        setup();
        pc.printf("\r\nHello world!! LEGEND US_module\r\n");
        pc.printf("This STM32 works with %d MHz clock\r\n", (SystemCoreClock / 1000000));
        while(1) {
                ui.Run();
        }
}
