#include "mbed.h"
#include "setup_com.h"
#include "setup.h"
float p1,p2,p3;
//prototypes
INLINE void getSensors();
INLINE void sensorMonitor();
INLINE void Body_Main();
INLINE void Body_Debug();
INLINE void BallTracking(uint8_t MovingSpeed);
INLINE void GoBackToHome(uint8_t MovingSpeed);
float deg;
int power;
bool ShouldBackFlag;
bool kickerflag;

char testP;

Ticker UIPrint;



//Functions
INLINE void getSensors(){
        sensors.irRecieved = IR.get();  //取得開始
        cam.get();                      //取得開始
        IMU.get();
        //sensors.dir = (0.5-CN4.read()* -360)-180;//向いている角度の取得
        // if(abs(sensors.dir) < 4) {sensors.dir = 0;}
        sensors.PhotoSensorFront = CN2.read_u16();
        sensors.PhotoSensorBack = CN3.read_u16();
        sensors.irDegLast  = sensors.irDeg;//前回の角度
        sensors.irDistLast = sensors.irDist;//前回の距離
        sensors.irDeg  = IR.getDir();//角度代入
        sensors.irDist = IR.getDist();//距離代入
        sensors.EchoRight = US_R.Get();
        sensors.EchoLeft = US_L.Get();
        sensors.DistB = sensors.EchoRight + sensors.EchoLeft;
        sensors.irDegTweak = sensors.irDeg;//計算ようの変数に代入
        if(abs(sensors.irDeg  - sensors.irDegLast ) <= 23) { sensors.irDegSame = 1;}else{sensors.irDegSame = 0;}//前回と一緒判定
        if(abs(sensors.irDist - sensors.irDistLast) <=  1) {sensors.irDistSame = 1;}else{sensors.irDistSame = 0;}//前回と一緒判定
        //IRv = makeXYFromRT(sensors.irDeg == 5000 ? 0 : 100, sensors.irDeg);//角度をベクトルに
        sensors.DistB = 5.3444 * pow(double(CN4.read()), -1.736);

        IRv = makeXYFromRT(sensors.irDeg == -5000 ? 0 : 100, sensors.irDeg);//角度をベクトル
        cam.recognizeCheck();//ゴールが見えているかいないか
        //if(ui.OwnGoal == YELLOW) {
        camera.OwnX = cam.OwnX;        //自陣ゴールX座標
        camera.OwnY = cam.OwnY;        //自陣ゴールY座標
        camera.OwnW = cam.OwnW;        //自陣ゴール幅
        camera.OwnDir = cam.calDegree(0);        //自陣ゴールの角度
        camera.OwnDist = sqrt(camera.OwnX*camera.OwnX+camera.OwnY*camera.OwnY);        //敵陣ゴールの距離(カメラの値上)
        camera.OwnGoalRightEnd = cam.OwnGoalRightEnd;
        camera.OwnGoalLeftEnd = cam.OwnGoalLeftEnd;
        /*}else if(ui.OwnGoal == BLUE) {
           camera.OwnX = cam.OpsX;        //自陣ゴールX座標
           camera.OwnY = cam.OpsY;        //自陣ゴールY座標
           camera.OwnW = cam.OpsW;        //自陣ゴール幅
           camera.OwnDir = cam.calDegree(1);        //自陣ゴールの角度
           camera.OwnDist = sqrt(camera.OwnX*camera.OwnX+camera.OwnY*camera.OwnY);        //敵陣ゴールの距離(カメラの値上)
           camera.OwnGoalRightEnd = cam.OpsGoalRightEnd;
           camera.OwnGoalLeftEnd = cam.OpsGoalLeftEnd;
           }*/
        sensors.dir = (float)IMU.dir;
        sensors.velocity = IMU.velocity;//速度
        sensors.calDir = sensors.dir;
/*
        camera.OpsX = cam.OpsX;//敵陣ゴールX座標
        camera.OpsY = cam.OpsY;//敵陣ゴールY座標
        camera.OpsW = cam.OpsW;//敵陣ゴール幅
        camera.OpsDir = cam.calDegree(1);//自陣ゴールの角度
        camera.OpsDist = sqrt(camera.OpsX*camera.OpsX+camera.OpsY*camera.OpsY);//敵陣ゴールの距離(カメラの値上)
        //カメラで幅と端を書き込むプログラム
        camera.OpsGoalRightEnd = cam.OpsGoalRightEnd;
        camera.OpsGoalLeftEnd = cam.OpsGoalLeftEnd;
 */

        pidDir.Compute();

}

INLINE void sensorMonitor(){
        pc.printf("\r\n");
}

INLINE void GoBackToHome(uint8_t MovingSpeed){
        if(camera.OwnW != 0) {

                DriveV = makeXYFromRT((float)MovingSpeed,camera.OwnDir);

        }else{
                DriveV = makeXYFromRT((float)MovingSpeed,180);
        }


}

INLINE void Keeper(uint8_t MovingSpeed) {
        if(camera.OwnW != 0) {
                if(camera.OpsGoalRightEnd < 0) {
                        ui.SetBZ(3);
                        DriveV.x = -4*camera.OpsGoalRightEnd;
                        DriveV.y =0;
                }else if(camera.OpsGoalLeftEnd >0) {
                        ui.SetBZ(3);
                        DriveV.x = -4*camera.OpsGoalLeftEnd;
                        DriveV.y = 0;
                }else{
                        BallTracking(ui.speed);
                }
        }
}


INLINE void BallTracking(uint8_t MovingSpeed){
        ui.SetKICKER(0);
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
                        DriveV.y = -30;
                }
        }
}


INLINE void Prog1(uint8_t MovingSpeed){
        if(sensors.irDeg != -5000) {
                DriveV = makeXYFromRT((float)MovingSpeed,sensors.irDeg);
        }else{
                DriveV.x = 0; DriveV.y = 0;
        }
}
INLINE void Body_Main(){
        ui.SetBZ(7);
        getSensors();
        DriveV = SetZero();
        //BallTracking(ui.speed);
        //Keeper(ui.speed);
        //GoBackToHome(ui.speed);
        Prog1(ui.speed);
        //MD.run(40,40,40);
        Bound = makeXYFromRT(100,Line.Raw);//前回ラインを踏んでいた

        Line.SetLine(&DriveV,&IRv,&Bound);//フィルタリング処理

        pc.printf("%d\t%d\r\n",camera.OwnGoalRightEnd,camera.OwnGoalLeftEnd);
        //MD.run(-sensors.calDir,-sensors.calDir,-sensors.calDir);

        MD.driveVector(DriveV,sensors.turnDir);
        //wait_ms(3);
}
/******************************************************************************
   testP++;
   //deg++;
   if (testP<50) {
        MD.run(20,-20,0);

   }else if (testP<=100) {
        MD.run(-20,20,0);

   }else{
        testP = 0;
   }
   wait_ms(50);

   deg = 0;
   power=100;
   p1 = (sin((deg-60) * PI / 180) * power);
   p2 = (sin((deg+60) * PI / 180) * power);
   p3 = (sin((deg+180) * PI / 180) * power);
   MD.run(p1,p2,p3);
******************************************************************************/


INLINE void Body_Debug(){
        getSensors();
        Bound = makeXYFromRT(100,Line.Raw);//前回ラインを踏んでいた
        pc.printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n",camera.OwnGoalRightEnd,camera.OwnGoalLeftEnd,sensors.EchoRight,sensors.EchoLeft,sensors.DistB,sensors.PhotoSensorBack,sensors.PhotoSensorFront);
        //pc.printf("%d\t%d\r\n",sensors.irDeg,sensors.irDist);
        wait_ms(10);
}
void UI_Print(){
        ui.putc(ui.state);//キッカー、LED、ブザーの情報をなげる
}

void initUIPrint(){
        UIPrint.attach_us(&UI_Print,100);
}

void desrUIPrint(){
        UIPrint.detach();
}


void setup(){
        pc.baud(230400);// パソコン,IRのボーレート
        ui.baud(BAUDRATE);  //UIとのボーレートBAUDRATE

        pidDir.SetMode(AUTOMATIC);
        pidDir.SetOutputLimits(-70,70);
        pidDir.SetSampleTime(9);  //5


        ui.speed = 100;  //デフォルトスピード
        camera.useCamDirection = 1;
        //_imu_.mode(PullDown);

        wait(1);
        //Mode enrties.
        ui.Entry(MAIN,callback(&initUIPrint),callback(&Body_Main),callback(&desrUIPrint));  //callback(&stopAll)
        ui.Entry(DEBUG,DONOTHING,callback(&Body_Debug),DONOTHING);

        initSpeedProcess();
        initBallProcess();
        initCompassProcess();
        initLineProcess();
        initCameraProcess();
        initUsProcess();
        initOwnGoalProcess();

        ui.Entry(SPEED,callback(&Before_Speed),callback(&Body_Speed),DONOTHING);
}


int main() {
        setup();
        pc.printf("\r\nHello world!! LEGEND\r\n");
        pc.printf("This STM32 works with %d MHz clock\r\n", (SystemCoreClock / 1000000));
        while(1) {
                //MD.run(-40,-40,-40);
                ui.Run();
                // Body_Main();
        }
}
