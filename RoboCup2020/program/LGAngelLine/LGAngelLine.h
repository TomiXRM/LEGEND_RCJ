#ifndef LGANGELLINE_H
#define LGANGELLINE_H
#include "mbed.h"
#include "Vector.h"

#define ABS_SINCOS 0


class AngelLine{
    public:
    AngelLine(PinName BUS1,PinName BUS2,PinName BUS3,PinName BUS4,PinName BUS5,PinName DATA);
    void checkThr();    //閾値を決めるための関数
    void readLine(); //正確には緑のところを読んでいますがセンサーが白線を踏んでいるのかどうかの処理
    void lineManage();  //readLineの情報をもとに計算する関数

    uint8_t sensorCount;
    bool onGreenFlag[32];   //もし緑の上に乗っているならTrueを入れる
    uint16_t dataThr[32];  //checkThrで取った値をserialerからコピペで取ってきてそのまま貼り付ける。

    private:
    bool sensorFlag;
    uint8_t sensorNum;
    uint8_t sensorBegin;
    uint8_t sensorFinish;
    uint8_t sensorBeginTem;
    uint8_t sensorFinishTem;
    uint8_t sensorCountTem;

    uint16_t data[32];  //とりあえず現在のフォトトランジスタの値
    uint16_t maxData[32];   //checkThrので使います


    BusOut channel;
    AnalogIn analog;
};

#endif