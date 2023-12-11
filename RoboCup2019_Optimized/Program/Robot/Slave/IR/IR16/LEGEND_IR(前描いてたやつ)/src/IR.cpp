#include "mbed.h"
#include "moving_average.h"
#define INLINE inline __attribute__((always_inline))

DigitalOut led(A2, 0);    //LED
Serial pc(USBTX, NC);     //PC
RawSerial Nucleo(D5, NC); //通信
InterruptIn Call(D4);     //通信
Timer IRTimer;            //IRセンサのタイマー
Ticker irSend;


MovingAverage smaMaxPulse(20);
MovingAverage smaDist(20);

#define PI 3.14159265359

//------------------------------------------------------------------------------
//三角関数表
#define sin0_0    0
#define cos0_0    1
#define sin22_5   0.3827
#define cos22_5   0.9239
#define sin45_0   0.7071
#define cos45_0   0.7071
#define sin67_5   0.9239
#define cos67_5   0.3827
#define sin90_0   1
#define cos90_0   0
#define sin112_5  0.9239
#define cos112_5 -0.3827
#define sin135_0  0.7071
#define cos135_0  0.7071
#define sin157_5  0.3827
#define cos157_5 -0.9239
#define sin180_0  0
#define cos180_0  -1

#define sin_22_5  -0.3827
#define cos_22_5   0.9239
#define sin_45_0  -0.7071
#define cos_45_0   0.7071
#define sin_67_5  -0.9239
#define cos_67_5   0.3827
#define sin_90_0  -1
#define cos_90_0   0
#define sin_112_5 -0.9239
#define cos_112_5 -0.3827
#define sin_135_0 -0.7071
#define cos_135_0 -0.7071
#define sin_157_5 -0.3827
#define cos_157_5 -0.9239
//------------------------------------------------------------------------------
//めちゃくちゃにします(XY反転)
//XにsinかけてYにcosかける脳筋法
#define X00   IR_PulseWidth[0]*sin0_0
#define Y00   IR_PulseWidth[0]*cos0_0
#define X225  IR_PulseWidth[1]*sin_22_5
#define Y225  IR_PulseWidth[1]*cos_22_5
#define X450  IR_PulseWidth[3]*sin_45_0
#define Y450  IR_PulseWidth[3]*cos_45_0
#define X675  IR_PulseWidth[5]*sin_67_5
#define Y675  IR_PulseWidth[5]*cos_67_5
#define X900  IR_PulseWidth[7]*sin_90_0
#define Y900  IR_PulseWidth[7]*cos_90_0
#define X1125 IR_PulseWidth[9]*sin_112_5
#define Y1125 IR_PulseWidth[9]*cos_112_5
#define X1350 IR_PulseWidth[11]*sin_135_0
#define Y1350 IR_PulseWidth[11]*cos_135_0
#define X1575 IR_PulseWidth[13]*sin_157_5
#define Y1575 IR_PulseWidth[13]*cos_157_5
#define X1800 IR_PulseWidth[15]*sin180_0
#define Y1800 IR_PulseWidth[15]*cos180_0

#define X_225  IR_PulseWidth[2]*sin22_5
#define Y_225  IR_PulseWidth[2]*cos22_5
#define X_450  IR_PulseWidth[4]*sin45_0
#define Y_450  IR_PulseWidth[4]*cos45_0
#define X_675  IR_PulseWidth[6]*sin67_5
#define Y_675  IR_PulseWidth[6]*cos67_5
#define X_900  IR_PulseWidth[8]*sin90_0
#define Y_900  IR_PulseWidth[8]*cos90_0
#define X_1125 IR_PulseWidth[10]*sin112_5
#define Y_1125 IR_PulseWidth[10]*cos112_5
#define X_1350 IR_PulseWidth[12]*sin135_0
#define Y_1350 IR_PulseWidth[12]*cos135_0
#define X_1575 IR_PulseWidth[14]*sin157_5
#define Y_1575 IR_PulseWidth[14]*cos157_5
//------------------------------------------------------------------------------


DigitalIn IRsensors[16] = {
        D0,   //PA9   9   0
        D1,   //PA10  10  22.5
        A1,   //PA1   1   22.5
        D2,   //PA12  12  45
        A0,   //PA0   0   45
        D3,   //PB0   0   67.5
        D13,  //PB3   3   67.5
        A4,   //PA5   5   90
        D12,  //PB4   4   90
        A5,   //PA6   6   112.5
        D11,  //PB5   5   112.5
        D6,   //PB1   1   135
        D10,  //PA11  11  135
        D7,   //PF0   0   167.5
        D9,   //PA8   8   167.5
        D8    //PF1   1   180
};

short degree = 5000;
char Max_Sen;
short IR_MaxPulse = 0;//最大パルス
short smaIR_MaxPulse = 0;//移動平均
short IRdistance;//距離
short smaIRdistance;
uint16_t ir = 0;
short IR_PulseWidth[16];//パルス幅16個分
short IR_ActiveSensorCount;//アクティブなセンサの数
const int timeout = 833;//パルス周期
short data;//送信データ
float Vx, Vy = 0;//ベクトル


INLINE void ballread() {
        //------------------------------------------------------------------------------
        //初期化
        for (int j = 0; j < 16; j++) {
                IR_PulseWidth[j] = 0;
        }
        //------------------------------------------------------------------------------
        //読み取り
        //本当はエッジがかかるまで待機してパルスの始まりから測定するべき
        IRTimer.reset();
        IRTimer.start();
        IR_ActiveSensorCount = 0;
        while (IRTimer.read_us() < timeout*2) {//パルスの周期内で読み取りIRTimer.read_us() < timeout*2
                for (int i = 0; i < 16; i++) {
                        if (!IRsensors[i]) {
                                IR_PulseWidth[i]++;
                                IR_ActiveSensorCount++;
                        }
                }
        }
        IRTimer.stop();
        //------------------------------------------------------------------------------
        IR_MaxPulse = 0;//最大パルス数
        Max_Sen = 20;//エラー対策愛大パルスを受信したセンサ番号
        //------------------------------------------------------------------------------
        //最大パルス数を受信したセンサ番号と最大パルスの決定
        for (short I_f = 0; I_f < 16; I_f++) { //見えてるやつ
                if (IR_PulseWidth[I_f] > IR_MaxPulse) {//比較
                        IR_MaxPulse = IR_PulseWidth[I_f];//最大パルス
                        Max_Sen = I_f;//最大パルスを受信したセンサ番号0~15
                }
        }
        //------------------------------------------------------------------------------
        //ベクトル計算
        //最大パルスを受信したセンサの左右2つづつのベクトルを足し算します。
        if(Max_Sen != 20) {
                IR_MaxPulse = IR_MaxPulse*1.6*(1+(0.003*Max_Sen));//まほうのそーすをかけます
                if(Max_Sen == 0) {//0
                        Vx = X00+X225+X_225;//+X450+X_450;
                        Vy = Y00+Y225+Y_225;//+Y450+Y_450;
                }else if(Max_Sen == 1) {//22.5
                        Vx = X225+X00+X450+X_225+X675;
                        Vy = Y225+Y00+Y450+Y_225+Y675;
                }else if(Max_Sen == 2) {//-22.5
                        Vx = X_225+X00+X_450+X225+X_675;
                        Vy = Y_225+Y00+Y_450+Y225+Y_675;
                }else if(Max_Sen == 3) {//45
                        Vx = X450+X225+X675+X00+X900;
                        Vy = Y450+Y225+Y675+Y00+Y900;
                }else if(Max_Sen == 4) {//-45
                        Vx = X_450+X_225+X_675+X00+X_900;
                        Vy = Y_450+Y_225+Y_675+Y00+Y_900;
                }else if(Max_Sen == 5) {//67.5
                        Vx = X675+X450+X900+X225+X1125;
                        Vy = Y675+Y450+Y900+Y225+Y1125;
                }else if(Max_Sen == 6) {//-67.5
                        Vx = X_675+X_450+X_900+X_225+X_1125;
                        Vy = Y_675+Y_450+Y_900+Y_225+Y_1125;
                }else if(Max_Sen == 7) {//90
                        Vx = X900+X675+X1125+X450+X1350;
                        Vy = Y900+Y675+Y1125+Y450+Y1350;
                }else if(Max_Sen == 8) {//-90
                        Vx = X_900+X_675+X_1125+X_450+X_1350;
                        Vy = Y_900+Y_675+Y_1125+Y_450+Y_1350;
                }else if(Max_Sen == 9) {//112.5
                        Vx = X1125+X900+X1350+X675+X1575;
                        Vy = Y1125+Y900+Y1350+Y675+Y1575;
                }else if(Max_Sen == 10) {//-112.5
                        Vx = X_1125+X_900+X_1350+X_675+X_1575;
                        Vy = Y_1125+Y_900+Y_1350+Y_675+Y_1575;
                }else if(Max_Sen == 11) {//135
                        Vx = X1350+X1125+X1575+X900+X1800;
                        Vy = Y1350+Y1125+Y1575+Y900+Y1800;
                }else if(Max_Sen == 12) {//-135
                        Vx = X_1350+X_1125+X_1575+X_900+X1800;
                        Vy = Y_1350+Y_1125+Y_1575+Y_900+Y1800;
                }else if(Max_Sen == 13) {//157.5
                        Vx = X1575+X1350+X1800+X1125+X_1575;
                        Vy = Y1575+Y1350+Y1800+Y1125+Y_1575;
                }else if(Max_Sen == 14) {//-157.5
                        Vx = X_1575+X_1350+X1800+X_1125+X1575;
                        Vy = Y_1575+Y_1350+Y1800+Y_1125+Y1575;
                }else if(Max_Sen == 15) {//180
                        Vx = X1800+X1575+X_1575+X1350+X_1350;
                        Vy = Y1800+Y1575+Y_1575+Y1350+Y_1350;
                }
                IRdistance = (short)(sqrt(Vx*Vx+Vy*Vy)*2);//三平方の定理で距離を求める
                degree = atan2(Vx,Vy)/PI*180.0;//ベクトルから角度の算出
                smaIR_MaxPulse = (uint16_t)(smaMaxPulse.updateData(IR_MaxPulse));
                smaIRdistance = (uint16_t)(smaDist.updateData(IRdistance));

                if(Max_Sen == 0) {//0
                        //degree = 0;
                        if(smaIRdistance>410) {
                                ir=1000;//ちかい
                        }else if(smaIRdistance>350) {
                                ir=2000;//まあちかい
                        }else if(smaIRdistance>310
                                 ) {
                                ir=3000;//かろうじて近い
                        }else if(smaIRdistance>1) {
                                ir=4000;//めちゃくちゃ遠い
                        }else{
                                ir=5000;
                        }
                }else if(Max_Sen == 1) {//22.5
                        //degree = -22.5;
                        if(smaIRdistance>500) {
                                ir=1000;//ちかい
                        }else if(smaIRdistance>460) {
                                ir=2000;//まあちかい
                        }else if(smaIRdistance>330) {
                                ir=3000;//かろうじて近い
                        }else if(smaIRdistance>1) {
                                ir=4000;//めちゃくちゃ遠い
                        }else{
                                ir=5000;
                        }
                }else if(Max_Sen == 2) {//-22.5
                        //degree = 22.5;
                        if(smaIRdistance>500) {
                                ir=1000;//ちかい
                        }else if(smaIRdistance>450) {
                                ir=2000;//まあちかい
                        }else if(smaIRdistance>330) {
                                ir=3000;//かろうじて近い
                        }else if(smaIRdistance>1) {
                                ir=4000;//めちゃくちゃ遠い
                        }else{
                                ir=5000;
                        }
                }else if(Max_Sen == 3) {//45
                        degree = -45;
                        if(smaIRdistance>480) {
                                ir=1000;//ちかい
                        }else if(smaIRdistance>440) {
                                ir=2000;//まあちかい
                        }else if(smaIRdistance>330) {
                                ir=3000;//かろうじて近い
                        }else if(smaIRdistance>1) {
                                ir=4000;//めちゃくちゃ遠い
                        }else{
                                ir=5000;
                        }
                }else if(Max_Sen == 4) {//-45
                        degree = 45;
                        if(smaIRdistance>440) {
                                ir=1000;//ちかい
                        }else if(smaIRdistance>410) {
                                ir=2000;//まあちかい
                        }else if(smaIRdistance>330) {
                                ir=3000;//かろうじて近い
                        }else if(smaIRdistance>1) {
                                ir=4000;//めちゃくちゃ遠い
                        }else{
                                ir=5000;
                        }
                }else if(Max_Sen == 5) {//67.5
                        degree = -67.5;
                        if(smaIRdistance>440) {
                                ir=1000;//ちかい
                        }else if(smaIRdistance>430) {
                                ir=2000;//まあちかい
                        }else if(smaIRdistance>330) {
                                ir=3000;//かろうじて近い
                        }else if(smaIRdistance>1) {
                                ir=4000;//めちゃくちゃ遠い
                        }else{
                                ir=5000;
                        }
                }else if(Max_Sen == 6) {//-67.5
                        degree = 67.5;
                        if(smaIRdistance>440) {
                                ir=1000;//ちかい
                        }else if(smaIRdistance>430) {
                                ir=2000;//まあちかい
                        }else if(smaIRdistance>400) {
                                ir=3000;//かろうじて近い
                        }else if(smaIRdistance>1) {
                                ir=4000;//めちゃくちゃ遠い
                        }else{
                                ir=5000;
                        }
                }else if(Max_Sen == 7) {//90
                        degree = -90;
                        if(smaIRdistance>470) {
                                ir=1000;//ちかい
                        }else if(smaIRdistance>430) {
                                ir=2000;//まあちかい
                        }else if(smaIRdistance>400) {
                                ir=3000;//かろうじて近い
                        }else if(smaIRdistance>1) {
                                ir=4000;//めちゃくちゃ遠い
                        }else{
                                ir=5000;
                        }
                }else if(Max_Sen == 8) {//-90
                        degree = 90;
                        if(smaIRdistance>470) {
                                ir=1000;//ちかい
                        }else if(smaIRdistance>430) {
                                ir=2000;//まあちかい
                        }else if(smaIRdistance>400) {
                                ir=3000;//かろうじて近い
                        }else if(smaIRdistance>1) {
                                ir=4000;//めちゃくちゃ遠い
                        }else{
                                ir=5000;
                        }
                }else if(Max_Sen == 9) {//112.5
                        degree = -112.5;
                        if(smaIRdistance>470) {
                                ir=1000;//ちかい
                        }else if(smaIRdistance>430) {
                                ir=2000;//まあちかい
                        }else if(smaIRdistance>400) {
                                ir=3000;//かろうじて近い
                        }else if(smaIRdistance>1) {
                                ir=4000;//めちゃくちゃ遠い
                        }else{
                                ir=5000;
                        }
                }else if(Max_Sen == 10) {//-112.5
                        degree = 112.5;
                        if(smaIRdistance>470) {
                                ir=1000;//ちかい
                        }else if(smaIRdistance>430) {
                                ir=2000;//まあちかい
                        }else if(smaIRdistance>400) {
                                ir=3000;//かろうじて近い
                        }else if(smaIRdistance>1) {
                                ir=4000;//めちゃくちゃ遠い
                        }else{
                                ir=5000;
                        }
                }else if(Max_Sen == 11) {//135
                        degree = -135;
                        if(smaIRdistance>340) {
                                ir=1000;//ちかい
                        }else if(smaIRdistance>330) {
                                ir=2000;//まあちかい
                        }else if(smaIRdistance>300) {
                                ir=3000;//かろうじて近い
                        }else if(smaIRdistance>1) {
                                ir=4000;//めちゃくちゃ遠い
                        }else{
                                ir=5000;
                        }
                }else if(Max_Sen == 12) {//-135
                        degree = 135;
                        if(smaIRdistance>430) {
                                ir=1000;//ちかい
                        }else if(smaIRdistance>400) {
                                ir=2000;//まあちかい
                        }else if(smaIRdistance>300) {
                                ir=3000;//かろうじて近い
                        }else if(smaIRdistance>1) {
                                ir=4000;//めちゃくちゃ遠い
                        }else{
                                ir=5000;
                        }
                }else if(Max_Sen == 13) {//157.5
                        degree = -157.5;
                        if(smaIRdistance>330) {
                                ir=1000;//ちかい
                        }else if(smaIRdistance>300) {
                                ir=2000;//まあちかい
                        }else if(smaIRdistance>250) {
                                ir=3000;//かろうじて近い
                        }else if(smaIRdistance>1) {
                                ir=4000;//めちゃくちゃ遠い
                        }else{
                                ir=5000;
                        }
                }else if(Max_Sen == 14) {//-157.5
                        degree = 157.5;
                        if(smaIRdistance>330) {
                                ir=1000;//ちかい
                        }else if(smaIRdistance>300) {
                                ir=2000;//まあちかい
                        }else if(smaIRdistance>250) {
                                ir=3000;//かろうじて近い
                        }else if(smaIRdistance>1) {
                                ir=4000;//めちゃくちゃ遠い
                        }else{
                                ir=5000;
                        }
                }else if(Max_Sen == 15) {//180
                        degree = 180;
                        if(smaIRdistance>330) {
                                ir=1000;//ちかい
                        }else if(smaIRdistance>300) {
                                ir=2000;//まあちかい
                        }else if(smaIRdistance>250) {
                                ir=3000;//かろうじて近い
                        }else if(smaIRdistance>1) {
                                ir=4000;//めちゃくちゃ遠い
                        }else{
                                ir=5000;
                        }
                }
                if(degree < 0) {degree += 360;}//角度が-180から-1の場合は180から359のデータに換算



                if(IR_MaxPulse==0) {
                        ir=6000;
                }else{

                        /*if(smaIR_MaxPulse>120) {
                                ir=1000;//ちかい
                           }else if(smaIR_MaxPulse>100) {
                                ir=2000;//まあちかい
                           }else if(smaIR_MaxPulse>85) {
                                ir=3000;//かろうじて近い
                           }else if(smaIR_MaxPulse>60) {
                                ir=4000;//めちゃくちゃ遠い
                           }else{
                                ir=5000;
                           }
                           /*if(abs(degree) < 45) {
                                if(smaIRdistance > 0) ir=4000; //めちゃくちゃ遠い
                                if(smaIRdistance > 340) ir=3000; //かろうじてちかい
                                if(smaIRdistance > 530) ir=2000; //まあちかい
                                if(smaIRdistance > 550) ir=1000; //ちかい
                           }else if(degree == 45) {
                                if(smaIRdistance > 0) ir=4000; //めちゃくちゃ遠い
                                if(smaIRdistance > 400) ir=3000; //かろうじてちかい
                                if(smaIRdistance > 440) ir=2000; //まあちかい
                                if(smaIRdistance > 500) ir=1000; //ちかい
                           }else if(degree == 315) {
                                if(smaIRdistance > 0) ir=4000; //めちゃくちゃ遠い
                                if(smaIRdistance > 400) ir=3000; //かろうじてちかい
                                if(smaIRdistance > 460) ir=2000; //まあちかい
                                if(smaIRdistance > 470) ir=1000; //ちかい
                           }else if(degree == 67) {
                                if(smaIRdistance > 0) ir=4000; //めちゃくちゃ遠い
                                if(smaIRdistance > 400) ir=3000; //かろうじてちかい
                                if(smaIRdistance > 460) ir=2000; //まあちかい
                                if(smaIRdistance > 490) ir=1000; //ちかい
                           }else if(degree == 293) {
                                if(smaIRdistance > 0) ir=4000; //めちゃくちゃ遠い
                                if(smaIRdistance > 380) ir=3000; //かろうじてちかい
                                if(smaIRdistance > 420) ir=2000; //まあちかい
                                if(smaIRdistance > 460) ir=1000; //ちかい
                           }else if(degree == 90) {
                                if(smaIRdistance > 0) ir=4000; //めちゃくちゃ遠い
                                if(smaIRdistance > 410) ir=3000; //かろうじてちかい
                                if(smaIRdistance > 480) ir=2000; //まあちかい
                                if(smaIRdistance > 505) ir=1000; //ちかい
                           }else if(degree == 270) {
                                if(smaIRdistance > 0) ir=4000; //めちゃくちゃ遠い
                                if(smaIRdistance > 410) ir=3000; //かろうじてちかい
                                if(smaIRdistance > 420) ir=2000; //まあちかい
                                if(smaIRdistance > 450) ir=1000; //ちかい








                           }else if(degree == 112) {
                                if(smaIRdistance > 0) ir=4000; //めちゃくちゃ遠い
                                if(smaIRdistance > 380) ir=3000; //かろうじてちかい
                                if(smaIRdistance > 420) ir=2000; //まあちかい
                                if(smaIRdistance > 460) ir=1000; //ちかい
                           }else if(degree == 248) {
                                if(smaIRdistance > 0) ir=4000; //めちゃくちゃ遠い
                                if(smaIRdistance > 380) ir=3000; //かろうじてちかい
                                if(smaIRdistance > 420) ir=2000; //まあちかい
                                if(smaIRdistance > 460) ir=1000; //ちかい
                           }else if(degree == 135) {
                                if(smaIRdistance > 0) ir=4000; //めちゃくちゃ遠い
                                if(smaIRdistance > 380) ir=3000; //かろうじてちかい
                                if(smaIRdistance > 420) ir=2000; //まあちかい
                                if(smaIRdistance > 460) ir=1000; //ちかい
                           }else if(degree == 225) {
                                if(smaIRdistance > 0) ir=4000; //めちゃくちゃ遠い
                                if(smaIRdistance > 380) ir=3000; //かろうじてちかい
                                if(smaIRdistance > 420) ir=2000; //まあちかい
                                if(smaIRdistance > 460) ir=1000; //ちかい
                           }else if(degree == 315) {
                                if(smaIRdistance > 0) ir=4000; //めちゃくちゃ遠い
                                if(smaIRdistance > 380) ir=3000; //かろうじてちかい
                                if(smaIRdistance > 420) ir=2000; //まあちかい
                                if(smaIRdistance > 460) ir=1000; //ちかい
                           }else if(degree == 157) {
                                if(smaIRdistance > 0) ir=4000; //めちゃくちゃ遠い
                                if(smaIRdistance > 380) ir=3000; //かろうじてちかい
                                if(smaIRdistance > 420) ir=2000; //まあちかい
                                if(smaIRdistance > 460) ir=1000; //ちかい
                           }else if(degree == 203) {
                                if(smaIRdistance > 0) ir=4000; //めちゃくちゃ遠い
                                if(smaIRdistance > 380) ir=3000; //かろうじてちかい
                                if(smaIRdistance > 420) ir=2000; //まあちかい
                                if(smaIRdistance > 460) ir=1000; //ちかい
                           }else if(degree == 180) {
                                if(smaIRdistance > 0) ir=4000; //めちゃくちゃ遠い
                                if(smaIRdistance > 380) ir=3000; //かろうじてちかい
                                if(smaIRdistance > 420) ir=2000; //まあちかい
                                if(smaIRdistance > 460) ir=1000; //ちかい
                           }*/
                }   //*/
                ir = ir + degree;
        }else{
                Vx = 0; Vy = 0; IRdistance = 0; degree = 0; ir =6000;
                smaIR_MaxPulse = 0; smaIRdistance = 0;
        }
        //------------------------------------------------------------------------------

        data = ir;

        //pc.printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t", IR_PulseWidth[0],IR_PulseWidth[1],IR_PulseWidth[2],IR_PulseWidth[3],IR_PulseWidth[4],IR_PulseWidth[5],IR_PulseWidth[6],IR_PulseWidth[7],IR_PulseWidth[8],IR_PulseWidth[9],IR_PulseWidth[10],IR_PulseWidth[11],IR_PulseWidth[12],IR_PulseWidth[13],IR_PulseWidth[14],IR_PulseWidth[15]);
        pc.printf(" angle:%d \t MaxPulse:%d \t Distance:%d \t  Max_Sen:%d \tir:%d\r\n",degree,smaIR_MaxPulse,smaIRdistance,Max_Sen,ir);
        //pc.printf("%d\r\n",smaIR_MaxPulse);
        //wait_us(500);
}

void SendToMaster() {
        led = 1;
        Nucleo.putc('H');
        Nucleo.putc(data % 256);
        Nucleo.putc(data / 256);
        //pc.printf("Interrupt!!\t");
        // pc.printf("%d\r\n",data);
}

int main() {
        pc.baud(230400);
        Nucleo.baud(230400);
        IRTimer.start();
        Call.rise(&SendToMaster);
        Call.mode(PullDown);
        //pc.printf("\n");
        while (1) {
                led = 0;
                ballread();
                // pc.printf("angle is %d !!\r\n",ir);
        }
}
