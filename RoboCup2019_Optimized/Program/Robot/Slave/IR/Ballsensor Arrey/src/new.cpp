#include "mbed.h"

#define OUT_OFFSET 0
#define TIMEOUT 833
#define INLINE inline __attribute__((always_inline))


DigitalIn IR[4] = {D3,D1,D4,D2};//3142の順番
DigitalOut state_led[4] = {D6,D10,D9,A2};
DigitalOut led(D8);
AnalogOut OUT(A3);
Timer IRTimer;
Serial pc(USBTX,NC);

uint16_t IR_PulseWidth[4];
uint16_t Max_num;
uint16_t Max_dat;
uint16_t Sum_dat;
uint16_t Ave_dat;
uint16_t Var_dat;
float cross[4]={0,0,0,0}; //[x1,y1,x2,y2]

int C1[4];
int C2[4];
int C3[4];

void setup();
INLINE void Read();//読み取り
INLINE void cal();//計算

INLINE uint16_t sum(uint16_t da[],uint8_t n);//合計
INLINE uint16_t ave(uint16_t da[],uint8_t n);//平均
INLINE uint16_t var(uint16_t da[],uint8_t n);//分散

INLINE void print();

INLINE void Read() {
        //  ir = 0;
        IR_PulseWidth[0] = 0;
        IR_PulseWidth[1] = 0;
        IR_PulseWidth[2] = 0;
        IR_PulseWidth[3] = 0;
        IRTimer.reset();
        IRTimer.start();
        while (IRTimer.read_us() < TIMEOUT) {
                for (int i = 0; i < 4; i++) {
                        if (!IR[i]) {
                                IR_PulseWidth[i] += 1;
                        }
                        state_led[i]=!IR[i];
                }
        }
        IRTimer.stop();

}


INLINE void cal(){
        Max_num = 0;
        Max_dat = 0;
        for (uint8_t i = 0; i < 4; i++) {
                if(Max_dat < IR_PulseWidth[i]) { Max_dat = IR_PulseWidth[i]; Max_num = i+1;}
        }
        /*Sum_dat = sum(IR_PulseWidth,4);
           Ave_dat = ave(IR_PulseWidth,4);/*/
        Var_dat = var(IR_PulseWidth,4);
}

INLINE uint16_t sum(uint16_t da[],uint8_t n){
        uint16_t total;
        for (uint8_t i = 0; i < n; i++) {
                total += da[i];
        }
        return total;
}

INLINE uint16_t ave(uint16_t da[],uint8_t n){
        return sum(da,n)/n;
}

INLINE uint16_t var(uint16_t da[],uint8_t n){
        uint16_t av = ave(da,n);
        uint16_t v  = 0;//分散
        for (uint8_t i = 0; i < n; i++) {
                v += (da[i] - av)*(da[i] - av);
        }
        return v/n;
}

//円周りの処理********************************
//cross...length: 4 のfloat配列
//cross{x1,y1,x2,y2}
INLINE void Calc_WCircle(int *p,int x1,int y1,uint16_t r1,int x2, int y2,uint16_t r2){

        float xi1=0;
        float yi1=0;
        float xi2=0;
        float yi2=0;
        int l=0;
        float theta1=0;
        float theta2=0;

        l = sqrt(powf(x2 - x1, 2) + powf(y2 - y1,2));
        theta1 = atan2(y2 - y1,x2 - x1);
        theta2 = acos((powf(l,2) + powf(r1,2) - powf(r2,2)) / (2 * l * r1));
        xi1 = x1 + r1 * cos(theta1 + theta2);
        yi1 = y1 + r1 * sin(theta1 + theta2);
        xi2 = x1 + r1 * cos(theta1 - theta2);
        yi2 = y1 + r1 * sin(theta1 - theta2);

        p[0]= (int)(round(xi1 * 1000) / 1000);
        p[1] = (int)(round(yi1 * 1000) / 1000);
        p[2] = (int)(round(xi2 * 1000) / 1000);
        p[3] = (int)(round(yi2 * 1000) / 1000);


}

INLINE void print(){
        // pc.printf("Max_Sensor:%d\t",Max_num);
        // pc.printf("Max_Pulse:%d\t",Max_dat);
        // pc.printf("Sum_Pulse:%d\t",Sum_dat);
        // pc.printf("Ave_Pulse:%d\t",Ave_dat);
        //pc.printf("Var_Pulse:%d\t",Var_dat/10);

        pc.printf("%d\t", IR_PulseWidth[0]);
        pc.printf("%d\t", IR_PulseWidth[1]);
        pc.printf("%d\t", IR_PulseWidth[2]);
        pc.printf("%d\t", IR_PulseWidth[3]);

        pc.printf("\r\n");
}

void setup(){
        for (char i = 0; i < 4; i++) {
                C1[i] = 0;
                C2[i] = 0;
                C3[i] = 0;
        }

}
int main() {
        pc.baud(115200);
        IRTimer.start();
        setup();
        while (1) {
                IR_PulseWidth[0] = 80;
                IR_PulseWidth[1] = 60;
                Calc_WCircle(&C1[0],0,0,IR_PulseWidth[0],25,0,IR_PulseWidth[1]);//1と2
                Calc_WCircle(&C2[0],0,25,IR_PulseWidth[1],75,0,IR_PulseWidth[2]);//2と3
                Calc_WCircle(&C3[0],0,75,IR_PulseWidth[2],100,0,IR_PulseWidth[3]);//3と4


                pc.printf("(%d,%d) (%d,%d)",C1[0],C1[1],C1[2],C1[3]);

                //Read();
                cal();
                print();
                //wait_ms(50);
        }
}
