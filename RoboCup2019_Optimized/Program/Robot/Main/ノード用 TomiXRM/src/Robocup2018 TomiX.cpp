
#include "mbed.h"
#include "setup.h"
//#include "LGKicker.h"

#define LINECHECK 500

DigitalOut IMU_Chenge(PC_10);
LineManage Line(D2,D4,PC_4,PB_14);
float cum;
uint8_t cumf;

/*
   nc  kicker  NC  kicker
   nc  photo   NC  PHOTO
   PW4 PW1     PW4 PW1
   MD4 MD1     MD4 MD1
   PW2 LI5     PW2 LI5
   MD2 LI1     MD2 LI1
   PW3 LI2     PW3 LI2
   MD3 LI3     MD3 LI3
 +5V LI4     +5V LI4
   GND 3V3     GND 3V3
 */
/*memo
   ・割り込み関数ないで変更される変数はvolatileをつけて宣言すること
 */
//Timer T1;



Timer Tikicker;
Ticker kicker;
Ticker kicker2;

Timer Debug;

Timer TrackingOut;

//PwmOut sp(PA_11);
/****************************************VARIABLE****************************************/
bool kicked = 0;
int movingdeg = 0;
struct {
        short IRangle;
        short IRangle_cal;
        short IRangleLast;
        bool IRangleSame;
        short IRangleSameTime;
        short IRlong;
        short IRreceive;
        float dir;
        float ballx;
        float ballxOut;
        short USright;
        short USleft;
        short save_IRangle;
        short balldeg_differential;
        bool IMUS;
        float Output;
} sensors;

PID pid(&sensors.dir, &sensors.Output,0, 1.4, 0, 0.04,DIRECT);
PID pidBallx(&sensors.ballx, &sensors.ballxOut,0, 1.4, 0, 0.04,DIRECT);
VectorXY vector;
VectorXY IRv;
VectorXY Bound;

/*********************************************************kicker*******************************************************/
void kicker_reset(void) {
        if (solenoid == 1) {
                solenoid = 0;
                kicker.detach();
        }
}
void kick(void) {
        solenoid = 1;
        kicker2.detach();
        kicker.attach(&kicker_reset, 0.02);
        Tikicker.start();
        Tikicker.reset();
        kicked = 0;
}
void kicker_enable(void){
        if (Photo.read() < 0.55) {
                if (Tikicker.read_ms() > 150) {
                        if (kicked == 0) {
                                kicker2.attach(&kick, 0.05);
                                kicked = 1;
                        }
                }
        } else {
                solenoid = 0;
        }
}
/*********************************************************Ball*******************************************************/
void Ball_Check(){
        sensors.IRreceive = Ball.get();
        sensors.IRangle = Ball.getDir();
        sensors.IRlong = Ball.getDist();
        sensors.ballx = (US1.read()*1000);
        pc.printf("received data:%d\t angle:%d\t deg:%d\t long:%d \tBallX:%d\t Photo:%d\t",sensors.IRreceive, sensors.IRangle,Ball.adjustment(60,abs(sensors.IRangle)>120 ? 70 : 30,30,20,0),sensors.IRlong,(short)sensors.ballx,(short)(Photo.read() *1000));
}
/*********************************************************IMU*******************************************************/
void IMU_Check(){
        sensors.dir = (0.5-US3.read()* -180)-90;
        pc.printf("degree:%d\t",sensors.dir);
}
void US_Check(){
        //sensors.USright = (short)(US1.read() *200);
        sensors.USleft = (short)(53.444*pow(double(US2.read()),-1.736));
        sensors.USright = (short)(53.444*pow(double(US1.read()),-1.736));
}
/*********************************************************BallTracking*******************************************************/
INLINE void BallTracking(uint8_t Pow){
        if(sensors.IRangle > 33) {
                movingdeg = Ball.adjustment(70,40,30,20,0);
        }else if(sensors.IRangle < -33) {
                movingdeg = Ball.adjustment(70,40,30,20,0);
        }else{
                movingdeg = Ball.adjustment(20,5,2,0,0);
        }
        if (sensors.IRangle != 5000) {
                vector = makeXYFromRT((float)Pow,movingdeg);
        }  else{
                vector = SetZero();
        }
}
INLINE void Soccer(){
        sensors.IRangleLast = sensors.IRangle_cal;//記憶
        sensors.IRreceive = Ball.get();
        sensors.IRangle_cal = Ball.getDir();
        sensors.IRlong = Ball.getDist();
        sensors.ballx = cos(sensors.IRangle / 180.0 * PI)*100;
        sensors.IRangle = sensors.IRangle_cal;

        //周り込みを深くするやつ
/*
        if(abs(sensors.IRangle_cal - sensors.IRangleLast) <= 30) {
                sensors.IRangleSame = 1;
        }else{
                sensors.IRangleSame = 0;
        }
        if(sensors.IRangleSame == 1) {
                sensors.IRangleSameTime = TrackingOut.read_ms();
        }else{
                TrackingOut.reset();
                sensors.IRangleSameTime = 0;
        }
        if(sensors.IRangleSameTime > 20000 || sensors.IRangleSameTime) {
                sensors.IRangleSameTime = 1001;
        }
        if(sensors.IRangleSameTime > 1000) {
                if(sensors.IRangle < 0) {
                        sensors.IRangle -= 90;
                }else if(sensors.IRangle > 0) {
                        sensors.IRangle += 90;
                }
        }*/

        IRv = makeXYFromRT(sensors.IRangle == 6000 ? 0 : 100, sensors.IRangle);
        Bound = makeXYFromRT(100,Line.LastRaw);//前回ラインを踏んでいた
        BallTracking(130);//ボールの方向へ動くようにフィルタリング
        if(abs(sensors.IRangle)<90&&sensors.IRlong <= 2&&vector.y < 0) {
                vector.y *=0.7;
        }
        //キーパー向けに変更したところ
        //ここから
        //if (vector.y > 30) vector.y = 0; //前進を許さない
        /*if(IRv.x > 0) {//ボールが左右どちらにあるか
                vector.x = 140;//右なら強制的にXを140%に
           }else if(IRv.x < 0) {
                vector.x = -140;//左なら強制的にXを-140%に
           }*/
        cum = (US4.read()*1000);//カメラ読み取り
/*
        //ゴールの位置による動きの変更
        if(cum > 900) {
                //真ん中
                cumf = 0;//どちら側にいたのか（フラグ）
        }else if(cum > 700&&cum < 900) {
                //右
                if(vector.x > 0) {vector.x = 0;}//これ以上右に動こうとするなら0%にする
                if(sensors.IRangle == 5000) {vector.x =-100;}
                cumf = 1;//どちら側にいたのか（フラグ）
        }else if(cum > 300&&cum < 700) {
                //左
                if(vector.x < 0) {vector.x = 0;}//これ以上左に動こうとするなら0%にする
                if(sensors.IRangle == 5000) {vector.x =100;}
                cumf = 2;//どちら側にいたのか（フラグ）
        }else{
                //ゴールがない時
                if(cumf == 0) {//どちら側でもない
                        //前進
                        vector.y = 90;
                }else if(cumf == 1) {
                        //左前
                        vector.x = -45;
                        vector.y = 90;
                }else if(cumf == 2) {
                        //右前
                        vector.x = 45;
                        vector.y = 90;
                }
        }
        //ここまで*/
        Line.SetLine(&vector,&IRv,&Bound);//フィルタリング処理
        IMU_Chenge = sensors.IMUS;
        sensors.dir = (0.5-US3.read()* -180)-90;
        pid.Compute();
        pidBallx.Compute();
        MD.driveVector(vector,sensors.Output);
        wait_ms(1);
}

INLINE void debug(){
        pc.printf("Order:%d\tRaw%d\tTime%d\tBall:%d\tBx:%d\tBy:%d\tMPU:%d\tdir:%d\tcum:%d\tsametime:%d\tBallLastNowDif:%d",Line.order,Line.Raw,Line.T,sensors.IRangle,((short)(IRv.x)),((short)(IRv.y)),(short)(US3.read()*1000),(int)sensors.dir,(short)cum,sensors.IRangleSameTime,abs(sensors.IRangle_cal - sensors.IRangleLast));
        pc.printf("\r\n");
}
/*********************************************************main*******************************************************/
void setup(){
        pc.baud(230400);
        ballserial.baud(230400);
        IMU.frequency(230400);
        IMU.Setinterval(200000); // 50ms
        MD.frequency(1.0/100000);
        SW.mode(PullUp);
        LINE1.mode(PullDown);
        LINE2.mode(PullDown);
        LINE3.mode(PullDown);
        LINE4.mode(PullDown);
        Tikicker.start();

        pid.SetMode(AUTOMATIC);
        pid.SetOutputLimits(-60,60);
        pid.SetSampleTime(9);//5

        pidBallx.SetMode(AUTOMATIC);
        pidBallx.SetOutputLimits(-100,100);
        pidBallx.SetSampleTime(5);//5
        MD.tweak(101, 100, 100,100);

        wait(1);
        pc.printf("Hello world!! LEGENDα TomiXRM\r\n");
        pc.printf("This computer is %d MHz clock\r\n", (SystemCoreClock / 1000000));

        Debug.start();
        TrackingOut.start();
}

void main2(){
        MD.drive(90,100,sensors.Output);
        sensors.dir = (0.5-US3.read()* -180)-90;
        pid.Compute();
}


int main(void){
        setup();
        MD.frequency(1.0/100000);
        MD.stop(0);
        while(1) {
                led1 = Line.front;
                led2 = Line.right;
                led3 = Line.back;
                led4 = Line.left;

                if(SW == 0) {
                        if(Debug.read_ms() > 30) {
                                debug();
                                Debug.reset();
                        }
                        Soccer();
                        //main2();
                }else{
                        //Debug.detach();
                        MD.stop(0);
                        vector = SetZero();
                        kicker_enable();
                        //wait_ms(10);

                        US_Check(); Ball_Check();
                        pc.printf("USright:%d\tUSleft:%d\r\n",sensors.USright,sensors.USleft);

                }

        }
}
