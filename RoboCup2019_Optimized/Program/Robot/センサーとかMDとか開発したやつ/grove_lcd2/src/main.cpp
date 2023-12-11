#include "Grove_LCD_RGB_Backlight.h"
#include "RotationalEncoder.h"
#include "Ping.h"
#include "AT24C1024.h"
#include "stdio.h"
#include "mbed.h"
#define PI 3.14159265359
#define START_MODE 0
#define POWER_MODE 1
#define COMMPUS_MODE 3
#define LINE_MODE 2
#define US_MODE 5
#define WTIMER 4
#define Number_of_the_mode 6
#define INLINE inline __attribute__((always_inline))
/*intからcharへの分解
   分解の際
   dat1 = data % 256;
   dat2 = data / 256;
   復元の際
   data = dat2 * 256 + dat1;
   参考元：https://oshiete.goo.ne.jp/qa/6219385.html
 */
/*
   D0  RX
   D1  TX
   D2  rota
   D3  rotb
   D4  sda
   D5  scl
   D6  battery
   D7  sp
   D8  serialinter
   D9  red
   D10 greeen
   D11 blue
   D12 sw
   D13 led
   A0  TRIG
   A1  ECHO1
   A2  ECHO2
   A3  echo3
   A4  ECHO4
   A5  start
   A6  serialout
   A7  buzzerin
 */
I2C i2c(D4,D5);      // SDA, SCL
Grove_LCD_RGB_Backlight rgbLCD(D4,D5);
AT24C1024 at24c1024(D4,D5);// Atmel 1Mbit EE-PROM
RotationalEncoder encoder(D2, D3);
Serial pc(USBTX, USBRX);
RawSerial Nucleo(D1,D0);
InterruptIn Call(D8);
Timer T1;
PwmOut sp(D7);
InterruptIn sw(D12);
DigitalIn sw_(D12);
DigitalOut startled(A5,0);
DigitalOut LED[3] = {
        D9,D10,D11
};

Ping US(A3,NC);
Ping US1(NC,A1);
Ping US2(NC,A2);
Ping US3(NC,A0);
Ping US4(NC,A4);
char lcdMessage[16];
int trimdig = 100;
int mode = 0;                   //default start
bool sww = 0;
bool Break_in =0;
short Mpower = trimdig;
short PING1,PING2,PING3,PING4;
short a;
struct {
        unsigned char highbyte;
        unsigned char lowbyte;
        short intdat = 0;
} data;

INLINE void buzzer(char spm = 0){
        switch(spm) {
        case 0:
                sp.period(1.0/3000);
                sp.write(0.5);
                wait_ms(8);
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
        default:
                sp.period(1.0/3000);
                sp.write(0.5);
                wait_ms(10);
                sp.write(0);   //reset
                break;
        }
}
INLINE void power_trim(){
        LED[0] = 1;
        LED[1] = 0;
        LED[2] = 1;
        trimdig = encoder.getAbsolutePulses();
        trimdig = Mpower + trimdig%101;
        if(trimdig >= 100) {
                encoder.reset();
                //encoder.chenge(100);
                trimdig = 100;
                Mpower = trimdig;
                rgbLCD.setRGB(200,0,40);
                buzzer(2);
        }else if(trimdig <= 0) {
                encoder.reset();
                //encoder.chenge(0);
                trimdig = 0;
                Mpower = trimdig;
        }else if(trimdig < 50) {
                rgbLCD.setRGB(255,50,80);
        }else{
                rgbLCD.setRGB(10,255,40);
        }
        /*----------lcdMessage---------*/
        rgbLCD.locate(0,1); //行、列
        sprintf(lcdMessage,"  Power is%4d",trimdig);
        rgbLCD.print(lcdMessage);
        pc.printf("%s\r\n",lcdMessage);
/*----------Buzzer check---------*/
        if(abs(encoder.getRelativePulses()) > 0) { //rotaryencoder
                buzzer();
        }
}
INLINE void switch_read(){
        buzzer(sww+1);
        while(sw.read() == 0) ;
        sww =!sww;
        //pc.printf("sww = %d",sww);
}
INLINE void LCD_print_MODE(short _mode){
        switch(_mode) {
        case START_MODE:
                rgbLCD.locate(0,0);//行、列
                rgbLCD.print("Start MODE");
                rgbLCD.print("                ");//クリア
                rgbLCD.locate(0,1);//行、列
                rgbLCD.print("Ready??     ");
                //rgbLCD.clear();
                //rgbLCD.print("                ");//クリア
                rgbLCD.setRGB(255,255,255);
                LED[0] = 0;
                LED[1] = 0;
                LED[2] = 0;
                break;
        case POWER_MODE:
                rgbLCD.locate(0,0);//行、列
                rgbLCD.print("Power MODE");
                rgbLCD.print("                ");//クリア
                rgbLCD.locate(0,1);//行、列
                sprintf(lcdMessage,"Power is %3d  ",trimdig);
                rgbLCD.print(lcdMessage);
                //rgbLCD.print("                ");//クリア
                rgbLCD.setRGB(10,255,40);
                LED[0] = 1;
                LED[1] = 0;
                LED[2] = 1;
                break;
        case COMMPUS_MODE:
                rgbLCD.locate(0,0);//行、列
                rgbLCD.print("Commpus MODE");
                rgbLCD.print("                ");//クリア
                rgbLCD.locate(0,1);//行、列
                rgbLCD.print("Ready??     ");
                rgbLCD.setRGB(255,80,90);
                LED[0] = 0;
                LED[1] = 1;
                LED[2] = 0;
                break;
        case LINE_MODE:
                rgbLCD.locate(0,0);//行、列
                rgbLCD.print("Line check MODE");
                rgbLCD.print("                ");//クリア
                rgbLCD.locate(0,1);//行、列
                rgbLCD.print("Ready??     ");
                rgbLCD.setRGB(10,255,255);
                LED[0] = 1;
                LED[1] = 0;
                LED[2] = 0;
                break;
        case WTIMER:
                rgbLCD.locate(0,0);//行、列
                rgbLCD.print("Timer MODE");
                rgbLCD.print("                ");//クリア
                rgbLCD.locate(0,1);//行、列
                rgbLCD.print("Ready??     ");
                rgbLCD.setRGB(255,255,10);
                LED[0] = 0;
                LED[1] = 0;
                LED[2] = 1;
                break;
        case US_MODE:
                rgbLCD.locate(0,0);//行、列
                rgbLCD.print("US check MODE");
                rgbLCD.print("                ");//クリア
                rgbLCD.locate(0,1);//行、列
                rgbLCD.print("Ready??     ");
                rgbLCD.setRGB(10,10,240);
                LED[0] = 1;
                LED[1] = 1;
                LED[2] = 0;
                break;
        }
}
INLINE void watchingTimer(){
        T1.reset();
        T1.start();
        //1000　1秒
        //60000 1分
        short Tim = 0;
        uint16_t sec = 0;
        uint8_t _sec = 0;
        uint16_t min=0;
        while(sww == 1) {
                _sec = sec;
                Tim = T1.read();
                min = Tim/60;
                sec = Tim%60;
                if(sec-_sec >= 1) buzzer(2);
                sprintf(lcdMessage,"%2d:%2d  ",min,sec);
                rgbLCD.locate(0,1);//行、列
                rgbLCD.print(lcdMessage);
        }
        rgbLCD.print("                ");//クリア
}
INLINE void EEPROM_write(short data,int8_t addre){
        char dl = data%256;
        char dh = data/256;
        at24c1024.write(addre, dl); // write addr=0 data=dl
        wait_ms(5);
        at24c1024.write(addre + 1, dh); // write addr=0 data=dh
        wait_ms(5);
}
INLINE short EEPROM_read(uint8_t addre){
        char dl = at24c1024.read(addre); // read addr=0
        char dh = at24c1024.read(addre + 1); // read addr=0
        return (dh*256 + dl);
}
INLINE void sendTomaster(){
        a++;
        data.intdat = a;
        data.highbyte = data.intdat%256;
        data.lowbyte = data.intdat/256;
        Nucleo.putc('C');
        Nucleo.putc(data.highbyte);
        Nucleo.putc(data.lowbyte);
}
int main()
{
        rgbLCD.init();
        wait_ms(10);
        rgbLCD.locate(0,0);//行、列
        rgbLCD.print("Ri-one SAKURA");
        rgbLCD.locate(0,1);//行、列
        rgbLCD.print("Well come!!");
        rgbLCD.setRGB(255, 255, 255);
        buzzer(3);//login
        sw.rise(&switch_read);
        sw.mode(PullDown);
        Call.rise(&sendTomaster);
        Call.mode(PullDown);
        Nucleo.baud(230400);
        //wait(0.5);
        Mpower = EEPROM_read(0);
        trimdig = Mpower;
        while(1) {
                startled = !startled;
                wait_ms(500);
        }
        while(1) {
                rgbLCD.locate(0,1);//行、列
                rgbLCD.print("                ");
                while(2) {
                        mode = encoder.getAbsolutePulses();//クリックを取得
                        mode = abs(mode%(Number_of_the_mode));
                        if(abs(encoder.getRelativePulses()) > 0) {//rotaryencoder
                                buzzer();
                        }
                        //決定ボタン押すまでloop
                        sww,Break_in = 0;//フラグリセット
                        switch(mode) {
                        case START_MODE:
                                LCD_print_MODE(START_MODE);
                                if(sww == 1) {
                                        Break_in = 1;
                                }
                                break;
                        case POWER_MODE:
                                LCD_print_MODE(POWER_MODE);
                                if(sww == 1) {
                                        Break_in = 1;
                                }
                                break;
                        case COMMPUS_MODE:
                                LCD_print_MODE(COMMPUS_MODE);
                                if(sww == 1) {
                                        Break_in = 1;
                                }
                                break;
                        case LINE_MODE:
                                LCD_print_MODE(LINE_MODE);
                                if(sww == 1) {
                                        Break_in = 1;
                                }
                                break;
                        case WTIMER:
                                LCD_print_MODE(WTIMER);
                                if(sww == 1) {
                                        Break_in = 1;
                                }
                                break;
                        case US_MODE:
                                LCD_print_MODE(US_MODE);
                                if(sww == 1) {
                                        Break_in = 1;
                                }
                                break;
                        }
                        if(Break_in == 1) {
                                break;//while2の抜け出しbreak
                        }
                }
                /*------------------------------------ Function ------------------------------------*/
                encoder.reset();
                //sww = 0;
                startled.write(0);
                switch(mode) {
                case START_MODE:
                        while(sww == 1) {
                                //rgbLCD.locate(0,1); //行、列
                                //rgbLCD.print("Start       ");
                                startled.write(1);
                                //PING取得
                        }
                        startled.write(0);
                        rgbLCD.print("                "); //クリア
                        rgbLCD.locate(0,1); //行、列
                        rgbLCD.print("Stop");
                        //rgbLCD.print("                ");//クリア
                        break;
                case POWER_MODE:
                        Mpower = EEPROM_read(0);
                        while(sww == 1) {
                                power_trim();
                        }
                        Mpower = trimdig;
                        EEPROM_write(Mpower,0);
                        rgbLCD.clear();
                        break;
                case COMMPUS_MODE:
                        while(sww == 1) {
                                rgbLCD.locate(0,1); //行、列
                                rgbLCD.print("COMMPUS MODE");
                        }
                        rgbLCD.print("                "); //クリア
                        break;
                case LINE_MODE:
                        while(sww == 1) {
                                rgbLCD.locate(0,1); //行、列
                                rgbLCD.print("LINE MODE");
                        }
                        rgbLCD.print("                "); //クリア
                        break;
                case WTIMER:
                        watchingTimer();
                        break;
                case US_MODE:
                        rgbLCD.locate(0,0); //行、列
                        rgbLCD.print("                "); //クリア
                        while(sww == 1) {
                                /*US1.Send();
                                   PING1 = (short)US1.Read_cm();*/
                                rgbLCD.locate(0,0);   //行、列
                                rgbLCD.print("US MODE");
                                sprintf(lcdMessage,"US1 %4d",PING1);
                                rgbLCD.locate(0,1);   //行、列
                                rgbLCD.print(lcdMessage);
                                pc.printf("US1:%4d\tUS2:%4d\tUS3:%4d\tUS4:%4d\r\n",PING1,PING2,PING3,PING4);
                                wait_ms(10);
                        }
                        rgbLCD.clear();
                        break;
                }
                //encoder.reset();
                encoder.chenge(mode);
        }
}
