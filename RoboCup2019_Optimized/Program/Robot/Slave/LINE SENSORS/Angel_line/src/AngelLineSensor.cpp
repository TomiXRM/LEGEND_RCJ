#include "mbed.h"
#include "LGAngelLine.h"

#define INLINE inline __attribute__((always_inline))
#define PI 3.14159265358979323846264338327


#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

//白色判断の閾値設定(0~65535)
#define WhiteThreshold_00 30000
#define WhiteThreshold_01 30000
#define WhiteThreshold_02 30000
#define WhiteThreshold_03 30000
#define WhiteThreshold_04 30000
#define WhiteThreshold_05 30000
#define WhiteThreshold_06 30000
#define WhiteThreshold_07 30000
#define WhiteThreshold_08 30000
#define WhiteThreshold_09 30000
#define WhiteThreshold_10 30000
#define WhiteThreshold_11 30000
#define WhiteThreshold_12 30000
#define WhiteThreshold_13 30000
#define WhiteThreshold_14 30000
#define WhiteThreshold_15 30000

//空中判断の閾値設定(0~65535)
#define OnAirThreshold 2000

//ledの輝度(0.00~1.00)
#define LedBrightness 1.00

//prototypes
INLINE void ReadLine();
INLINE void JudgeOnWhat();
INLINE void LineFlgSubstitution();
INLINE void MonitorAnalog(bool printEnable);
INLINE void MonitorWhiteFlg(bool printEnable);
INLINE void MonitorGreenFlg(bool printEnable);
INLINE void MonitorAirFlg(bool printEnable);
void setup();


RawSerial pc(PA_9,NC);//デバッグ
RawSerial Main(PC_6,PC_7);//メイン通信
Timer timer;
//  DigitalIn lineSensors_D[16] = {
// //         //0  1    2    3    4    5    6    7    8    9    10   11   12   13   14   15
//          PA_0,PA_1,PA_2,PA_3,PA_4,PA_5,PA_6,PA_7,PC_4,PC_5,PB_0,PB_1,PC_0,PC_1,PC_2,PC_3
//  };

AnalogIn lineSensors[16] = {
        //0  1    2    3    4    5    6    7    8    9    10   11   12   13   14   15
        PA_0,PA_1,PA_2,PA_3,PA_4,PA_5,PA_6,PA_7,PC_4,PC_5,PB_0,PB_1,PC_0,PC_1,PC_2,PC_3
};

int a;
// AnalogIn lineSensors[16] = {
//         //0  1    2    3    4    5    6    7    8    9    10   11   12   13   14   15
//         NC,NC,NC,NC,NC,NC,NC,NC,PA_5,PA_6,PA_7,NC,NC,NC,NC,NC
// };
// FROM DATA SHEET(STM32F446XX)
// ADC123_IN0 PA_0
// ADC123_IN1 PA_1
// ADC123_IN2 PA_2
// ADC123_IN3 PA_3
// ADC123_IN4 PA_4
// ADC123_IN5 PA_5
// ADC123_IN6 PA_6
// ADC123_IN7 PA_7
// ADC123_IN8 PB_0
// ADC123_IN9 PB_1
// ADC123_IN10 PC_0
// ADC123_IN11 PC_1
// ADC123_IN12 PC_2
// ADC123_IN13 PC_3
// ADC123_IN14 PC_4
// ADC123_IN15 PC_5

// AnalogIn lineSensor00(PA_0);
// AnalogIn lineSensor01(PA_1);
// AnalogIn lineSensor02(PA_2);//#
// AnalogIn lineSensor03(PA_3);//#
// AnalogIn lineSensor04(PA_4);
// AnalogIn lineSensor05(PA_5);
// AnalogIn lineSensor06(PA_6);
// AnalogIn lineSensor07(PA_7);
// AnalogIn lineSensor08(PB_0);
// AnalogIn lineSensor09(PB_1);
// AnalogIn lineSensor10(PC_0);
// AnalogIn lineSensor11(PC_1);
// AnalogIn lineSensor12(PC_2);
// AnalogIn lineSensor13(PC_3);
// AnalogIn lineSensor14(PC_4);
// AnalogIn lineSensor15(PC_5);

PwmOut LEDS(PA_11);//ledドライバ(PA_11)

uint16_t lineThresholdOnWhite[16]{//閾値軍
        WhiteThreshold_00,
        WhiteThreshold_01,
        WhiteThreshold_02,
        WhiteThreshold_03,
        WhiteThreshold_04,
        WhiteThreshold_05,
        WhiteThreshold_06,
        WhiteThreshold_07,
        WhiteThreshold_08,
        WhiteThreshold_09,
        WhiteThreshold_10,
        WhiteThreshold_11,
        WhiteThreshold_12,
        WhiteThreshold_13,
        WhiteThreshold_14,
        WhiteThreshold_15
};

uint16_t lineValue[16];//センサの値
bool lineOnWhite[16];//白flg
bool lineOnGreen[16];//緑flg
bool lineOnAir[16];//空中flg
uint16_t lineFlg;//白色に触れているセンサのビットフラグ。

uint8_t highbyte;
uint8_t lowbyte;


/**************************************************************************************************/
//ReadLine
//ラインセンサをアナログ値(0~65535)で読み取りする関数
//for構文を使っていない理由は高速化を図るためです。
INLINE void ReadLine(){
        timer.reset();
        timer.start();
        lineValue[0] = lineSensors[0].read_u16();
        lineValue[1] = lineSensors[1].read_u16();
        lineValue[2] = lineSensors[2].read_u16();
        lineValue[3] = lineSensors[3].read_u16();
        lineValue[4] = lineSensors[4].read_u16();
        lineValue[5] = lineSensors[5].read_u16();
        lineValue[6] = lineSensors[6].read_u16();
        lineValue[7] = lineSensors[7].read_u16();
        lineValue[8] = lineSensors[8].read_u16();
        lineValue[9] = lineSensors[9].read_u16();
        lineValue[10] = lineSensors[10].read_u16();
        lineValue[11] = lineSensors[11].read_u16();
        lineValue[12] = lineSensors[12].read_u16();
        lineValue[13] = lineSensors[13].read_u16();
        lineValue[14] = lineSensors[14].read_u16();
        lineValue[15] = lineSensors[15].read_u16();
        // timer.stop();
}
/**************************************************************************************************/
//JudgeOnWhat
// それぞれのセンサが現在どの状態にあるかを判定する関数です。
// 状況は3つあります。(白色の上,緑色の上,空中)
INLINE void JudgeOnWhat(){
        for (size_t i = 0; i < 16; i++) {
                if (lineValue[i] > lineThresholdOnWhite[i]) {
                        lineOnWhite[i] = true;
                        lineOnGreen[i] = false;
                        lineOnAir[i] = false;
                }else{
                        if(lineValue[i] < OnAirThreshold) {
                                lineOnAir[i] = true;
                                lineOnGreen[i] = false;
                        }else{
                                lineOnWhite[i] = false;
                                lineOnGreen[i] = true;
                                lineOnAir[i] = false;
                        }
                }
        }
}
/**************************************************************************************************/
//LineFlgSubstitution
// センサの状態(白色か白色じゃないか)を16bitの変数 "lineFlg"　にビット単位で代入する関数です。
//変数lineFlgは65536通りパターンを示すことができます。
INLINE void LineFlgSubstitution(){//白色ラインセンサのフラグ代入
        for (size_t i = 0; i < 16; i++) {
                lineFlg = bitSet(lineOnWhite[i],i);//linterエラー無視していいですよ
        }
}

/**************************************************************************************************/
//MonitorAnalog
// 各センサのアナログ値をプリントする関数です。
INLINE void MonitorAnalog(bool printEnable){
        if(printEnable) pc.printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t",lineValue[0],lineValue[1],lineValue[2],lineValue[3],lineValue[4],lineValue[5],lineValue[6],lineValue[7],lineValue[8],lineValue[9],lineValue[10],lineValue[11],lineValue[12],lineValue[13],lineValue[14],lineValue[15]);
}
/**************************************************************************************************/
//MonitorWhiteFlg
//白色を感知しているセンサのフラグ(0/1)をプリントする関数です。
INLINE void MonitorWhiteFlg(bool printEnable){
        if(printEnable) {
                for (uint8_t i = 0; i < 16; i++) {
                        pc.putc(bitRead(1,lineOnWhite[i]));
                }
        }
}
/**************************************************************************************************/
//MonitorGreenFlg
//緑色を感知しているセンサのフラグ(0/1)をプリントする関数です。
INLINE void MonitorGreenFlg(bool printEnable){
        if(printEnable) {
                for (uint8_t i = 0; i < 16; i++) {
                        pc.putc(bitRead(1,lineOnGreen[i]));
                }
        }
}
/**************************************************************************************************/
//MonitorGreenFlg
//空中と判断しているセンサのフラグ(0/1)をプリントする関数です。
INLINE void MonitorAirFlg(bool printEnable){
        if(printEnable) {
                for (uint8_t i = 0; i < 16; i++) {
                        pc.putc(bitRead(1,lineOnAir[i]));
                }
        }
}
/**************************************************************************************************/
//CallBack
//メインにシリアルでラインセンサの状態を送信する関数
void CallBack(){
        // Main.putc(data % 256);
        // Main.putc(data / 256);
        //なんか書いてください・
        //呼び出す部分も書いてください
        //定期的にプリントとかならpidとかしやすそう

}
/**************************************************************************************************/
//setup
//設定や最初に実行する処理をここに買いてください
void setup(){
        pc.baud(230400);//シリアルデバッグする方のボーレート
        Main.baud(460800);//メインに送るボーレート
        LEDS.period_us(100);//LEDドライバのPWM周期
        timer.reset();
        timer.start();
        // for (size_t i = 0; i < 16; i++) {
        //         lineSensors_D[i].mode(PullDown);
        // }
        for (float i = 0; i < 1; i = i + 0.01) {
                LEDS = i; wait_ms(10);
        }
        for (uint8_t i = 0; i < 2; i++) {
                LEDS = 1.0; wait_ms(100);
                LEDS = 0.0; wait_ms(100);
        }
        LEDS=1;
}
/**************************************************************************************************/
int main() {
        pc.printf("Angel Line Sensor!!\r\n");
        setup();
        while(1) {
                // if(a%250 == 0){
                //         LEDS = !LEDS;
                // }
                a++;
                ReadLine();
                JudgeOnWhat();
                LineFlgSubstitution();
                MonitorAnalog(1);
                MonitorWhiteFlg(0);
                MonitorGreenFlg(0);
                MonitorAirFlg(0);
                timer.stop();
                pc.printf("time:%d\r\n",timer.read_us());
        }
}
