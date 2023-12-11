
#include "mbed.h"
#include "setup.h"
//#include "LGKicker.h"

#define LINECHECK 500

DigitalOut IMU_Chenge(PC_10);

/*
   nc  kikker  NC  KIKKER
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

Timer line_wait;

Timer TiLINE;
Ticker lineOP;

Timer Tikikker;
Ticker kikker;
Ticker kikker2;

Timer Debug;

bool didavoid;
//PwmOut sp(PA_11);
/****************************************VARIABLE****************************************/
int a;
bool kicked = 0;
int movingdeg = 0;
short dif;
struct {
        short IRangle;
        short IRlong;
        short IRreceive;
        volatile short dir;
        short USright;
        short USleft;
        short save_IRangle;
        short balldeg_differential;
        bool IMUS;
} sensors;


struct {
        bool front;
        bool right;
        bool back;
        bool left;
        short OnWH1st;
        short OnWH2nd;
        short OnWH3rd;
        short OnWH4th;
        bool Frised;
        bool Rrised;
        bool Brised;
        bool Lrised;
        short Raw;//0~8(0は踏んでない)
        short LastRaw;
        int T;
        uint8_t wt;
} line;
struct {
        short movingdeg;
        short shouldmovedeg;
        volatile short linedeg;
        volatile short order;
        volatile short delegate;
        bool error; // eraorが出たら1
        volatile bool isOUT;
        short lineSensordeg;
        short save_linedeg;
} outofbounds;



typedef struct {
        float x;
        float y;
        float r;
} vector_xy;
/*マグロ定義*/

vector_xy vector;
vector_xy IRv;
vector_xy Bound;

vector_xy setVectorZero() {
        vector_xy rslt;
        rslt.x = 0;
        rslt.y = 0;
        rslt.r = 0;
        return rslt;
}

vector_xy makeXYFromRT(double radius, float theta) {

        vector_xy rslt;

        theta += 90;
        rslt.y = radius * sin(theta / 180.0 * PI);
        rslt.x = radius * cos(theta / 180.0 * PI);

        return rslt;
}

vector_xy makeXY(double x, double y) {

        vector_xy rslt;
        rslt.x = x;
        rslt.y = y;

        return rslt;
}

INLINE void driveVector(vector_xy vec){
        float M1 = vec.y* 0.866 + vec.x * -0.5 + sensors.dir;
        float M2 = vec.y* -0.866 + vec.x * -0.5 + sensors.dir;
        float M3  = vec.y * -0.707 + vec.x * 0.707 + sensors.dir;
        float M4 = vec.y * 0.707 +vec.x * 0.707 + sensors.dir;
        MD.move(M1, M2, M3, M4);
}

/*********************************************************kicker*******************************************************/
void kikker_reset(void) {
        if (solenoid == 1) {
                solenoid = 0;
                kikker.detach();
        }
}
void kick(void) {
        solenoid = 1;
        kikker2.detach();
        kikker.attach(&kikker_reset, 0.02);
        Tikikker.start();
        Tikikker.reset();
        kicked = 0;
}
void kicker_enable(void){
        if (Photo.read() < 0.55) {
                if (Tikikker.read_ms() > 150) {
                        if (kicked == 0) {
                                kikker2.attach(&kick, 0.05);
                                kicked = 1;
                        }
                }
        } else {
                solenoid = 0;
        }
}
/*********************************************************ball*******************************************************/
void Ball_Check(){
        sensors.IRreceive = Ball.read(0);
        sensors.IRangle = Ball.readIRangle();
        sensors.IRlong = Ball.readIRlong();
        pc.printf("received data:%d\t angle:%d\t deg:%d\tlong:%d\tPhoto:%d\t",sensors.IRreceive, sensors.IRangle, Ball.adjustment(60, 30, 0),sensors.IRlong,(short)(Photo.read() *1000));
}
/*********************************************************IMU*******************************************************/
void IMU_Check(){
        sensors.dir = IMU.receive()*3.25;
        led1 = !led1;
        //MD.move(0, 0, 0, 0,sensors.dir);
        pc.printf("%d\r\n", sensors.dir);
}
void US_Check(){
        //sensors.USright = (short)(US1.read() *200);
        sensors.USleft = (short)(53.444*pow(double(US2.read()),-1.736));
        sensors.USright = (short)(53.444*pow(double(US1.read()),-1.736));
}
/*********************************************************BallTracking*******************************************************/

void line_check(){

        outofbounds.isOUT = 0;
        //outofbounds.order = 0;
        line.front = LINE1.read();
        line.right = LINE2.read();
        line.back = LINE3.read();
        line.left = LINE4.read();


        if(line.front == 1 || line.right == 1 || line.back == 1 || line.left == 1) {
                if(line.OnWH1st == 0) {//初回
                        if(line.front == 1) {
                                line.OnWH1st = 1;
                        }
                        if(line.right == 1) {
                                line.OnWH1st = 2;
                        }
                        if(line.back == 1) {
                                line.OnWH1st = 3;
                        }
                        if(line.left == 1) {
                                line.OnWH1st = 4;
                        }
                        line.OnWH2nd = 0;
                        line.OnWH3rd = 0;

                        outofbounds.order = (line.OnWH1st * 1000);
                }else if(line.OnWH2nd == 0) {
                        switch (line.OnWH1st) {
                        case 1:
                                if(line.right == 1) {
                                        line.OnWH2nd = 2;
                                }
                                if(line.back == 1) {
                                        line.OnWH2nd = 3;
                                }
                                if(line.left == 1) {
                                        line.OnWH2nd = 4;
                                }
                                break;
                        case 2:
                                if(line.front == 1) {
                                        line.OnWH2nd = 1;
                                }
                                if(line.back == 1) {
                                        line.OnWH2nd = 3;
                                }
                                if(line.left == 1) {
                                        line.OnWH2nd = 4;
                                }
                                break;
                        case 3:
                                if(line.front == 1) {
                                        line.OnWH2nd = 1;
                                }
                                if(line.right == 1) {
                                        line.OnWH2nd = 2;
                                }
                                if(line.left == 1) {
                                        line.OnWH2nd = 4;
                                }
                                break;
                        case 4:
                                if(line.front == 1) {
                                        line.OnWH2nd = 1;
                                }
                                if(line.right == 1) {
                                        line.OnWH2nd = 2;
                                }
                                if(line.back == 1) {
                                        line.OnWH2nd = 3;
                                }
                                break;
                        }
                        line.OnWH3rd == 0;
                        outofbounds.order = outofbounds.order + (line.OnWH2nd * 100);
                        TiLINE.reset();
                        //TiLINE.start();//2個目と3個目の　時間差　を計測開始
                }else if(line.OnWH3rd == 0) {//踏んでない

                        line.T = TiLINE.read_us();//差を取得
                        TiLINE.reset();
                        if(line.OnWH1st == 1 && line.OnWH2nd == 2) {

                                if(line.back == 1) {
                                        line.OnWH3rd = 3;
                                }
                                if(line.left == 1) {
                                        line.OnWH3rd = 4;
                                }
                        }else if(line.OnWH1st == 1 && line.OnWH2nd == 3) {
                                //エラー
                                if(line.right == 1) {
                                        line.OnWH3rd= 2;
                                }

                                if(line.left == 1) {
                                        line.OnWH3rd = 4;
                                }
                        }else if(line.OnWH1st == 1 && line.OnWH2nd == 4) {

                                if(line.right == 1) {
                                        line.OnWH3rd = 2;
                                }
                                if(line.back == 1) {
                                        line.OnWH3rd = 3;
                                }

                        }else if(line.OnWH1st == 2 && line.OnWH2nd == 1) {

                                if(line.back == 1) {
                                        line.OnWH3rd = 3;
                                }
                                if(line.left == 1) {
                                        line.OnWH3rd = 4;
                                }
                        }else if(line.OnWH1st == 2 && line.OnWH2nd == 3) {

                                if(line.front == 1) {
                                        line.OnWH3rd= 1;
                                }
                                if(line.left == 1) {
                                        line.OnWH3rd = 4;
                                }
                        }else if(line.OnWH1st == 2 && line.OnWH2nd == 4) {
                                //エラー
                                if(line.front == 1) {
                                        line.OnWH3rd= 1;
                                }
                                if(line.back == 1) {
                                        line.OnWH3rd = 3;
                                }

                        }else if(line.OnWH1st == 3 && line.OnWH2nd == 1) {
                                //エラー
                                if(line.right == 1) {
                                        line.OnWH3rd= 2;
                                }

                                if(line.left == 1) {
                                        line.OnWH3rd = 4;
                                }
                        }else if(line.OnWH1st == 3 && line.OnWH2nd == 2) {
                                if(line.front == 1) {
                                        line.OnWH3rd= 1;
                                }
                                if(line.left == 1) {
                                        line.OnWH3rd = 4;
                                }
                        }else if(line.OnWH1st == 3 && line.OnWH2nd == 4) {
                                if(line.front == 1) {
                                        line.OnWH3rd= 1;
                                }
                                if(line.right == 1) {
                                        line.OnWH3rd= 2;
                                }

                        }else if(line.OnWH1st == 4 && line.OnWH2nd == 1) {

                                if(line.right == 1) {
                                        line.OnWH3rd= 2;
                                }
                                if(line.back == 1) {
                                        line.OnWH3rd = 3;
                                }

                        }else if(line.OnWH1st == 4 && line.OnWH2nd == 2) {
                                //エラー
                                if(line.front == 1) {
                                        line.OnWH3rd= 1;
                                }
                                if(line.back == 1) {
                                        line.OnWH3rd = 3;
                                }

                        }else if(line.OnWH1st == 4 && line.OnWH2nd == 3) {
                                if(line.front == 1) {
                                        line.OnWH3rd= 1;
                                }
                                if(line.right == 1) {
                                        line.OnWH3rd= 2;
                                }

                        }
                        outofbounds.order = outofbounds.order + (line.OnWH3rd * 10);
                }else if(line.OnWH1st != 0 && line.OnWH2nd != 0 && line.OnWH3rd != 0) {
                        //3つ全部踏んだ
                }else{
                        //踏んでない
                }
                outofbounds.isOUT = 1;
                line_wait.reset();
        }else{
                if(line.OnWH2nd != 0) {
                        if(line.OnWH3rd != 0) {
                                line.wt = 500;
                        }else{
                                line.wt = 200;
                        }
                }else{
                        line.wt = 1;
                }

                if(line_wait.read_ms() > line.wt) {
                        outofbounds.isOUT = 0;
                        outofbounds.order = 0;
                        line.OnWH1st = 0;
                        line.OnWH2nd = 0;
                        line.OnWH3rd = 0;
                        line.OnWH4th = 0;
                        line.T = 0;
                }
        }

        //LINE RAW
        line.Raw = line.front + (line.right*2) + (line.back * 4) + (line.left * 8);
        //踏んでいても踏んでいなくても処理する
        switch (line.Raw) {

        case 1:    line.Raw =  0;   break;     //F
        case 2:    line.Raw = -90;  break;     //R
        case 3:    line.Raw = -45;  break;     //FR
        case 4:    line.Raw =  180; break;     //B
        case 5:    line.Raw =  360; break;     //error
        case 6:    line.Raw = -135; break;     //RB
        case 7:    line.Raw =  0;   break;     //FRB
        case 8:    line.Raw =  90;  break;      //L
        case 9:    line.Raw =  45;  break;     //FL
        case 10:   line.Raw =  360; break;     //error
        case 11:   line.Raw =  0;   break;      //FRL
        case 12:   line.Raw =  135; break;     //BL
        case 13:   line.Raw =  90;  break;      //FBL
        case 14:   line.Raw =  180; break;     //RBL
        case 15:   line.Raw =  360; break;     //error
        }

}

INLINE void setline(){
        if(outofbounds.isOUT == 1) {
                sensors.IMUS = 1;
                if(outofbounds.order == 1000) {
                        vector.y = -90;
                }else if(outofbounds.order == 1200) {
                        switch (line.Raw) {
                        case 0:
                                vector = makeXY(0,-100); break;
                        case -90:
                                vector = makeXY(-90,-60); break;
                        case -45:
                                vector = makeXY(-90,-60); break;
                        default:
                                vector = makeXY(-90,-90); break;
                        }
                }else if(outofbounds.order == 1230) {
                        switch (line.Raw) {
                        case 0:
                                vector = makeXY(0,-100); break;
                        case 45:
                                vector = makeXY(-90,-90); break;
                        case -45:
                                vector = makeXY(-90,-90); break;
                        case 90:
                                vector = makeXY(-90,60); break;
                        case -90:
                                vector = makeXY(-90,60); break;
                        case 135:
                                vector = makeXY(-90,-90); break;
                        case -135:
                                vector = makeXY(-30,-90); break;
                        case 180:
                                vector = makeXY(0,-100); break;
                        case 360:
                                vector = makeXY(-90,0); break;
                        }
                }else if(outofbounds.order == 1240) {
                        switch (line.Raw) {
                        case 0:
                                vector = makeXY(0,-100); break;
                        case 45:
                                vector = makeXY(90,-90); break;
                        case -45:
                                vector = makeXY(-90,-90); break;
                        case 90:
                                vector = makeXY(90,-60); break;
                        case -90:
                                vector = makeXY(-90,-60); break;
                        case 135:
                                vector = makeXY(30,-90); break;
                        case -135:
                                vector = makeXY(-30,-90); break;
                        case 180:
                                vector = makeXY(0,-120); break;
                        case 360:
                                vector = makeXYFromRT(100, 180); break;
                        }
                }else if(outofbounds.order == 1300) {
                        //error
                        vector.y = 0;
                        vector.x *= 1.2;
                }else if(outofbounds.order == 1320) {
                        if(vector.x > 0) { vector.x = -90; vector.y = 0;}
                }else if(outofbounds.order == 1340) {
                        if(vector.x > 0) { vector.x = -90; vector.y = 0;}
                }else if(outofbounds.order == 1400) {
                        switch (line.Raw) {
                        case 0:
                                vector = makeXY(0,-100); break;
                        case 90:
                                vector = makeXY(90,-60); break;
                        case 45:
                                vector = makeXY(90,-60); break;
                        default:
                                vector = makeXY(90,-90); break;
                        }
                }else if(outofbounds.order == 1420) {
                        switch (line.Raw) {
                        case 0:
                                vector = makeXY(0,-100); break;
                        case 45:
                                vector = makeXY(90,-90); break;
                        case -45:
                                vector = makeXY(-90,-90); break;
                        case 90:
                                vector = makeXY(90,-60); break;
                        case -90:
                                vector = makeXY(-90,-60); break;
                        case 135:
                                vector = makeXY(30,-90); break;
                        case -135:
                                vector = makeXY(-30,-90); break;
                        case 180:
                                vector = makeXY(0,-120); break;
                        case 360:
                                vector = makeXYFromRT(100, 180); break;
                        }
                }else if(outofbounds.order == 1430) {
                        switch (line.Raw) {
                        case 0:
                                vector = makeXY(0,-100); break;
                        case 45:
                                vector = makeXY(90,-90); break;
                        case -45:
                                vector = makeXY(90,-90); break;
                        case 90:
                                vector = makeXY(90,0); break;
                        case -90:
                                vector = makeXY(90,0); break;
                        case 135:
                                vector = makeXY(90,-90); break;
                        case -135:
                                vector = makeXY(30,-90); break;
                        case 180:
                                vector = makeXY(0,-100); break;
                        case 360:
                                vector = makeXY(90,0); break;
                        }
                }else if(outofbounds.order == 2000) {
                        if(vector.x >= 0) {
                                vector.x = -90;
                        }
                }else if(outofbounds.order == 2100) {
                        switch (line.Raw) {
                        case 0:
                                vector = makeXY(0,-100); break;
                        case -90:
                                vector = makeXY(-90,-60); break;
                        case -45:
                                vector = makeXY(-90,-60); break;
                        default:
                                vector = makeXY(-90,-90); break;
                        }
                }else if(outofbounds.order == 2130) {
                        vector.x = -90; vector.y = 0;
                }else if(outofbounds.order == 2140) {
                        vector = makeXY(-90,-90);
                }else if(outofbounds.order == 2300) {
                        switch (line.Raw) {
                        case 180:
                                vector = makeXY(0,90); break;
                        case -90:
                                vector = makeXY(-90,0); break;
                        case -135:
                                vector = makeXY(-60,90); break;
                        default:
                                vector = makeXY(-90,90); break;
                        }
                }else if(outofbounds.order == 2310) {
                        //vector.x = -90; vector.y = 0;
                        switch (line.Raw) {
                        case 0:
                                vector = makeXY(-30,-100); break;
                        case 45:
                                vector = makeXY(-30,-100); break;
                        case -45:
                                vector = makeXY(-90,-90); break;
                        case 90:
                                vector = makeXY(-90,0); break;
                        case -90:
                                vector = makeXY(-90,0); break;
                        case 135:
                                vector = makeXY(-90,90); break;
                        case -135:
                                vector = makeXY(-90,90); break;
                        case 180:
                                vector = makeXY(-30,100); break;
                        case 360:
                                vector = makeXY(-90,0); break;
                        }
                }else if(outofbounds.order == 2340) {
                        switch (line.Raw) {
                        case 0:
                                vector = makeXY(-100,100); break;
                        case 45:
                                vector = makeXY(-30,100); break;
                        case -45:
                                vector = makeXY(0,-90); break;
                        case 90:
                                vector = makeXY(-90,0); break;
                        case -90:
                                vector = makeXY(-90,0); break;
                        case 135:
                                vector = makeXY(0,90); break;
                        case -135:
                                vector = makeXY(-60,90); break;
                        case 180:
                                vector = makeXY(0,100); break;
                        case 360:
                                vector = makeXY(-90,100); break;
                        }
                }else if(outofbounds.order == 2400) {
                        //error
                        vector.x = 0;
                }else if(outofbounds.order == 2410) {
                        vector = makeXY(0,-50);
                }else if(outofbounds.order == 2430) {
                        vector = makeXY(0,50);
                }else if(outofbounds.order == 3000) {
                        if(vector.y <= 0) {vector.y = 90;}
                }else if(outofbounds.order == 3100) {
                        vector.y = 0;
                }else if(outofbounds.order == 3120) {
                        if(vector.x > 0) { vector.x = -90; vector.y = 0;}
                }else if(outofbounds.order == 3140) {
                        if(vector.x < 0) { vector.x = 90; vector.y = 0;}
                }else if(outofbounds.order == 3200) {
                        switch (line.Raw) {
                        case 180:
                                vector = makeXY(0,90); break;
                        case -90:
                                vector = makeXY(-90,0); break;
                        case -135:
                                vector = makeXY(-60,90); break;
                        default:
                                vector = makeXY(-90,90); break;
                        }
                }else if(outofbounds.order == 3210) {
                        if(vector.x > 0) { vector.x = -90; vector.y = 10;}
                }else if(outofbounds.order == 3240) {
                        switch (line.Raw) {
                        case 0:
                                vector = makeXY(0,100); break;
                        case 45:
                                vector = makeXY(0,100); break;
                        case -45:
                                vector = makeXY(0,100); break;
                        case 90:
                                vector = makeXY(-90,0); break;
                        case -90:
                                vector = makeXY(90,0); break;
                        case 135:
                                vector = makeXY(60,90); break;
                        case -135:
                                vector = makeXY(-60,90); break;
                        case 180:
                                vector = makeXY(0,100); break;
                        case 360:
                                vector = makeXY(0,100); break;
                        }
                }else if(outofbounds.order == 3400) {
                        switch (line.Raw) {
                        case 180:
                                vector = makeXY(0,90); break;
                        case 90:
                                vector = makeXY(90,0); break;
                        case 135:
                                vector = makeXY(60,90); break;
                        default:
                                vector = makeXY(90,90); break;
                        }
                }else if(outofbounds.order == 3410) {
                        if(vector.x < 0) { vector.x = 90; vector.y = 10;}
                }else if(outofbounds.order == 3420) {
                        switch (line.Raw) {
                        case 0:
                                vector = makeXY(0,100); break;
                        case 45:
                                vector = makeXY(0,100); break;
                        case -45:
                                vector = makeXY(0,100); break;
                        case 90:
                                vector = makeXY(-90,0); break;
                        case -90:
                                vector = makeXY(90,0); break;
                        case 135:
                                vector = makeXY(60,90); break;
                        case -135:
                                vector = makeXY(-60,90); break;
                        case 180:
                                vector = makeXY(0,100); break;
                        case 360:
                                vector = makeXY(0,100); break;
                        }
                }else if(outofbounds.order == 4000) {
                        if(vector.x <= 0) { vector.x = 90; vector.y = 0;}
                }else if(outofbounds.order == 4100) {
                        switch (line.Raw) {
                        case 0:
                                vector = makeXY(0,-100); break;
                        case 90:
                                vector = makeXY(90,-60); break;
                        case 45:
                                vector = makeXY(90,-60); break;
                        default:
                                vector = makeXY(90,-90); break;
                        }
                }else if(outofbounds.order == 4120) {
                        vector = makeXY(90,-90);
                }else if(outofbounds.order == 4130) {
                        vector.x = 90; vector.y = 0;
                }else if(outofbounds.order == 4200) {
                        vector.x = 0;
                }else if(outofbounds.order == 4210) {
                        if(vector.x > 0) { vector.x = -90; vector.y = 0;}
                }else if(outofbounds.order == 4230) {
                        vector = makeXY(0,50);
                }else if(outofbounds.order == 4300) {
                        switch (line.Raw) {
                        case 180:
                                vector = makeXY(0,90); break;
                        case 90:
                                vector = makeXY(90,0); break;
                        case 135:
                                vector = makeXY(60,90); break;
                        default:
                                vector = makeXY(90,90); break;
                        }
                }else if(outofbounds.order == 4310) {
                        vector.x = 90; vector.y = 0;
                }else if(outofbounds.order == 4320) {
                        switch (line.Raw) {
                        case 0:
                                vector = makeXY(100,100); break;
                        case 45:
                                vector = makeXY(30,100); break;
                        case -45:
                                vector = makeXY(0,-90); break;
                        case 90:
                                vector = makeXY(90,0); break;
                        case -90:
                                vector = makeXY(90,0); break;
                        case 135:
                                vector = makeXY(0,90); break;
                        case -135:
                                vector = makeXY(60,90); break;
                        case 180:
                                vector = makeXY(0,100); break;
                        case 360:
                                vector = makeXY(90,100); break;
                        }
                }
                line_wait.reset();
                line.LastRaw = line.Raw;
        }else{
                if(line_wait.read_ms() < 1000) {
                        sensors.IMUS = 1;
                        /*
                           if(line.LastRaw == 180 && sensors.IRangle < 0) {
                                if(abs((-sensors.IRangle) - line.LastRaw) <= 45) {
                                        vector = setVectorZero();
                                }
                           }else{
                                if(abs(sensors.IRangle - line.LastRaw) <= 45) {
                                        vector = setVectorZero();
                                }
                           }*/
                        if(IRv.x > 0 && Bound.x > 0 && vector.x > 0) {
                                vector.x = 0;
                        }
                        if(IRv.x < 0 && Bound.x < 0 && vector.x < 0) {
                                vector.x = 0;
                        }
                        if(IRv.y > 0 && Bound.y > 0 && vector.y > 0) {
                                vector.y = 0;
                        }
                        if(IRv.y < 0 && Bound.y < 0 && vector.y < 0) {
                                vector.y = 0;
                        }
                }
                //IMU_Chenge = 1;
        }

}


INLINE void BallTracking(uint8_t Pow){
        movingdeg = Ball.adjustment(60, abs(sensors.IRangle)>120 ? 70 : 30, 10);
        if (sensors.IRangle != 5000) {
                vector = makeXYFromRT((float)Pow,movingdeg);
                if(sensors.IRlong == 5 && abs(sensors.IRangle) < 60) {
                        vector.x *= 0.8;
                }
                if(abs(sensors.IRangle) <= 30) {
                        kicker_enable();
                }

                if(abs(sensors.IRangle) <= 60) {
                        sensors.IMUS = 0;
                }else{
                        sensors.IMUS = 1;
                }
        }
        else{
                vector = setVectorZero();
        }

}
INLINE void debug(){
        pc.printf("Order:%d\tRaw%d\tTime%d\tBall:%d\tBx:%d\tBy:%d\tMPU:%d\tdir:%d",outofbounds.order,line.Raw,line.T,sensors.IRangle,((short)(IRv.x)),((short)(IRv.y)),(short)(US3.read()*1000),sensors.dir);
        pc.printf("\r\n");
}
INLINE void Soccer(){
        sensors.IRreceive = Ball.read();
        sensors.IRangle = Ball.readIRangle();
        sensors.IRlong = Ball.readIRlong();
        IRv = makeXYFromRT(sensors.IRangle == 5000 ? 0 : 100, sensors.IRangle);
        Bound = makeXYFromRT(100,line.LastRaw);

        BallTracking(140);
        setline();
        //if (vector.y > 0) vector.y = 0;
        IMU_Chenge = sensors.IMUS;
        //sensors.dir = IMU.receive()*1.4;
        sensors.dir = (503-(US3.read()*1000))/2.4;
        driveVector(vector);
}

/*********************************************************main*******************************************************/
void setup(){
        pc.baud(115200);
        Ball.frequency(230400);
        IMU.frequency(230400);
        Ball.Setinterval(50000); // 50ms*/
        IMU.Setinterval(200000); // 50ms
        MD.frequency(1.0/100000);
        SW.mode(PullUp);
        LINE1.mode(PullDown);
        LINE2.mode(PullDown);
        LINE3.mode(PullDown);
        LINE4.mode(PullDown);
        Tikikker.start();
        TiLINE.start();
        line_wait.start();
        lineOP.attach(&line_check, 1.0 / 12000);

        line.OnWH1st = 0;
        line.OnWH2nd = 0;
        line.OnWH3rd = 0;
        line.OnWH4th = 0;
        line.Frised = 0;
        line.Rrised = 0;
        line.Brised = 0;
        line.Lrised = 0;

        outofbounds.isOUT = 0;
        outofbounds.error = 0;
        outofbounds.linedeg = 5000;
        outofbounds.delegate = 0;

        MD.tweak(101, 100, 100,100);
        wait(1);
        pc.printf("Hello world!! LEGENDα TomiXRM\r\n");
        pc.printf("This computer is %d MHz clock\r\n", (SystemCoreClock / 1000000));
        a = 0;
        didavoid = 0;
        Debug.start();
        line_wait.start();
}

void main2(){
        MD.move(50,50,50,50);
        MD.frequency(1.0/(4500/2));

        wait_ms(200);
        MD.frequency(1.0/(3600/2));
        wait_ms(200);
        MD.frequency(1.0/(600/2));
        wait_ms(200);
        MD.frequency(1.0/(1000/2));
        wait_ms(200);
        //sp.write(0);
        MD.frequency(1.0/100000);

}


int main(void){
        bool debug_Flag = 0;
        setup();
        main2();
        MD.frequency(1.0/100000);
        MD.stop(0);
        while(1) {
                led1 = LINE1;
                led2 = LINE2;
                led3 = LINE3;
                led4 = LINE4;
                if(SW == 0) {
                        if(Debug.read_ms() > 30) {
                                debug();
                                Debug.reset();
                        }
                        Soccer();
                        //IMU_Check();
                }else{
                        debug_Flag == 1;
                        //Debug.detach();


                        MD.stop(0);
                        vector = setVectorZero();
                        outofbounds.order = 0;
                        kicker_enable();
                        wait_ms(10);
                        Ball_Check();
                        US_Check();
                        printf("USright:%d\tUSleft:%d\r\n",sensors.USright,sensors.USleft);

                }

        }
}
