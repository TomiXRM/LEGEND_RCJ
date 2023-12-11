#include "mbed.h"
Serial pc(USBTX,USBRX);
DigitalOut LED(D13);
int a;
/*
      /\
   B /  \  A
    /    \
   / ____ \
      C
 */
int Cx1,Cy1,R1;
int Cx2,Cy2,R2;

int S,C,Px,Py;//面積、辺C(底辺)、交点x、三角形の高さ(交点y)


//面積を求める
int HeronsFormula(int a,int b,int c){
        int s = (a + b + c)/2;
        return sqrt(s*(s-a)*(s-b)*(s-c));
}
int main() {

        Cx1 = 0;  Cy1 = 0;  R1 = 50;//丸1
        Cx2 = 25; Cy2 = 0;  R2 = 50;//丸2
        while(1) {
                LED = !LED;
                //pc.printf("hello world !!\r\n");
                //wait_ms(60);

                C = sqrt((Cx1-Cx2)*(Cx1-Cx2) + (Cy1-Cy2)*(Cx1-Cx2));//辺Cの値を求める
                S = HeronsFormula(R2,R1,C);//面積を求める
                Py = (2*S)/C;//三角形の高さ(交点Y)を求める
                Px = sqrt((R1*R1)-(Py*Py));
        }
}
