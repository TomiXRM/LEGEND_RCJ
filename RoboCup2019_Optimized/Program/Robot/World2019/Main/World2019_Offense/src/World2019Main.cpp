#include "mbed.h"
#include "setup_com.h"
#include "setup.h"



#define CAMDIR_SET
#define IS_BALL_FOUND (sensors.irDeg != -5000)
#define IS_BALL_IN_FRONT_OF_THE_GOAL (abs(sensors.irDeg) <= 30)
#define IS_HOLDING_THE_BALL (abs(sensors.irDeg)<30 && sensors.irDist > 160 && sensors.PhotoSensor1 < PHOTO1_THRESHOLD)
#define IS_OPS_GOAL_FOUND (camera.OpsW != 0)
#define IS_OWN_GOAL_FOUND (camera.OwnW != 0)


#define PHOTO1_THRESHOLD 2000
#define PHOTO2_THRESHOLD 20000
// #define dribRER
//prototypes
INLINE void Body_Main();
INLINE void DecideFrontDegree(bool set);

INLINE void Soccer(uint8_t MovingSpeed);
INLINE void GoAroundToTheFront(uint8_t MovingSpeed);//回り込み
INLINE void GotToCenter();
INLINE void drible(uint8_t MovingSpeed);
INLINE void FrontBallTracking(uint8_t MovingSpeed);


Timer moveBackTimer;
Timer goalDegTimer;
Timer BallHoldingT;
Timer Drible_HoldBackTimer;
Timer Drible_HoldOwnSide;
Timer Drible_HoldCorner;
Timer MissingTheBall;
Timer Drible_Stack;

Timer dribOn;
char goalDegTweekFlag;
int8_t BallPlusDeg;
bool camNone;//カメラ見えないフラグ
bool frontGoalDegF;
bool isHoldTheBallInCorner;
Timer CAM_Timer;
uint8_t dribPower;




DigitalOut drib(PB_10);
Timeout dribOnTime;
Timeout dribOffTime;



//Functions

bool isDribOn;
bool isStack;
void dribIRQ0r();
void dribIRQ0l();
void dribIRQright();
void dribIRQleft();

void dribIRQ0r(){
        drib = 0;
        dribOnTime.attach_us(&dribIRQright,1500);
}
void dribIRQ0l(){
        drib = 0;
        dribOnTime.attach_us(&dribIRQleft,1500);
}
void dribIRQright(){
        drib = 1;
        dribOnTime.attach_us(&dribIRQ0r,2000);
}
void dribIRQleft(){
        drib = 1;
        dribOnTime.attach_us(&dribIRQ0l,1000);
}

void dribleState(bool state){
        if(state == 1) {
                if(isDribOn == 0) {
                        dribIRQright();
                        isDribOn = 1;
                }
        }else{
                if(isDribOn == 1) {

                        dribIRQleft();
                        isDribOn = 0;

                }
        }
}


void after_Main(){
        // drib.write(0.0);
        dribleState(0);
}

INLINE void getSensors(){
        sensors.irRecieved = IR.get();  //取得開始
        //   cam.get();                      //取得開始 NOTE:revで廃止
        //   IMU.get();//revで廃止
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
//1
// INLINE void drible(uint8_t MovingSpeed){
//         int16_t difference_deg;//differrence OpsGoal degree and IMUdeg.
//         // if(BallHoldingT.read_ms() < 100) {
//         //         DriveV.y = 60;
//         // }else
//         {
//                 if(IS_OPS_GOAL_FOUND) {//Opposide goal found
//                         difference_deg = abs((int16_t)(sensors.dir) - camera.OpsDir);
//                         // frontGoalDegF = 1;
//                         // if(abs(camera.OpsX) < 15) {
//                         //         ui.SetKICKER(1);
//                         // }else{
//                         //         ui.SetKICKER(0);
//                         // }
//                         // // DriveV.x = 5*camera.OpsX;
//                         // DriveV.y = MovingSpeed;
//                         if(camera.OpsY < 60) {
//                                 if(camera.OpsY < 40) {
//                                         DriveV.y = -90;//追加
//                                         DriveV.x = 0;//
//                                         Drible_HoldBackTimer.reset();//追加
//                                         frontGoalDegF = 0;//追加
//                                 }else{
//                                         if(abs(camera.OpsX) < 20) {
//                                                 DriveV.x = 3*camera.OpsX;//追加
//                                                 ui.SetKICKER(1);//追加
//                                                 frontGoalDegF = 0;//追加
//                                         }else{
//                                                 DriveV.x = *camera.OpsX;//追加
//                                                 ui.SetKICKER(0);//追加
//                                                 frontGoalDegF = 0;//追加
//                                         }
//                                 }
//                         }else{
//                                 if(difference_deg < 50) {
//                                         //the position able to shoot
//                                         if(Drible_HoldBackTimer.read_ms() < 600) {//追加
//                                                 frontGoalDegF = 0;//追加
//                                                 DriveV.y = -90;//追加
//                                                 DriveV.x = 3*camera.OpsX;//追加
//
//                                                 if(abs(camera.OpsX) < 15) {
//                                                         ui.SetKICKER(1);//追加
//                                                 }else{
//                                                         ui.SetKICKER(0);//追加
//                                                 }
//                                         }else{//追加
//                                                 frontGoalDegF = 1;
//                                                 if(abs(camera.OpsX) < 15) {
//                                                         ui.SetKICKER(1);
//                                                 }else{
//                                                         ui.SetKICKER(0);
//                                                 }
//                                                 DriveV.x = 3*camera.OpsX;
//                                                 DriveV.y = 70;
//                                         }//追加
//
//
//                                 }else{
//                                         // //move the position could shoot
//                                         //
//                                         //
//                                         DriveV.x = 5*camera.OpsX;
//                                         DriveV.y = -90;
//                                         frontGoalDegF = 0;
//                                         ui.SetKICKER(0);
//                                         // if(camera.OpsDir > 0) {
//                                         //         TrackingDeg = camera.OpsDir + 100;
//                                         // }else{
//                                         //         TrackingDeg = camera.OpsDir - 100;
//                                         // }
//                                         // DriveV = makeXYFromRT(100,TrackingDeg);
//                                         // DriveV.x = camera.OpsX * 5;
//                                         // if(DriveV.y >= 70) {DriveV.y = 70;}
//                                         // if(DriveV.y <= -70) {DriveV.y = -70;}
//                                         // Drible_HoldBackTimer.reset();//追加
//                                         // ui.SetKICKER(0);
//                                         // frontGoalDegF = 0;//追加
//                                 }
//                         }
//
//                 }else{
//                         frontGoalDegF = 0;
//                         if(IS_OWN_GOAL_FOUND) {// Own Goal
//                                 if(BallHoldingT.read_ms() < 100) {
//                                         DriveV.y = 70;
//                                         ui.SetKICKER(0);
//                                 }else{
//                                         // if(Drible_HoldOwnSide.read_ms() < 500) {
//                                         //         DriveV.y = MovingSpeed;
//                                         //         DriveV.x = camera.OwnX*6;
//                                         //         ui.SetKICKER(0);
//                                         // }else{
//                                         //         DriveV.y = MovingSpeed;
//                                         //         ui.SetKICKER(1);
//                                         // }
//                                         DriveV.y = MovingSpeed;
//                                         DriveV.x = camera.OwnX*5;
//                                         if(DriveV.x > 100) {DriveV.x = 100;}
//                                         if(DriveV.x < -100) {DriveV.x = -100;}
//                                         if(abs(camera.OwnX) < 15) {
//                                                 ui.SetKICKER(1);
//                                         }else{
//                                                 ui.SetKICKER(0);
//                                         }
//                                 }
//                         }else{
//                                 //none
//
//                                 DriveV.y = MovingSpeed;
//                                 if(BallHoldingT.read_ms() < 200) {
//                                         ui.SetKICKER(0);
//                                 }else{
//                                         ui.SetKICKER(1);
//                                 }
//
//                         }
//                 }
//         }
//
// }
INLINE void drible(uint8_t MovingSpeed){
        int16_t difference_deg;//differrence OpsGoal degree and IMUdeg.
        // if(BallHoldingT.read_ms() < 100) {
        //         DriveV.y = 80;
        // }else
        {
                if(IS_OPS_GOAL_FOUND) {//Opposide goal found
                        difference_deg = abs((int16_t)(sensors.dir) - camera.OpsDir);




                        if(difference_deg < 50) {
                                if(sensors.velocity < 1 && isStack == 0) {
                                        isStack = 1;
                                        Drible_Stack.reset();
                                }
                                if(isStack == 1) {
                                        if(Drible_Stack.read_ms() < 500) {
                                                DriveV.y = -90;
                                                DriveV.x = camera.OpsX*4;
                                                ui.SetBZ(3);
                                                ui.SetKICKER(0);
                                                if(abs(camera.OpsX) < 20) {
                                                        isStack = 0;
                                                }

                                        }else{
                                                if(abs(camera.OpsX) < 15) {
                                                        DriveV.y = 100; //追加
                                                        ui.SetKICKER(1);
                                                }
                                                isStack = 0;
                                        }
                                }else{

                                        //the position able to shoot
                                        if(Drible_HoldBackTimer.read_ms() < 500) { //追加
                                                frontGoalDegF = 0; //追加
                                                DriveV.y = -90; //追加
                                                DriveV.x = 3*camera.OpsX; //追加

                                                if(abs(camera.OpsX) < 25) {
                                                        DriveV.y = 100; //追加
                                                        ui.SetKICKER(1); //追加
                                                }else{
                                                        ui.SetKICKER(0); //追加
                                                }
                                        }else{ //追加
                                                frontGoalDegF = 1;
                                                if(abs(camera.OpsX) < 15) {
                                                        DriveV.y = 100; //追加
                                                        ui.SetKICKER(1);
                                                }else{
                                                        ui.SetKICKER(0);
                                                }
                                                DriveV.x = 2*camera.OpsX;
                                                DriveV.y = 70;
                                        } //追加
                                }

                        }else{
                                // //move the position could shoot
                                //
                                //
                                // DriveV.x = 5*camera.OpsX;
                                // DriveV.y = -90;
                                // frontGoalDegF = 0;
                                // ui.SetKICKER(0);
                                if(camera.OpsDir > 0) {
                                        TrackingDeg = camera.OpsDir + 100;
                                }else{
                                        TrackingDeg = camera.OpsDir - 100;
                                }
                                DriveV = makeXYFromRT(70,TrackingDeg);
                                DriveV.y -=50;
                                // DriveV.x = camera.OpsX * 4;
                                Drible_HoldBackTimer.reset();        //追加
                                ui.SetKICKER(0);
                                frontGoalDegF = 0;
                        }



                }else{
                        frontGoalDegF = 0;
                        if(IS_OWN_GOAL_FOUND) {// Own Goal
                                if(BallHoldingT.read_ms() < 10) {
                                        DriveV.y = 90;
                                        ui.SetKICKER(0);
                                }else{
                                        // if(Drible_HoldOwnSide.read_ms() < 500) {
                                        //         DriveV.y = MovingSpeed;
                                        //         DriveV.x = camera.OwnX*6;
                                        //         ui.SetKICKER(0);
                                        // }else{
                                        //         DriveV.y = MovingSpeed;
                                        //         ui.SetKICKER(1);
                                        // }
                                        DriveV.y = MovingSpeed;
                                        DriveV.x = camera.OwnX*4;
                                        if(DriveV.x > 100) {DriveV.x = 100;}
                                        if(DriveV.x < -100) {DriveV.x = -100;}
                                        if(abs(camera.OwnX) < 20 || BallHoldingT.read_ms() > 400) {
                                                ui.SetKICKER(1);
                                                BallHoldingT.reset();
                                        }else{
                                                ui.SetKICKER(0);
                                        }
                                }
                        }else{
                                //none

                                // DriveV.y = 10;
                                // if(BallHoldingT.read_ms() < 200) {
                                //         ui.SetKICKER(0);
                                // }else{
                                //         ui.SetKICKER(1);
                                // }
                                ui.SetKICKER(0);

                        }
                }
        }

}


INLINE void BallTracking(uint8_t MovingSpeed){
        frontGoalDegF = 0;


        if(IS_BALL_FOUND) {
                if(abs(sensors.irDeg) < 90) {
                        dribleState(1);
                        // frontGoalDegF = 1;
                }
                if(abs(sensors.irDeg) <= 25) {

                        if(sensors.irDist < 180) {
                                TrackingDeg = sensors.irDeg*1.8;
                        }else{
                                TrackingDeg = sensors.irDeg*3;
                        }


                        DriveV = makeXYFromRT(MovingSpeed,TrackingDeg);
                        // if(sensors.irDist > 170 &&sensors.PhotoSensor2 < 1000) {
                        //         frontGoalDegF = 1;
                        // }
                        if(IS_HOLDING_THE_BALL) {
                                // if(sensors.velocity < 1) {
                                //         //止まってる
                                //         ui.SetKICKER(0);
                                //         ui.SetBZ(3);
                                // }else{
                                //
                                //         if(camera.OpsW =!0) {
                                //                 if(abs(camera.OpsX) < 15) {
                                //                         ui.SetKICKER(1);
                                //                 }else{
                                //                         ui.SetKICKER(0);
                                //                 }
                                //         }else{
                                //                 ui.SetKICKER(1);
                                //         }
                                //
                                // }
                                drible(MovingSpeed);
                        }else{
                                ui.SetKICKER(0);
                                BallHoldingT.reset();
                                Drible_HoldOwnSide.reset();
                        }
                }else if(abs(sensors.irDeg) >= 120) {
                        dribleState(0);

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
                        DriveV.x = 0; DriveV.y = 0;

                        ui.SetKICKER(0);
                        BallHoldingT.reset();
                        Drible_HoldOwnSide.reset();
                        // isHoldTheBallInCorner = 0;
                        DriveV = makeXYFromRT(MovingSpeed,TrackingDeg);
                }else{
                        dribleState(0);

                        if(sensors.irDist < 165) {
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
                                        TrackingDeg = (sensors.irDeg+60)+asin(60/(255 - sensors.irDist))/PI*180.0;
                                }else{
                                        //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                                        TrackingDeg = (sensors.irDeg-60)-asin(60/(255 - sensors.irDist))/PI*180.0;
                                }
                        }
                        ui.SetKICKER(0);
                        BallHoldingT.reset();
                        Drible_HoldOwnSide.reset();
                        // isHoldTheBallInCorner = 0;
                        DriveV = makeXYFromRT(MovingSpeed,TrackingDeg);
                }
                if(sensors.irDeg < 0&& sensors.irDeg >= 90) {
                        DriveV.x -= 7;
                }
                if(abs(sensors.irDeg) <= 30 ) {
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
                moveBackTimer.reset();
                MissingTheBall.reset();

                if(abs(sensors.irDeg) > 120) {

                }
        }else{
                ui.SetKICKER(0);
                // if(IS_OPS_GOAL_FOUND) {
                //         DriveV.x = camera.OpsX*5;
                // }else{
                //         DriveV = SetZero();
                // }
                // if(moveBackTimer.read_ms() < 500) {
                //         DriveV.y=-70;
                // }
                GotToCenter();
                // if(MissingTheBall.read() < 10) {
                //         GotToCenter();
                //         // MD.drive(0,0,sensors.turnDir);
                //         DriveV.x = 0; DriveV.y = 0;
                // }else{
                //         MD.drive(0,70,sensors.turnDir);
                //         MissingTheBall.read();
                //         while(!IS_BALL_FOUND && MissingTheBall.read() < 2) {
                //                 getSensors();
                //                 MD.drive(0,70,sensors.turnDir);
                //         } MissingTheBall.reset();
                //
                //
                // }
                dribleState(0);//
                BallHoldingT.reset();
                Drible_HoldOwnSide.reset();
        }

        DecideFrontDegree(frontGoalDegF);
}

// INLINE void GoAroundToTheFront(uint8_t MovingSpeed,int8_t arrow_degree){
// //回り込み
// //arrow_degree is valiable that irdeg upper than arrow_degree you arrow to Make in fron of the ball.
//         if(abs(sensors.irDeg) > arrow_degree) {
//                 if(abs(sensors.irDeg) > 120) {
//                         if(sensors.irDist < 115) {
//                                 if(abs(sensors.irDeg) == 180) {
//                                         TrackingDeg = sensors.irDeg+60;
//                                 }else{
//                                         if(sensors.irDeg > 0) {
//                                                 //TrackingDeg = pow(abs(sensors.irDeg),1.3);
//                                                 TrackingDeg = sensors.irDeg+20;
//                                         }else{
//                                                 //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
//                                                 TrackingDeg = sensors.irDeg-20;
//                                         }
//                                 }
//                         }
//                 }else{
//                         if(sensors.irDist < 120) {
//                                 if(sensors.irDeg > 0) {
//                                         //TrackingDeg = pow(abs(sensors.irDeg),1.3);
//                                         TrackingDeg = sensors.irDeg+10;
//                                 }else{
//                                         //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
//                                         TrackingDeg = sensors.irDeg-10;
//                                 }
//                         }else{
//                                 if(sensors.irDeg > 0) {
//                                         //TrackingDeg = pow(abs(sensors.irDeg),1.3);
//                                         TrackingDeg = (sensors.irDeg+40)+asin(35/(200 - sensors.irDist))/PI*180.0;
//                                 }else{
//                                         //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
//                                         TrackingDeg = (sensors.irDeg-40)-asin(25/(200 - sensors.irDist))/PI*180.0;
//                                 }
//                         }
//                 }
//         }else{
//                 //none
//         }
//         DriveV = makeXYFromRT(MovingSpeed,TrackingDeg);
//
//         // if(sensors.irDeg <  0&& sensors.irDeg >= 90) {
//         //         DriveV.x -= 7;
//         // }
// }
// INLINE void Soccer(uint8_t MovingSpeed){
//         if(IS_BALL_FOUND) {
//                 //recognize the Ball
//                 if(IS_BALL_IN_FRONT_OF_THE_GOAL) {//30
//                         //preparetion of catch the ball
//                         //dribler on
//                         if(IS_HOLDING_THE_BALL) {
//                                 //dribrer function
//                                 drible(MovingSpeed);
//                         }else{
//                                 DecideFrontDegree(0);
//                                 FrontBallTracking(MovingSpeed);
//                         }
//                 }else{
//                         //dribler off
//                         DecideFrontDegree(0);
//                         GoAroundToTheFront(MovingSpeed,30);//speed , arrow_degree
//                 }
//                 //valiebles changes
//                 moveBackTimer.reset();//The timer using for get back to center position.
//         }else{
//                 //couldn't recognize the ball
//                 //move to home position?
//                 DecideFrontDegree(0);
//                 GotToCenter();//move the robot in center of court
//                 //valiebles changes
//         }
// }
//
//
INLINE void GotToCenter(){
        if(camera.OpsW != 0) {
                DriveV.x = camera.OpsX*5;
                if(abs(DriveV.x) > 100) {
                        if(DriveV.x > 0) {
                                DriveV.x = 100;
                        }else{
                                DriveV.x = -100;
                        }
                }
                ui.SetLED(1,1,1);
        }else{
                if(camera.OwnW != 0) {
                        DriveV.x = camera.OwnX*5;
                        if(DriveV.x > 0) {
                                DriveV.x = 100;
                        }else{
                                DriveV.x = -100;
                        }
                }else{
                        DriveV = SetZero();
                        ui.SetLED(1,0,0);
                }
        }
        if(moveBackTimer.read_ms() < 500) {
                DriveV.y=-70;
        }else{
                DriveV.y=0;
        }
}
//
// INLINE void FrontBallTracking(uint8_t MovingSpeed){
//         if(sensors.irDist < 180) {
//                 TrackingDeg = sensors.irDeg*1.8;
//         }else{
//                 TrackingDeg = sensors.irDeg*2.7;
//         }
//         DriveV = makeXYFromRT(MovingSpeed,TrackingDeg);
// }

INLINE void DecideFrontDegree(bool set){
  #ifdef CAMDIR_SET
        if(sensors.irDeg != -5000 && set == 1) {//abs(sensors.irDeg - camera.OpsDir) < 70
                if (camera.OpsW != 0) {
                        camNone = 0;
                        if (abs(sensors.dir) < 50) {
                                if(abs((int)sensors.irDeg+camera.OpsDir) < 80) {
                                        sensors.turnDir = -camera.OpsX*1.4;
                                }else{
                                        // sensors.turnDir = -camera.OpsX*0.7;
                                        sensors.calDir = sensors.dir;
                                }

                        }

                } else {
                        if (camNone == 1) {
                                if (CAM_Timer.read_ms() > 500) {
                                        camNone = 1;
                                        sensors.calDir = sensors.dir;
                                }
                        } else {
                                CAM_Timer.reset();
                                camNone = 1;
                                if (abs(sensors.dir) < 60) {
                                        sensors.turnDir = -camera.OpsX*1.4;
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


INLINE void Body_Main(){
        getSensors();

        // getSensors();
        if(Line.OutFlag == 1) {ui.SetBZ(0);}else{ui.SetBZ(7); /*0~7（7は音を鳴らさない）*/}//buzzer
        IRv = makeXYFromRT(sensors.irDeg == -5000 ? 0 : 100, sensors.irDeg);
        BallTracking(ui.speed);
        // if(abs(sensors.irDeg) > 91) {
        //         DriveV.x = 0; DriveV.y = 0;
        // }
        // Soccer(ui.speed);
        Line.SetLine(&DriveV,&IRv,&Bound); //フィルタリング処理
        Bound = makeXYFromRT(100,Line.LastRaw); //前回ラインを踏んでいた
        if(camera.OpsY < 67 || camera.OpsY > 95|| camera.OpsW == 0) {
                if(Line.order == 2000) {DriveV.x = -90;}
                if(Line.order == 4000) {DriveV.x = 90;}
        }else{
                if(Line.order == 2000||Line.order == 4000) {
                        DriveV.x *= 0.7;
                        DriveV.y *= 0.7;
                }
        }

        // if(Line.order == 1000 && DriveV.y <= 0) {isHoldTheBallInCorner = 1; Drible_HoldCorner.reset();}
        // if(IS_HOLDING_THE_BALL && DriveV.y <= 0) {
        //         DriveV.y = -70;
        //         if(camera.OpsW != 0) {DriveV.x = 3*camera.OpsX;}
        //         if(abs(camera.OpsX) < 15) {
        //                 ui.SetKICKER(1);
        //         }else{
        //                 ui.SetKICKER(0);
        //         }
        // }
        // }
        // if(DriveV.y <= 0) {
        //
        // }
        MD.driveVector(DriveV,sensors.turnDir);

        // //if(sensors.velocity < 2) {ui.SetBZ(0);}else{ui.SetBZ(7);}
        if(IS_OPS_GOAL_FOUND) {ui.SetLED(0,1,0);}else{ui.SetLED(1,0,0);}
        ui.putc(ui.state);//キッカー、LED、ブザーの情報をなげる

        // pc.printf("irDeg:%d\tTrackingDeg:%d\n",sensors.irDeg,TrackingDeg);
        // pc.printf("Order:%d\tRaw:%d\t",Line.order,Line.Raw);//ラインを踏んだ順番と現在踏んでいる角度
        // pc.printf("OwnX:%d\tOwnY:%d\tOpsX:%d\tOpsY:%d\tOpsDir:%d\tOwnDir:%d\t",camera.OwnX,camera.OwnY,camera.OpsX,camera.OpsY,camera.OpsDir,camera.OwnDir);//カメラ
}



void setup(){
        pc.baud(230400);// パソコン,IRのボーレート
        ui.baud(BAUDRATE);  //UIとのボーレートBAUDRATE

        pidDir.SetMode(AUTOMATIC);
        pidDir.SetOutputLimits(-100,100);
        pidDir.SetSampleTime(9);  //5


        ui.speed = 100;  //デフォルトスピード

        camera.useCamDirection = 1;


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
        dribOn.start();
        Drible_HoldBackTimer.start();
        Drible_HoldOwnSide.start();
        MissingTheBall.start();
        Drible_Stack.start();

        camNone = 0;
        // wait_ms(1000);
        // wait_ms(1000);
        // drib(PB_10);
        // DRIB.write(0.0);  wait_ms(1000);
        // DRIB.write(1.0);  wait_ms(1000);
        // DRIB.write(0.0);  wait_ms(1000);

        // for (size_t theta = 0; theta < 360; theta++) {
        //         for (size_t i = 0; i < 16; i++) {
        //                 DotStar.set(255*sin32_T(theta),255*sin32_T(theta+120),255*sin32_T(theta-120),i);
        //
        //         }
        //         DotStar.write(31);
        //         wait_ms(3);
        // }



}

time_t read_rtc(void){
        return 0;
}

int main() {
        attach_rtc(&read_rtc,NULL,NULL,NULL);
        setup();

        // dribIRQ0r(); wait_ms(1000); dribOnTime.detach(); dribOffTime.detach();
        // dribIRQ0l(); wait_ms(500); dribOnTime.detach(); dribOffTime.detach();
        // dribIRQ0r(); wait_ms(1000); dribOnTime.detach(); dribOffTime.detach();

        // dribIRQ0l();
        dribleState(0); wait_ms(300);
        dribleState(1); wait_ms(300);
        dribleState(0); wait_ms(100);

        pc.printf("\r\nHello world!! LEGEND\r\n");
        pc.printf("This STM32 works with %ld MHz clock\r\n", (SystemCoreClock / 1000000));
        while(1) {
                ui.Run();//
            //     pc.printf("\r\nHello world!! LEGEND\r\n");
                // dribleState(1);
                // drib.write(1.0);
        }
}
