#include "mbed.h"
#include "setup.hpp"
#include "moving_average.h"


// InterruptIn IRsensors[16]={PB_0,PB_1,PB_2,PB_15,PB_4,PB_5,PB_6,PB_7,PB_8,PB_9,PB_10,PB_11,PB_12,PB_13,PB_14,PB_3};

InterruptIn IR1(PB_0);
InterruptIn IR2(PB_1);
InterruptIn IR3(PB_2);
InterruptIn IR4(PB_15);
InterruptIn IR5(PB_4);
InterruptIn IR6(PB_5);
InterruptIn IR7(PB_6);
InterruptIn IR8(PB_7);
InterruptIn IR9(PB_8);
InterruptIn IR10(PB_9);
InterruptIn IR11(PB_10);
InterruptIn IR12(PB_11);
InterruptIn IR13(PB_12);
InterruptIn IR14(PB_13);
InterruptIn IR15(PB_14);
InterruptIn IR16(PB_3);


DigitalOut led(PA_8);
Serial pc(PA_9,NC);
RawSerial Nucleo(PA_2,PA_3);

Timer BaseIRTimer;
Ticker IRPrint;
MovingAverage smaMaxPulse(30);
MovingAverage smaDist(30);

uint16_t IR_PulseWidth[16];  //パルス幅16個分
uint16_t IR_StartTime[16];  //パルス幅16個分
bool IR_Count[16];
bool IR_ActiveSensor[16];
uint8_t IR_count_counter[16];
uint8_t IR_ActiveSensorNum;

uint16_t data;
uint16_t dist;
uint16_t scale;
float degree = 5000;
short IR_MaxPulse = 0;//最大パルス
char Max_Sen;

bool vector_calF;

int16_t Vx, Vy;

int IRdistance = 0;
uint16_t smaIR_MaxPulse,smaIRdistance;

//functions
void setup();
INLINE void SendToMaster();
void IR00change();
void IR01change();
void IR02change();
void IR03change();
void IR04change();
void IR05change();
void IR06change();
void IR07change();
void IR08change();
void IR09change();
void IR10change();
void IR11change();
void IR12change();
void IR13change();
void IR14change();
void IR15change();


void IR00change(){
        if(IR1 == false) {
                IR_StartTime[0] = BaseIRTimer.read_us();
        }else{
                IR_Count[0]=1;
                IR_PulseWidth[0]=BaseIRTimer.read_us()-IR_StartTime[0];
        }
}
void IR01change(){
        if(IR2 == false) {
                IR_StartTime[1] = BaseIRTimer.read_us();
        }else{
                IR_Count[1]=1;
                IR_PulseWidth[1]=BaseIRTimer.read_us()-IR_StartTime[1];
        }
}
void IR02change(){
        if(IR3 == false) {
                IR_StartTime[2] = BaseIRTimer.read_us();
        }else{
                IR_Count[2]=1;
                IR_PulseWidth[2]=BaseIRTimer.read_us()-IR_StartTime[2];
        }
}

void IR03change(){
        if(IR4 == false) {
                IR_StartTime[3] = BaseIRTimer.read_us();
        }else{
                IR_Count[3]=1;
                IR_PulseWidth[3]=BaseIRTimer.read_us()-IR_StartTime[3];
        }
}

void IR04change(){
        if(IR5 == false) {
                IR_StartTime[4] = BaseIRTimer.read_us();
        }else{
                IR_Count[4]=1;
                IR_PulseWidth[4]=BaseIRTimer.read_us()-IR_StartTime[4];
        }
}

void IR05change(){
        if(IR6 == false) {
                IR_StartTime[5] = BaseIRTimer.read_us();
        }else{
                IR_Count[5]=1;
                IR_PulseWidth[5]=BaseIRTimer.read_us()-IR_StartTime[5];
        }
}

void IR06change(){
        if(IR7 == false) {
                IR_StartTime[6] = BaseIRTimer.read_us();
        }else{
                IR_Count[6]=1;
                IR_PulseWidth[6]=BaseIRTimer.read_us()-IR_StartTime[6];
        }
}

void IR07change(){
        if(IR8 == false) {
                IR_StartTime[7] = BaseIRTimer.read_us();
        }else{
                IR_Count[7]=1;
                IR_PulseWidth[7]=BaseIRTimer.read_us()-IR_StartTime[7];
        }
}

void IR08change(){
        if(IR9 == false) {
                IR_StartTime[8] = BaseIRTimer.read_us();
        }else{
                IR_Count[8]=1;
                IR_PulseWidth[8]=BaseIRTimer.read_us()-IR_StartTime[8];
        }
}

void IR09change(){
        if(IR10 == false) {
                IR_StartTime[9] = BaseIRTimer.read_us();
        }else{
                IR_Count[9]=1;
                IR_PulseWidth[9]=BaseIRTimer.read_us()-IR_StartTime[9];
        }
}

void IR10change(){
        if(IR11 == false) {
                IR_StartTime[10] = BaseIRTimer.read_us();
        }else{
                IR_Count[10]=1;
                IR_PulseWidth[10]=BaseIRTimer.read_us()-IR_StartTime[10];
        }
}

void IR11change(){
        if(IR12 == false) {
                IR_StartTime[11] = BaseIRTimer.read_us();
        }else{
                IR_Count[11]=1;
                IR_PulseWidth[11]=BaseIRTimer.read_us()-IR_StartTime[11];
        }
}

void IR12change(){
        if(IR13 == false) {
                IR_StartTime[12] = BaseIRTimer.read_us();
        }else{
                IR_Count[12]=1;
                IR_PulseWidth[12]=BaseIRTimer.read_us()-IR_StartTime[12];
        }
}

void IR13change(){
        if(IR14 == false) {
                IR_StartTime[13] = BaseIRTimer.read_us();
        }else{
                IR_Count[13]=1;
                IR_PulseWidth[13]=BaseIRTimer.read_us()-IR_StartTime[13];
        }
}

void IR14change(){
        if(IR15 == false) {
                IR_StartTime[14] = BaseIRTimer.read_us();
        }else{
                IR_Count[14]=1;
                IR_PulseWidth[14]=BaseIRTimer.read_us()-IR_StartTime[14];
        }
}

void IR15change(){
        if(IR16 == false) {
                IR_StartTime[15] = BaseIRTimer.read_us();
        }else{
                IR_Count[15]=1;
                IR_PulseWidth[15]=BaseIRTimer.read_us()-IR_StartTime[15];
        }
}

INLINE void SendToMaster(){
        led = 1;
        Nucleo.putc('J');
        Nucleo.putc(data % 256);
        Nucleo.putc(data / 256);
        Nucleo.putc(dist);
}

void setup(){
        pc.baud(230400);
        Nucleo.baud(230400);
        BaseIRTimer.reset();
        BaseIRTimer.start();
        vector_calF = 1;
        //IRPrint.attach_us(&SendToMaster,10000);

        IR1.fall(&IR00change);
        IR2.fall(&IR01change);
        IR3.fall(&IR02change);
        IR4.fall(&IR03change);
        IR5.fall(&IR04change);
        IR6.fall(&IR05change);
        IR7.fall(&IR06change);
        IR8.fall(&IR07change);
        IR9.fall(&IR08change);
        IR10.fall(&IR09change);
        IR11.fall(&IR10change);
        IR12.fall(&IR11change);
        IR13.fall(&IR12change);
        IR14.fall(&IR13change);
        IR15.fall(&IR14change);
        IR16.fall(&IR15change);

        IR1.rise(&IR00change);
        IR2.rise(&IR01change);
        IR3.rise(&IR02change);
        IR4.rise(&IR03change);
        IR5.rise(&IR04change);
        IR6.rise(&IR05change);
        IR7.rise(&IR06change);
        IR8.rise(&IR07change);
        IR9.rise(&IR08change);
        IR10.rise(&IR09change);
        IR11.rise(&IR10change);
        IR12.rise(&IR11change);
        IR13.rise(&IR12change);
        IR14.rise(&IR13change);
        IR15.rise(&IR14change);
        IR16.rise(&IR15change);
}


int main() {
        setup();
        while(1) {
                led = 0;
                for(int l=0; l<16; l++) if(IR_Count[l]==0) IR_PulseWidth[l] = 0;
                for(int l=0; l<16; l++) IR_Count[l] = 0;
                wait_us(835);


                IR_MaxPulse = 0;//最大パルス数
                Max_Sen = 20;//エラー対策愛大パルスを受信したセンサ番号
                IR_ActiveSensorNum=0;
                for (size_t i = 0; i < 16; i++) {
                        //IR_ActiveSensorNum += IR_ActiveSensor[i];

                        if(IR_PulseWidth[i] > 0) {
                                IR_ActiveSensorNum++;
                        }

                        //最大パルス数を受信したセンサ番号と最大パルスの決定
                        if (IR_PulseWidth[i] > IR_MaxPulse) {//比較
                                IR_MaxPulse = IR_PulseWidth[i];//最大パルス
                                Max_Sen = i;//最大パルスを受信したセンサ番号0~15
                        }
                }

                // ------------------------------------------------------------------------------
                //ベクトル計算
                // for (size_t i = 1; i < 8; i++) {
                //         IR_PulseWidth[i] = IR_PulseWidth[i]*1.2*(1+(0.003*i));  //まほうのそーすをかけます
                //         IR_PulseWidth[16-i] = IR_PulseWidth[16-i]*1.2*(1+(0.003*i));  //まほうのそーすをかけます
                // }
                vector_calF = 0;
                if(Max_Sen != 20) {
                        if(Max_Sen == 0) {//0
                                degree = 0;
                                vector_calF = 0;
                                Vx =(int16_t)(X00+X225+X_225+X450+X_450);//+X675+X_675;
                                Vy =(int16_t)(Y00+Y225+Y_225+Y450+Y_450);//+Y675+Y_675;
                        }else if(Max_Sen == 1) {//-22.5
                                degree = -22.5;
                                //   vector_calF = 1;
                                Vx = (int16_t)(X_225+X00+X_450+X225+X_675);  //+X_900+X450;
                                Vy = (int16_t)(Y_225+Y00+Y_450+Y225+Y_675);  //+Y_900+Y450;
                        }else if(Max_Sen == 2) {//-45
                                degree = -45;
                                //   vector_calF = 1;
                                Vx = (int16_t)(X_450+X_225+X_675+X00+X_900);  //+X225+X_900;
                                Vy = (int16_t)(Y_450+Y_225+Y_675+Y00+Y_900);  //+Y225+Y_900;
                        }else if(Max_Sen == 3) {//-67.5
                                degree = -67.5;
                                //   vector_calF = 1;
                                Vx = (int16_t)(X_675+X_450+X_900+X_225+X_1125);  //+X00+X_1350;
                                Vy = (int16_t)(Y_675+Y_450+Y_900+Y_225+Y_1125);  //+Y00+Y_1350;
                        }else if(Max_Sen == 4) {//-90
                                degree = -90;
                                //   vector_calF = 1;
                                Vx = (int16_t)(X_900+X_675+X_1125+X_450+X_1350);  //+X_225+X_1575;
                                Vy = (int16_t)(Y_900+Y_675+Y_1125+Y_450+Y_1350);  //+Y_225+Y1575;
                        }else if(Max_Sen == 5) {//-112.5
                                degree = -112.5;
                                //   vector_calF = 1;
                                Vx = (int16_t)(X_1125+X_900+X_1350+X_675+X_1575);//+X_450+X1800;
                                Vy = (int16_t)(Y_1125+Y_900+Y_1350+Y_675+Y_1575);//+Y_450+Y1800;
                        }else if(Max_Sen == 6) {//-135
                                degree = -135;
                                //   vector_calF = 1;
                                Vx = (int16_t)(X_1350+X_1125+X_1575+X_900+X1800);//+X_675+X1575;
                                Vy = (int16_t)(Y_1350+Y_1125+Y_1575+Y_900+Y1800);//+Y_675+Y1575;
                        }else if(Max_Sen == 7) {//-157.5
                                degree = -157.5;
                                //   vector_calF = 1;
                                Vx = (int16_t)(X_1575+X_1350+X1800+X_1125+X1575);//+X_900+X1350;
                                Vy = (int16_t)(Y_1575+Y_1350+Y1800+Y_1125+Y1575);//+Y_900+X1350;
                        }else if(Max_Sen == 8) {//180
                                degree = 180;
                                //   vector_calF = 1;
                                Vx = (int16_t)(X1800+X1575+X_1575+X1350+X_1350);//+X1125+X_1125;
                                Vy = (int16_t)(Y1800+Y1575+Y_1575+Y1350+Y_1350);//+Y1125+Y_1125;
                        }else if(Max_Sen == 9) {//157.5
                                degree = 157.5;
                                //   vector_calF = 1;
                                Vx = (int16_t)(X1575+X1350+X1800+X1125+X_1575);//+X900+X_1350;
                                Vy = (int16_t)(Y1575+Y1350+Y1800+Y1125+Y_1575);//+Y900+Y_1350;
                        }else if(Max_Sen == 10) {//135
                                degree = 135;
                                //   vector_calF = 1;
                                Vx = (int16_t)(X1350+X1125+X1575+X900+X1800);//+X675+X_1575;
                                Vy = (int16_t)(Y1350+Y1125+Y1575+Y900+Y1800);//+Y675+Y_1575;
                        }else if(Max_Sen == 11) {//112.5
                                degree = 112.5;
                                //   vector_calF = 1;
                                Vx = (int16_t)(X1125+X900+X1350+X675+X1575);//+X450+X1800;
                                Vy = (int16_t)(Y1125+Y900+Y1350+Y675+Y1575);//+Y450+Y1800;
                        }else if(Max_Sen == 12) {//90
                                degree = 90;
                                //   vector_calF = 1;
                                Vx = (int16_t)(X900+X675+X1125+X450+X1350);//+X225+X1575;
                                Vy = (int16_t)(Y900+Y675+Y1125+Y450+Y1350);//+Y225+Y1575;
                        }else if(Max_Sen == 13) {//67.5
                                degree = 67.5;
                                //   vector_calF = 1;
                                Vx = (int16_t)(X675+X450+X900+X225+X1125);//+X00+X1350;
                                Vy = (int16_t)(Y675+Y450+Y900+Y225+Y1125);//+Y00+Y1350;
                        }else if(Max_Sen == 14) {//45
                                degree = 45;
                                //   vector_calF = 1;
                                Vx = (int16_t)(X450+X225+X675+X00+X900);//+X_225+X900;
                                Vy = (int16_t)(Y450+Y225+Y675+Y00+Y900);//+Y_225+Y900;
                        }else if(Max_Sen == 15) {//22.5
                                degree = 22.5;
                                //   vector_calF = 1;
                                Vx = (int16_t)(X225+X00+X450+X_225+X675);  //+X900+X_450;
                                Vy = (int16_t)(Y225+Y00+Y450+Y_225+Y675);  //+Y900+Y_450;
                        }


                        /*if(Max_Sen != 20) {
                                if(IR_MaxPulse==IR_PulseWidth[0]) {
                                        Vx=IR_PulseWidth[1]*0.3827+IR_PulseWidth[2]*0.7071+IR_PulseWidth[15]*(-0.3827)+IR_PulseWidth[14]*(-0.7071);
                                        Vy=IR_PulseWidth[0]+IR_PulseWidth[1]*0.9239+IR_PulseWidth[2]*0.7071+IR_PulseWidth[15]*(0.9239)+IR_PulseWidth[14]*(0.7071);
                                }else if(IR_MaxPulse==IR_PulseWidth[1]) {
                                        Vx=IR_PulseWidth[1]*0.3827+IR_PulseWidth[2]*0.7071+IR_PulseWidth[3]*0.9239+IR_PulseWidth[15]*(-0.3827)+IR_PulseWidth[4];
                                        Vy=IR_PulseWidth[0]+IR_PulseWidth[1]*0.9239+IR_PulseWidth[2]*0.7071+IR_PulseWidth[3]*0.3827+IR_PulseWidth[15]*(0.9239);
                                }else if(IR_MaxPulse==IR_PulseWidth[15]) {
                                        Vx=IR_PulseWidth[1]*0.3827+IR_PulseWidth[15]*(-0.3827)+IR_PulseWidth[14]*(-0.7071)+IR_PulseWidth[13]*(-0.9239)+IR_PulseWidth[12]*(-1);
                                        Vy=IR_PulseWidth[1]*0.9239+IR_PulseWidth[0]+IR_PulseWidth[15]*(0.9239)+IR_PulseWidth[14]*0.7071+IR_PulseWidth[13]*0.3827;
                                }else if(IR_MaxPulse==IR_PulseWidth[2]) {
                                        Vx=IR_PulseWidth[1]*0.3827+IR_PulseWidth[2]*0.7071+IR_PulseWidth[3]*0.9239+IR_PulseWidth[4];
                                        Vy=IR_PulseWidth[0]+IR_PulseWidth[1]*0.9239+IR_PulseWidth[2]*0.7071+IR_PulseWidth[3]*0.3827;
                                }else if(IR_MaxPulse==IR_PulseWidth[14]) {
                                        Vx=IR_PulseWidth[13]*(-0.9239)+IR_PulseWidth[14]*(-0.7071)+IR_PulseWidth[15]*(-0.3827)+IR_PulseWidth[12]*(-1);
                                        Vy=IR_PulseWidth[14]*0.7071+IR_PulseWidth[15]*(0.9239)+IR_PulseWidth[13]*0.3827;
                                }else if(IR_MaxPulse==IR_PulseWidth[3]) {
                                        Vx=IR_PulseWidth[1]*0.3827+IR_PulseWidth[2]*0.7071+IR_PulseWidth[3]*0.9239+IR_PulseWidth[5]*0.9239+IR_PulseWidth[4]+IR_PulseWidth[6]*0.7071;
                                        Vy=IR_PulseWidth[1]*0.9239+IR_PulseWidth[2]*0.7071+IR_PulseWidth[3]*0.3827+IR_PulseWidth[5]*(-0.3827)+IR_PulseWidth[6]*(-0.7071);
                                }else if(IR_MaxPulse==IR_PulseWidth[13]) {
                                        Vx=IR_PulseWidth[11]*(-0.9239)+IR_PulseWidth[12]*(-1)+IR_PulseWidth[13]*(-0.9239)+IR_PulseWidth[14]*(-0.7071);
                                        Vy=IR_PulseWidth[11]*(-0.3827)+IR_PulseWidth[13]*0.3827+IR_PulseWidth[14]*0.7071;
                                }else if(IR_MaxPulse==IR_PulseWidth[4]) {
                                        Vx=IR_PulseWidth[2]*0.7071+IR_PulseWidth[3]*0.9239+IR_PulseWidth[4]+IR_PulseWidth[5]*0.9239+IR_PulseWidth[6]*0.7071+IR_PulseWidth[7]*0.3827;
                                        Vy=IR_PulseWidth[2]*0.7071+IR_PulseWidth[3]*0.3827+IR_PulseWidth[5]*(-0.3827)+IR_PulseWidth[6]*(-0.7071)+IR_PulseWidth[7]*(-0.9239);
                                }else if(IR_MaxPulse==IR_PulseWidth[12]) {
                                        Vx=IR_PulseWidth[9]*(-0.3827)+IR_PulseWidth[10]*(-0.7071)+IR_PulseWidth[11]*(-0.9239)+IR_PulseWidth[12]*(-1)+IR_PulseWidth[13]*(-0.9239)+IR_PulseWidth[14]*(-0.7071);
                                        Vy=IR_PulseWidth[9]*(-0.9239)+IR_PulseWidth[10]*(-0.7071)+IR_PulseWidth[11]*(-0.3827)+IR_PulseWidth[13]*0.3827+IR_PulseWidth[14]*0.7071;
                                }else if(IR_MaxPulse==IR_PulseWidth[5]) {
                                        Vx=IR_PulseWidth[2]*0.7071+IR_PulseWidth[3]*0.9239+IR_PulseWidth[4]+IR_PulseWidth[5]*0.9239+IR_PulseWidth[6]*0.7071+IR_PulseWidth[7]*0.3827;
                                        Vy=IR_PulseWidth[2]*0.7071+IR_PulseWidth[3]*0.3827+IR_PulseWidth[5]*(-0.3827)+IR_PulseWidth[6]*(-0.7071)+IR_PulseWidth[7]*(-0.9239)+IR_PulseWidth[8]*(-1);
                                }else if(IR_MaxPulse==IR_PulseWidth[7]) {
                                        Vx=IR_PulseWidth[5]*0.9239+IR_PulseWidth[6]*0.7071+IR_PulseWidth[7]*0.3827+IR_PulseWidth[4];
                                        Vy=IR_PulseWidth[5]*(-0.3827)+IR_PulseWidth[6]*(-0.7071)+IR_PulseWidth[7]*(-0.9239)+IR_PulseWidth[8]*(-1);
                                }else if(IR_MaxPulse==IR_PulseWidth[6]) {
                                        Vx=IR_PulseWidth[3]*0.9239+IR_PulseWidth[4]+IR_PulseWidth[5]*0.9239+IR_PulseWidth[7]*0.3827+IR_PulseWidth[6]*0.7071;
                                        Vy=IR_PulseWidth[3]*0.3827+IR_PulseWidth[5]*(-0.3827)+IR_PulseWidth[6]*(-0.7071)+IR_PulseWidth[7]*(-0.9239);
                                }else if(IR_MaxPulse==IR_PulseWidth[8]) {
                                        Vx=IR_PulseWidth[6]*0.7071+IR_PulseWidth[7]*0.3827+IR_PulseWidth[9]*(-0.3827)+IR_PulseWidth[10]*(-0.7071);
                                        Vy=IR_PulseWidth[6]*(-0.7071)+IR_PulseWidth[7]*(-0.9239)+IR_PulseWidth[9]*(-0.9239)+IR_PulseWidth[8]*(-1)+IR_PulseWidth[10]*(-0.7071);
                                }else if(IR_MaxPulse==IR_PulseWidth[9]) {
                                        Vx=IR_PulseWidth[9]*(-0.3827)+IR_PulseWidth[10]*(-0.7071)+IR_PulseWidth[11]*(-0.9239);
                                        Vy=IR_PulseWidth[9]*(-0.9239)+IR_PulseWidth[8]*(-1)+IR_PulseWidth[10]*(-0.7071)+IR_PulseWidth[11]*(-0.3827);
                                }else if(IR_MaxPulse==IR_PulseWidth[10]) {
                                        Vx=IR_PulseWidth[9]*(-0.3827)+IR_PulseWidth[10]*(-0.7071)+IR_PulseWidth[11]*(-0.9239)+IR_PulseWidth[12]*(-1)+IR_PulseWidth[13]*(-0.9239);
                                        Vy=IR_PulseWidth[9]*(-0.9239)+IR_PulseWidth[8]*(-1)+IR_PulseWidth[10]*(-0.7071)+IR_PulseWidth[11]*(-0.3827)+IR_PulseWidth[13]*0.3827;
                                }else if(IR_MaxPulse==IR_PulseWidth[11]) {
                                        Vx=IR_PulseWidth[9]*(-0.3827)+IR_PulseWidth[11]*(-0.9239)+IR_PulseWidth[12]*(-1)+IR_PulseWidth[13]*(-0.9239)+IR_PulseWidth[10]*(-0.7071);
                                        Vy=IR_PulseWidth[9]*(-0.9239)+IR_PulseWidth[11]*(-0.3827)+IR_PulseWidth[13]*0.3827+IR_PulseWidth[10]*(-0.7071);
                                }*/


                        // if(abs(degree) < 24) {

                        IRdistance = (int)(sqrt(Vx*Vx+Vy*Vy));
                        // }//三平方の定理で距離を求める
                        if(vector_calF == 1) degree = atan2(Vx,Vy)/PI*180.0; //ベクトルから角度の算出

                        if(degree < 0) {degree += 360;}//角度が-180から-1の場合は180から359のデータに換算

                        smaIR_MaxPulse = (uint16_t)(smaMaxPulse.updateData(IR_MaxPulse));
                        smaIRdistance = (uint16_t)(smaDist.updateData(IRdistance));

                        if(IR_MaxPulse==0) {
                                scale=6000;
                        }else{
                                if(IRdistance>1500) {
                                        scale=1000;
                                }else if(IRdistance>1300) {
                                        scale=2000;
                                }else if(IRdistance>900) {
                                        scale=3000;
                                }else if(IRdistance>600) {
                                        scale=4000;
                                }else{
                                        scale=5000;
                                }
                        }
                }else{
                        Vx = 0; Vy = 0; IRdistance = 0; degree = 0; scale =6000;
                }
                data = degree +scale;

                // dist = smaIR_MaxPulse/2.5;
                if(abs(degree) <= 45) {
                        dist = (smaIRdistance/24);
                }else{
                        dist = (smaIRdistance/4)*0.6;
                }


                if(dist > 255) {
                        dist = 255;
                }
                // dist = IRdistance;
//シリアルで浮働小数点は%.16f
                pc.printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t",IR_PulseWidth[0],IR_PulseWidth[1],IR_PulseWidth[2],IR_PulseWidth[3],IR_PulseWidth[4],IR_PulseWidth[5],IR_PulseWidth[6],IR_PulseWidth[7],IR_PulseWidth[8],IR_PulseWidth[9],IR_PulseWidth[10],IR_PulseWidth[11],IR_PulseWidth[12],IR_PulseWidth[13],IR_PulseWidth[14],IR_PulseWidth[15]);
                pc.printf("angle:%d \t MaxPulse:%d \t Distance:%d \t  Max_Sen:%d \tdata:%d \t IR_ActiveSensorCount:%d \t Vx:%d \t Vy:%d \r\n",(short)degree,smaIRdistance,dist,Max_Sen,data,IR_ActiveSensorNum,(Vx),(Vy));
                // pc.printf("%d\t%d\t%ld\n",(int)degree,(int)smaIRdistance,(int)dist);
                pc.printf("\r\n");
                SendToMaster();
        }
}