#include "mbed.h"
#include "setup_com.h"
#include "setup.h"



#define CAMDIR_SET
#define IS_BALL_FOUND (sensors.irDeg != -5000)
#define IS_BALL_IN_FRONT_OF_THE_GOAL (abs(sensors.irDeg) <= 30)
#define IS_HOLDING_THE_BALL (abs(sensors.irDeg)<30 && sensors.irDist > 160 && sensors.PhotoSensor1 < PHOTO1_THRESHOLD)
#define IS_OPS_GOAL_FOUND (camera.OpsW != 0)
#define IS_OWN_GOAL_FOUND (camera.OwnW != 0)


#define PHOTO1_THRESHOLD 6000
#define PHOTO2_THRESHOLD 20000
// #define dribRER
//prototypes
INLINE void Body_Main();
INLINE void DecideFrontDegree(bool set);

INLINE void GotToCenter();
INLINE void drible(uint8_t MovingSpeed);


Timer moveBackTimer;
Timer goalDegTimer;
Timer BallHoldingT;
Timer Drible_HoldBackTimer;
Timer Drible_HoldOwnSide;
Timer Drible_HoldCorner;

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

INLINE void BallTracking(uint8_t MovingSpeed){

        while(ui.mode != MODESW) {
                ui.SetBZ(7);
                getSensors();
                ui.SetKICKER(0);
                if(IS_BALL_FOUND) {
                        if(IS_HOLDING_THE_BALL) {
                                if(camera.OpsW != 0) {
                                        ui.SetLED(0,1,0);
                                        if(abs(sensors.dir) > 90) {
                                                if(abs(sensors.dir) < 5) {
                                                        MD.run(0,0,0,0);
                                                }else{
                                                        if(sensors.dir < 0) {
                                                                MD.run(15,15,15,15);
                                                        }else{
                                                                MD.run(-15,-15,-15,-15);
                                                        }
                                                        BallHoldingT.reset();
                                                }
                                                ui.SetBZ(3);
                                        }else{
                                                // MD.drive(0, 100,-camera.OpsX);
                                                if(abs(camera.OpsDir) < 5) {
                                                        MD.drive(0,0,-camera.OpsX);
                                                        // wait(5);
                                                        // dribleState(0);
                                                        wait_ms(500);
                                                        if(  BallHoldingT.read_ms() < 300) {
                                                                ui.SetKICKER(0);
                                                        }else{
                                                                // MD.drive(0,50,-camera.OpsX);
                                                                // wait_ms(200);
                                                                MD.drive(0,150,0);
                                                                wait_ms(40);
                                                                dribleState(1);
                                                                wait_ms(30);
                                                                ui.SetKICKER(1);
                                                        }
                                                        ui.SetKICKER(1);
                                                }else{
                                                        if(camera.OpsDir < 0) {
                                                                MD.run(-15,-15,-15,-15);
                                                        }else{
                                                                MD.run(15,15,15,15);
                                                        }
                                                        BallHoldingT.reset();
                                                }

                                                // MD.run(camera.OpsDir,camera.OpsDir,camera.OpsDir,camera.OpsDir);
                                                // DecideFrontDegree(1);
                                                // MD.run(sensors.turnDir,sensors.turnDir,sensors.turnDir,sensors.turnDir);
                                                ui.SetBZ(1);
                                        }


                                }else{
                                        ui.SetLED(1,0,0);
                                        MD.run(0,0,0,0);
                                }

                                wait_ms(10);
                        }else{
                                // MD.run(sensors.irDeg,sensors.irDeg,sensors.irDeg,sensors.irDeg);
                                if(abs(sensors.dir) < 115) {
                                        MD.drive(0, IRv.x,sensors.irDeg);
                                }else{
                                        MD.drive(0, 0,sensors.irDeg);
                                }

                                dribleState(1);
                        }
                }else{
                        DecideFrontDegree(0);
                        dribleState(0);
                        // MD.run(sensors.turnDir,sensors.turnDir,sensors.turnDir,sensors.turnDir);
                        DecideFrontDegree(1);
                        MD.run(sensors.turnDir,sensors.turnDir,sensors.turnDir,sensors.turnDir);
                }
                ui.putc(ui.state);//キッカー、LED、ブザーの情報をなげる
        }
}

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
        Bound = makeXYFromRT(100,Line.LastRaw); //前回ラインを踏んでいた
        // if(camera.OpsY < 70 || camera.OpsY > 90|| camera.OpsW == 0) {
        //         if(Line.order == 2000) {DriveV.x = -90;}
        //         if(Line.order == 4000) {DriveV.x = 90;}
        // }else{
        //         if(Line.order == 2000||Line.order == 4000) {
        //                 DriveV.x *= 0.7;
        //                 DriveV.y *= 0.7;
        //         }
        // }
        Line.SetLine(&DriveV,&IRv,&Bound); //フィルタリング処理
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
        if(camera.OpsW != 0) {ui.SetLED(0,1,0);}else{ui.SetLED(1,0,0);}
        ui.putc(ui.state);//キッカー、LED、ブザーの情報をなげる




        // pc.printf("irDeg:%d\tTrackingDeg:%d\n",sensors.irDeg,TrackingDeg);
        // pc.printf("Order:%d\tRaw:%d\t",Line.order,Line.Raw);//ラインを踏んだ順番と現在踏んでいる角度
        // pc.printf("OwnX:%d\tOwnY:%d\tOpsX:%d\tOpsY:%d\tOpsDir:%d\tOwnDir:%d\t",camera.OwnX,camera.OwnY,camera.OpsX,camera.OpsY,camera.OpsDir,camera.OwnDir);//カメラ
}

void before_Main(){
        // wait(5);/
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
        ui.Entry(MAIN,before_Main,callback(&Body_Main),callback(&after_Main));  //callback(&stopAll)
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



int main() {
        setup();

        // dribIRQ0r(); wait_ms(1000); dribOnTime.detach(); dribOffTime.detach();
        // dribIRQ0l(); wait_ms(500); dribOnTime.detach(); dribOffTime.detach();
        // dribIRQ0r(); wait_ms(1000); dribOnTime.detach(); dribOffTime.detach();

        // dribIRQ0l();
        dribleState(0); wait_ms(300);
        dribleState(1); wait_ms(300);
        dribleState(0); wait_ms(100);

        pc.printf("\r\nHello world!! LEGEND\r\n");
        pc.printf("This STM32 works with %d MHz clock\r\n", (SystemCoreClock / 1000000));
        while(1) {
                ui.Run();//
                // dribleState(1);
                // drib.write(1.0);
        }
}
