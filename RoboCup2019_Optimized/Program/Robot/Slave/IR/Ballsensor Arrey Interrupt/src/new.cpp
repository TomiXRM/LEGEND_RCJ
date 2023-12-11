#include "mbed.h"//mbed.hをインクルード

#define OUT_OFFSET 0  //なんじゃこれ
#define TIMEOUT 870   //パルス幅
#define INLINE inline __attribute__((always_inline))

RawSerial pc(USBTX,NC);

InterruptIn IR1(D3);
InterruptIn IR2(D1);
InterruptIn IR3(D4);
InterruptIn IR4(D2);//3142の順番


DigitalOut state_led[4] = {D6,D10,D9,A2};//IRのLED
DigitalOut led(D8);//LED
AnalogOut OUT(A3);//アナログアウト
Timer IRTimer;//IRのタイマー

Timer IRtim1;
Timer IRtim2;
Timer IRtim3;
Timer IRtim4;

Ticker Debug;

int IR_PulseWidth[4];//パルスの数
int Max_num;//パルスの最大値を持つセンサの番号(1~4)
int Max_dat;//最大パルス
int Sum_dat;//パルスの合計
int Ave_dat;//パルスの平均
int Var_dat;//パルスの分散値

bool counted[4];
int outputdata;
float outputdata_;
INLINE void Read();//読み取り
INLINE void cal();//計算

INLINE int sum(int da[],uint8_t n);//合計
INLINE int ave(int da[],uint8_t n);//平均
INLINE int var(int da[],uint8_t n);//分
INLINE int calc();//データからおおよその位置を特定する関数

INLINE void print();//PCにプリントする関数
INLINE bool RL(int Pulse_width[]);//右か左か求める関数



void StartReadIR1(){
        IRtim1.reset();
}
void StartReadIR2(){
        IRtim2.reset();
}
void StartReadIR3(){
        IRtim3.reset();
}
void StartReadIR4(){
        IRtim4.reset();
}

void FinishReadIR1(){
        IR_PulseWidth[0] = IRtim1.read_us();
        counted[0] = 1;
}

void FinishReadIR2(){
        IR_PulseWidth[1] = IRtim1.read_us();
        counted[2] = 1;
}

void FinishReadIR3(){
        IR_PulseWidth[2] = IRtim1.read_us();
        counted[3] = 1;
}

void FinishReadIR4(){
        IR_PulseWidth[3] = IRtim1.read_us();
        counted[4] = 1;
}

/*INLINE void Read() {//読み取り
        //  ir = 0;
        IR_PulseWidth[0] = 0;//初期化
        IR_PulseWidth[1] = 0;//初期化
        IR_PulseWidth[2] = 0;//初期化
        IR_PulseWidth[3] = 0;//初期化
        IRTimer.reset();//初期化
        IRTimer.start();//パルスタイマースタート
   //-----------------------------------------------------------------------------
        //読み取り
        while (IRTimer.read_us() < TIMEOUT) {//パルス読み込み
                for (int i = 0; i < 4; i++) {
                        if (!IR[i]) {
                                IR_PulseWidth[i] += 1;
                        }
                        state_led[i]=!IR[i];
                }
        }
        IRTimer.stop();
   }*/

//-----------------------------------------------------------------------------
//もろもろ計算
INLINE void cal(){
        Max_num = 0;
        Max_dat = 0;
        for (uint8_t i = 0; i < 4; i++) {
                if(Max_dat < IR_PulseWidth[i]) {
                        Max_dat = IR_PulseWidth[i]; Max_num = i+1;
                }
        }
        /*Sum_dat = sum(IR_PulseWidth,4);
           Ave_dat = ave(IR_PulseWidth,4);/*/
        Var_dat = var(IR_PulseWidth,4);
}
//-----------------------------------------------------------------------------
//合計
INLINE int sum(int da[],uint8_t n){
        int total;
        for (uint8_t i = 0; i < n; i++) {
                total += da[i];
        }
        return total;
}
//-----------------------------------------------------------------------------
//平均
INLINE int ave(int da[],uint8_t n){
        return sum(da,n)/n;
}
//-----------------------------------------------------------------------------
//分散
INLINE int var(int da[],uint8_t n){
        int av = ave(da,n);
        int v  = 0;//分散
        for (uint8_t i = 0; i < n; i++) {
                v += (da[i] - av)*(da[i] - av);
        }
        return v/n;
}
//特定の処理********************************

INLINE int calc(){
        uint8_t result;
        int total;
        uint16_t var_num;
        bool rl;


        var_num = var(IR_PulseWidth,4)/500;
        if(IR_PulseWidth[0] != 0&&IR_PulseWidth[1] != 0&&IR_PulseWidth[2] != 0&&IR_PulseWidth[3] != 0) {
                total = sum(IR_PulseWidth,4);
                if (IR_PulseWidth[0] + IR_PulseWidth[1] == IR_PulseWidth[2] + IR_PulseWidth[3]) {
                        result = 50;
                }else{
                        if(IR_PulseWidth[0] + IR_PulseWidth[1] > IR_PulseWidth[2] + IR_PulseWidth[3]) {
                                rl = 0;//左
                        }else if (IR_PulseWidth[0] + IR_PulseWidth[1] < IR_PulseWidth[2] + IR_PulseWidth[3]) {
                                rl = 1;//右
                        }
                        if (total == 0) {
                                //ボールなし
                                result = 50;
                        }else{
                                if (var_num > 50) {var_num = 50;}
                                if(rl == 1) {
                                        result = 50 + var_num;
                                }else if (rl == 0) {
                                        result = 50 - var_num;
                                }
                        }
                        //ボールあり

                }
        }else{
                result = 50;
        }
        return result;
}
//-----------------------------------------------------------------------------

INLINE void print(){

        pc.printf("Max_Sensor:%d\t",Max_num);
        pc.printf("Max_Pulse:%d\t",Max_dat);
        /*pc.printf("Sum_Pulse:%d\t",Sum_dat);
           pc.printf("Ave_Pulse:%d\t",Ave_dat);
           pc.printf("Var_Pulse:%d\t",Var_dat/10);
         */
        pc.printf("Var_Pulse:%d\t",var(IR_PulseWidth,4)/2000);
        pc.printf(" %d\t", IR_PulseWidth[0]);
        pc.printf(" %d\t", IR_PulseWidth[1]);
        pc.printf(" %d\t", IR_PulseWidth[2]);
        pc.printf(" %d\t", IR_PulseWidth[3]);
        //*/

        pc.printf("%d \t %d\n",outputdata,(short)(outputdata_*1000));


        //pc.printf("\r\n");
}


int main() {

        IR1.fall(&StartReadIR1);
        IR2.fall(&StartReadIR2);
        IR3.fall(&StartReadIR3);
        IR4.fall(&StartReadIR4);

        IR1.rise(&FinishReadIR1);
        IR2.rise(&FinishReadIR2);
        IR3.rise(&FinishReadIR3);
        IR4.rise(&FinishReadIR4);


        pc.baud(230400);
        IRTimer.start();
        Debug.attach_us(&print,10000);
        IRtim1.start(); IRtim2.start(); IRtim3.start(); IRtim4.start();
        while (1) {
                //for(uint8_t l=0; l<4; l++) if(counted[l]==0) IR_PulseWidth[l]=0;
                //for(uint8_t l=0; l<4; l++) counted[l]=0;
                //wait_us(1000);
                //Read();
                if(IRtim1.read_us() > TIMEOUT) {IR_PulseWidth[0] = 0; IRtim1.reset();}
                if(IRtim2.read_us() > TIMEOUT) {IR_PulseWidth[1] = 0; IRtim2.reset();}
                if(IRtim3.read_us() > TIMEOUT) {IR_PulseWidth[2] = 0; IRtim3.reset();}
                if(IRtim4.read_us() > TIMEOUT) {IR_PulseWidth[3] = 0; IRtim4.reset();}
                cal();
                //outputdata = calc();
                outputdata_ = (float)outputdata;
                outputdata_ *= 0.01;
                OUT.write((float)(outputdata_/100));
                //wait_ms(50);
        }
}
