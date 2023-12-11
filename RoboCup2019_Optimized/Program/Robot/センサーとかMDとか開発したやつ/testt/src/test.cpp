#include "mbed.h"
#include "RotationalEncoder.h"
#include "QEI.h"
#include "Ping.h"
//#include "AT24C1024.h"
#include "stdio.h"

#define PI 3.14159265359
#define START_MODE 0
#define LINE_MODE 1
#define COMMPUS_MODE 2
#define US_MODE 3
#define BALL_MODE 4
#define Number_of_the_mode 5


#define RED 0
#define GREEN 1
#define BLUE 2
#define INLINE inline __attribute__((always_inline))

#define tone_A  262
#define tone_S  294
#define tone_D  330
#define tone_F  349
#define tone_G  392
#define tone_H  440
#define tone_J  494
#define tone_K  523
/*intからcharへの分解
   分解の際
   dat1 = data % 256;
   dat2 = data / 256;
   復元の際
   data = dat2 * 256 + dat1;
   参考元：https://oshiete.goo.ne.jp/qa/6219385.html
 */
/*
   D0  RX           PA10
   D1  TX           PA9
   D2  rota         PA12
   D3  rotb         PB0
   D4  sda          PB7
   D5  scl          PB6
   D6  battery      PB1
   D7  BUZZER PF0
   D8  serialinter  PF1
   D9  red
   D10 greeen
   D11 blue
   D12 sw           PB4
   D13 led
   A0  ECHO3        PA0
   A1  ECHO1        PA1
   A2  ECHO2        PA3
   A3  TRIG         PA4
   A4  ECHO4        PA5
   A5  start        PA6
   A6  serialout    PA7
   A7  buzzerin     A7(USBTX)



   INTERRUPTIN

   PB0
   PA0//ECHO
   PF1
   PA1//ECHO
   PA3//ECHO
   PB4
   PA5//ECHO
   PA12

 */
Timer RT;
Serial pc(USBTX, NC);//ひつよう
RawSerial Nucleo(D1,D0);//必要
//RotationalEncoder encoder(D2, D3);//必要
QEI encoder(D2,D3,NC,24,&RT,QEI::X2_ENCODING);
I2C i2c(D4,D5);
PwmOut sp(D7);////必要
InterruptIn Call(D8);////必要
DigitalOut LED[3] = {
        D9,D10,D11
};
InterruptIn sw(D12);////必要 PB_4
DigitalOut led(D13);
DigitalOut ST(A5,0);////必要


/*INTERRUPTIN
   PA_0
   PA_1
   PA_3
   PB_4
   PA_5
   PA_12
   PB_0
 */
/*DigitalOut TRIGGER(A3);
   DigitalIn US1(A1);//PA_1
   DigitalIn US2(A2);///PA_3
   DigitalIn US3(A0);//PA_0
   DigitalIn US4(A4);//PA_5*/

//Ping US1(A3,A1);
/*Ping US2(A3,A2);
   Ping US3(A3,A0);
   Ping US4(A3,A4);*/
//InterruptIn BUZZER(A7);
Timer T1;////必要
uint8_t mode;
uint8_t sww;
short a;
short Relativepulse,oldRelativepulse;
struct {
        unsigned char highbyte;
        unsigned char lowbyte;
        short intdat = 0;
} data;
struct {//超音波センサ
        volatile unsigned char front;
        volatile unsigned char back;
        volatile unsigned char left;
        volatile unsigned char right;
} Sonic;
int timeout;

INLINE void sendTomaster(){
        if(mode == US_MODE) {
                data.intdat = a;
                data.highbyte = data.intdat%256;
                data.lowbyte = data.intdat/256;
                Nucleo.putc('S');
                Nucleo.putc(Sonic.front);
                Nucleo.putc(Sonic.right);
                Nucleo.putc(Sonic.back);
                Nucleo.putc(Sonic.left);
        }else{
                a++;
                data.intdat = a;
                data.highbyte = data.intdat%256;
                data.lowbyte = data.intdat/256;
                Nucleo.putc('C');
                Nucleo.putc(data.highbyte);
                Nucleo.putc(data.lowbyte);
        }
}
INLINE void buzzer(char spm = 0){
        switch(spm) {
        case 0:
                sp.period(1.0/3000);
                sp.write(0.5);
                wait_ms(2);
                sp.write(0);   //reset
                break;
        case 1:
                sp.period(1.0/3000);
                sp.write(0.5);
                wait_ms(50);
                sp.period(1.0/6000);
                wait_ms(30);
                sp.write(0);
                break;
        case 2:
                sp.period(1.0/10000);
                sp.write(0.5);
                wait_ms(20);
                sp.period(1.0/4200);
                wait_ms(50);
                sp.write(0);
                break;
        case 3:
                //login
                sp.period(1.0/9000);
                sp.write(0.5);
                wait_ms(100);
                sp.period(1.0/7200);
                wait_ms(100);
                sp.period(1.0/1200);
                wait_ms(100);
                sp.period(1.0/8000);
                wait_ms(100);
                sp.write(0);
                break;
        case 4:
                //sublogin
                sp.period(1.0/(tone_H*4));
                sp.write(0.5);
                wait_ms(60);
                sp.period(1.0/(tone_F*4));
                wait_ms(60);
                sp.period(1.0/(tone_H*4));
                wait_ms(60);
                sp.period(1.0/(tone_F*4));
                wait_ms(60);
                sp.period(1.0/(tone_H*4));
                wait_ms(60);
                sp.period(1.0/(tone_F*4));
                wait_ms(300);
                sp.write(0);
                break;
        default:
                sp.period(1.0/3000);
                sp.write(0.5);
                wait_ms(10);
                sp.write(0);   //reset
                break;
        }
}
INLINE void Buzzer_call(void){
        sp.period(1.0/3000);
        sp.write(0.5);
        //pc.printf("buzzer is on\r\n");
        led = 1;
}
INLINE void Buzzer_call_stop(void){
        sp.write(0);
        led = 0;
}
INLINE void switch_read(){
        buzzer(sww+1);
        while(sw.read() == 0) ;
        sww =!sww;
}
/*
   INLINE void ustest(){
        TRIGGER=0;
        wait_us(1);
        TRIGGER=1;
        wait_us(30);
        TRIGGER=0;
        T1.start();
        timeout = 0;
        while(US1.read()==0 && timeout==0) {
                if(T1.read_us()>2000) {
                        timeout=1;
                        //pc.printf("timeout1\r\n");
                }
        }
        T1.stop();
        T1.reset();
        if(timeout==0) {
                T1.start();
                while(US1.read()==1 && timeout==0) {
                        if(T1.read_us()>1000000) {
                                timeout=1;
                                T1.stop();
                        }
                }
                if(timeout==1) {
                        timeout=0;
                        Sonic.front=500;
                }
                else{
                        timeout=0;
                        T1.stop();
                        Sonic.front=T1.read_us();
                        T1.reset();
                        Sonic.front = Sonic.front/2;
                        Sonic.front = Sonic.front*340*100/1000000;
                }
        }
        else{
                Sonic.front=500;
                timeout=0;
        }

   }
 */
/*INLINE void USread(){
        US1.Send(); wait_ms(20);
        Sonic.front = US1.Read_cm();
        Sonic.right = US2.Read_cm();
        Sonic.back = US3.Read_cm();
        Sonic.left = US4.Read_cm();
   }*/
INLINE void LCD_print_MODE(short _mode){
        switch(_mode) {
        case START_MODE:
                pc.printf("Start MODE\r\n");
                pc.printf("Ready??\r\n");
                LED[0] = 0;
                LED[1] = 0;
                LED[2] = 0;
                break;
        case COMMPUS_MODE:
                pc.printf("Commpus MODE\r\n");
                pc.printf("Ready??\r\n");
                LED[0] = 1;
                LED[1] = 0;
                LED[2] = 1;
                break;
        case US_MODE:
                pc.printf("US check MODE\r\n");
                pc.printf("Ready??\r\n");
                LED[0] = 0;
                LED[1] = 1;
                LED[2] = 0;
                break;
        }
}
int main() {
        sw.rise(&switch_read);
        sw.mode(PullDown);
        Call.rise(&sendTomaster);
        Call.mode(PullDown);
        /*BUZZER.rise(&Buzzer_call);
           BUZZER.fall(&Buzzer_call_stop);
           BUZZER.mode(PullDown);*/
        Nucleo.baud(230400);
        pc.baud(115200);
        /*US1.mode(PullDown);
           US2.mode(PullDown);
           US3.mode(PullDown);
           US4.mode(PullDown);*/
        LED[GREEN] = LED[RED] = LED[BLUE] = 0;
        wait_ms(50);
        LED[GREEN] = LED[RED] = LED[BLUE] = 1;
        wait_ms(50);
        LED[GREEN] = LED[RED] = LED[BLUE] = 0;
        wait_ms(50);
        LED[GREEN] = LED[RED] = LED[BLUE] = 1;
        wait_ms(50);
        LED[GREEN] = LED[RED] = LED[BLUE] = 0;
        wait_ms(50);
        LED[GREEN] = LED[RED] = LED[BLUE] = 1;
        wait_ms(50);
        buzzer(0);
        buzzer(1);
        buzzer(2);
        wait_ms(300);
        buzzer(3);
        pc.printf("Hello world!! LEGENDα US_module\r\n");
        pc.printf("This STM32 is %d MHz clock\r\n", (SystemCoreClock / 1000000));
        while(1) {
                ST = 0;
                mode = encoder.getPulses()/2;//クリックを取得
                Relativepulse = mode - oldRelativepulse;
                oldRelativepulse = mode;
                mode = abs(mode%(Number_of_the_mode));
                if(abs(Relativepulse) > 0) {      //rotaryencoder
                        buzzer();
                }
                switch (mode) {
                case START_MODE:
                        LED[RED] = 0;
                        LED[GREEN] = 0;
                        LED[BLUE] = 0;
                        if(sww == 1) {
                                ST = 1;
                                Nucleo.putc('A');
                                while(sww == 1) {
                                        //us
                                        wait_ms(1);
                                        pc.printf("moving\r\n");
                                }
                                ST = 0;
                        }
                        wait_ms(100);
                        break;
                case US_MODE:
                        LED[RED] = 0;
                        LED[GREEN] = 1;
                        LED[BLUE] = 0;
                        if(sww == 1) {
                                ST = 1;
                                Nucleo.putc('B');
                                while(sww == 1) {
                                        /*US1.Send(); wait_ms(15);
                                           Sonic.front = US1.Read_cm();
                                           Sonic.right = US2.Read_cm();
                                           Sonic.back = US3.Read_cm();
                                           Sonic.left = US4.Read_cm();*/
                                        pc.printf("US1:%d\tUS2:%d\tUS3:%d\tUS3:%d\r\n",Sonic.front,Sonic.right,Sonic.back,Sonic.left);
                                }
                                ST = 0;
                        }
                        break;
                case LINE_MODE:
                        LED[RED] = 1;
                        LED[GREEN] = 0;
                        LED[BLUE] = 1;
                        if(sww == 1) {
                                ST = 1;
                                Nucleo.putc('C');
                                buzzer(4);
                                while(sww == 1) {
                                        wait_ms(10);
                                        pc.printf("line check!!\r\n");
                                }
                                ST = 0;
                        }
                        break;
                case BALL_MODE:
                        LED[RED] = 1;
                        LED[GREEN] = 0;
                        LED[BLUE] = 0;
                        if(sww == 1) {
                                ST = 1;
                                Nucleo.putc('D');
                                while(sww == 1) {
                                        wait_ms(10);
                                        pc.printf("ball check!!\r\n");
                                }
                                ST = 0;
                        }
                        break;
                case COMMPUS_MODE:
                        LED[RED] = 0;
                        LED[GREEN] = 1;
                        LED[BLUE] = 1;
                        if(sww == 1) {
                                ST = 1;
                                Nucleo.putc('E');
                                buzzer(4);
                                while(sww == 1) {
                                        wait_ms(10);
                                        pc.printf("commpus check!!\t\n");
                                }
                                ST = 0;
                        }
                        break;
                }

                //wait_ms(109);
                pc.printf("%d\t%d\t%d\r\n",mode,sww,Relativepulse);

        }
}
