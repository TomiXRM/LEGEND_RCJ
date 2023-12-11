#include "mbed.h"
#include "setup_com.h"
#include "setup.h"

//prototypes
INLINE void getSensors();
INLINE void sensorMonitor();
INLINE void Body_Main();
INLINE void Body_Debug();

Timer moveBackTimer;
Timer goalDegTimer;


char goalDegTweekFlag;

//Functions
INLINE void getSensors(){
        sensors.irRecieved = IR.get();  //取得開始
        cam.get();                      //取得開始
        sensors.dir = (0.5-IMU.read()* -360)-180;//向いている角度の取得
        sensors.PhotoSensor = Photo.read_u16();
        if(abs(sensors.dir) < 4) {sensors.dir = 0;}//正面の微調整
        pidDir.Compute();
        sensors.irDegLast  = sensors.irDeg;//前回の角度
        sensors.irDistLast = sensors.irDist;//前回の距離
        sensors.irDeg  = IR.getDir();//角度代入
        sensors.irDist = IR.getDist();//距離代入
        sensors.irDegTweak = sensors.irDeg;//計算ようの変数に代入
        if(abs(sensors.irDeg  - sensors.irDegLast ) <= 23) { sensors.irDegSame = 1;}else{sensors.irDegSame = 0;}//前回と一緒判定
        if(abs(sensors.irDist - sensors.irDistLast) <=  5) {sensors.irDistSame = 1;}else{sensors.irDistSame = 0;}//前回と一緒判定
        IRv = makeXYFromRT(sensors.irDeg == 5000 ? 0 : 100, sensors.irDeg);//角度をベクトルに
        cam.recognizeCheck();//ゴールが見えているかいないか
        camera.OwnX = cam.OwnX;//自陣ゴールX座標
        camera.OwnY = cam.OwnY;//自陣ゴールY座標
        camera.OpsX = cam.OpsX;//敵陣ゴールX座標
        camera.OpsX = cam.OpsY;//敵陣ゴールY座標
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
        sensors.calDir = sensors.dir;//更新します。
        //↓使う角度をカメラと方位センサから選ぶ
        /*if(cam.OpsRecognize == 1) {//相手ゴールが見えている時
                if(abs(sensors.dir) < 70) {
                        sensors.calDir = camera.OpsDir;
                        camera.useCamDirection = 1;
                }else{
                        sensors.calDir = sensors.dir;
                        camera.useCamDirection = 0;
                }
                directionOpsCtrl.reset();
           }else{
                if(directionOpsCtrl.read_ms() > 300) {
                        camera.useCamDirection = 0;//前入れたカメラの角度が入っているので更新しない
                }else{
                        sensors.calDir = sensors.dir;//更新します。
                        camera.useCamDirection = 1;
                }
           }   //*/
}
INLINE void sensorMonitor(){
        pc.printf("Front:%d Right:%d Back:%d Left:%d\t",Line.front,Line.right,Line.back,Line.left);//ラインが触れているか
        pc.printf("Order:%d\tRaw:%d\t",Line.order,Line.Raw);//ラインを踏んだ順番と現在踏んでいる角度
        pc.printf("IMUDir:%d\t",(short)(sensors.dir));
        pc.printf("IRrecv:%d \tIRdeg:%d \tIRdist%d\tIRx:%d\tIRy:%d\t",sensors.irRecieved,sensors.irDeg,sensors.irDist,((short)(IRv.x)),((short)(IRv.y)));//IR
        pc.printf("OwnX:%d\tOwnY:%d\tOpsX:%d\tOpsY:%d\tOpsDir:%d\tOwnDir:%d\t",camera.OwnX,camera.OwnY,camera.OpsX,camera.OpsY,camera.OpsDir,camera.OwnDir);//カメラ
        pc.printf("OwnDist:%d\tOpsDist%d\t",camera.OwnDist,camera.OpsDist);
        pc.printf("OwnRightEnd:%d\tOwnLeftEnd:%d\t",camera.OwnGoalRightEnd,camera.OwnGoalLeftEnd);
        pc.printf("\r\n");
}
INLINE void BallTracking(uint8_t MovingSpeed){
        if(sensors.irDeg != -5000) {
                if(abs(sensors.irDeg) <= 40) {
                        //TrackingDeg = IR.adjustment(sensors.irDeg,70,60,30,0,0);
                        if(sensors.irDist < 140) {
                                TrackingDeg = sensors.irDeg*1.5;
                        }else{
                                TrackingDeg = sensors.irDeg*2.7;
                        }
                        if(abs(sensors.irDeg)<30 && sensors.irDist > 160 && sensors.PhotoSensor < 61000) {
                                if(camera.OpsW != 0) {
                                        if(camera.OpsX >20) {
                                                goalDegTweekFlag = 1;
                                                sensors.turnDir += 50;
                                        }else if(camera.OpsX < -20) {
                                                goalDegTweekFlag = 2;
                                                sensors.turnDir -= 50;
                                        }else{
                                                goalDegTweekFlag = 0;
                                        }
                                }else{
                                        if(goalDegTimer.read_ms() < 200) {
                                                if(goalDegTweekFlag == 1) {
                                                        sensors.turnDir += 50;
                                                }else if(goalDegTweekFlag == 2) {
                                                        sensors.turnDir -= 50;
                                                }
                                        }else{
                                                goalDegTweekFlag = 0;
                                        }
                                }
                                ui.SetKICKER(1);
                        }else{
                                ui.SetKICKER(0);
                        }
                }else{
                        if(sensors.irDeg > 0) {
                                //TrackingDeg = pow(abs(sensors.irDeg),1.3);
                                TrackingDeg = (sensors.irDeg+30)+asin(90/(255 - sensors.irDist))/PI*180.0;
                        }else{
                                //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                                TrackingDeg = (sensors.irDeg-30)-asin(90/(255 - sensors.irDist))/PI*180.0;
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
                if(abs(sensors.irDeg) <= 90 &&abs(sensors.irDeg) > 22 && sensors.irDist < 130) {
                        if(sensors.irDeg > 0) {
                                //TrackingDeg = pow(abs(sensors.irDeg),1.3);
                                DriveV.x = -140;
                        }else{
                                //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                                DriveV.x = 140;
                        }
                }
                moveBackTimer.reset();
        }else{
                if(camera.OwnW != 0) {
                        DriveV.x = camera.OpsX*5;
                }else{
                        DriveV = SetZero();
                }
                if(moveBackTimer.read_ms() < 500) {
                        DriveV.y=-50;
                }

        }
}
INLINE void Body_Main(){
        getSensors();
        IRv = makeXYFromRT(sensors.irDeg == -5000 ? 0 : 100, sensors.irDeg);
        BallTracking(ui.speed);
        Bound = makeXYFromRT(100,Line.LastRaw);   //前回ラインを踏んでいた
        Line.SetLine(&DriveV,&IRv,&Bound);   //フィルタリング処理
        MD.driveVector(DriveV,sensors.turnDir);
        if(Line.OutFlag == 1) {ui.SetBZ(0);}else{ui.SetBZ(7); /*0~7（7は音を鳴らさない）*/}//buzzer
        ui.putc(ui.state);//キッカー、LED、ブザーの情報をなげる
        pc.printf("irDeg:%d\tTrackingDeg:%d\n",sensors.irDeg,TrackingDeg);
        // pc.printf("Order:%d\tRaw:%d\t",Line.order,Line.Raw);//ラインを踏んだ順番と現在踏んでいる角度
        // pc.printf("OwnX:%d\tOwnY:%d\tOpsX:%d\tOpsY:%d\tOpsDir:%d\tOwnDir:%d\t",camera.OwnX,camera.OwnY,camera.OpsX,camera.OpsY,camera.OpsDir,camera.OwnDir);//カメラ
}
INLINE void Body_Debug(){
        //Body_Main();
        getSensors();
        MD.run(camera.OpsX, camera.OpsX, camera.OpsX, camera.OpsX);
        //sensorMonitor();

}


void setup(){
        pc.baud(230400);// パソコン,IRのボーレート
        ui.baud(BAUDRATE);//UIとのボーレート
        pidDir.SetMode(AUTOMATIC);
        pidDir.SetOutputLimits(-100,100);
        pidDir.SetSampleTime(9);//5

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
        ui.Entry(SPEED,callback(&Before_Speed),callback(&Body_Speed),DONOTHING);


        moveBackTimer.start();
        goalDegTimer.start();

}


int main() {
        setup();
        pc.printf("\r\nHello world!! LEGEND\r\n");
        pc.printf("This STM32 works with %d MHz clock\r\n", (SystemCoreClock / 1000000));
        while(1) {
                ui.Run();
        }
}
