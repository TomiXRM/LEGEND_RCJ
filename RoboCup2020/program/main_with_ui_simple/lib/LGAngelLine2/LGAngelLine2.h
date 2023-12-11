#ifndef LGANGELLINE_H
#define LGANGELLINE_H
#include "mbed.h"
#include "Vector.h"

//#defineLINE_TOMIX
#define PI 3.141592653589
#define center_init 8//センサー番号0の場所を一番前に合わせる為の数字(何個ずれているのかの数字)
#define LINESENSOR_QTY 32//数
#define AVE_NUM 0.3

//CONFIG
#define DEBUG_ANGEL
// #define DEGUB_ANGEL_ONLINE_SENSORNUM
// #define DEGUB_ANGEL_ONLINE_SENSOR_INTERVAL
#define DEGUB_ANGEL_ONLINE_SENSOR_INTERVAL_MAX
#define DEGUB_ANGEL_ONLINE_CURRENT_DEG

class LGAngelLine {
public:
LGAngelLine(PinName BUS1,PinName BUS2,PinName BUS3,PinName BUS4,PinName BUS5,PinName DATA,uint16_t *_dataThr,RawSerial *_pc);
void checkThr(); //閾値確認用の関数です。EPROM使う時も使える
void readLine(); //正確には緑のところを読んでいますがセンサーが白線を踏んでいるのかどうかの処理
//voidsensorManage();//一番緑の上に乗っているセンサーが多い集団の端を求める関数
void lineConpute();
void lineManage(); //集団の端からベクトル使って戻るべき方向を求める。これを使えばライン制御は大丈夫なはず
//今はとりあえずデバッグのためにパブリックにしてまーす
bool onGreenFlag[32]; //もし緑の上に乗っているならTrueを入れる
bool onWhiteFlag[32]; //もし白の上に乗っているならTrueを入れる
uint16_t data[32]; //とりあえず現在のフォトトランジスタの値
uint16_t maxData[32]; //checkThrので使います
uint8_t onWhiteSensorQty; //白線の上のセンサの数合計
int16_t lineCurrentDegree;//現在のラインのベクトル平均からの角度
int16_t linePreviousDegree;
const int16_t sensorVector[32][2]={ //[0]はcosの値、[1]はsinの値を格納しています。
        {0,10000},
        {-1951,9808},
        {-3827,9239},
        {-5556,8315},
        {-7071,7071},
        {-8315,5556},
        {-9239,3827},
        {-9808,1951},
        {-10000,0},
        {-9808,-1951},
        {-9239,-3827},
        {-8315,-5556},
        {-7071,-7071},
        {-5556,-8315},
        {-3827,-9239},
        {-1951,-9808},
        {-0,-10000},
        {1951,-9808},
        {3827,-9239},
        {5556,-8315},
        {7071,-7071},
        {8315,-5556},
        {9239,-3827},
        {9808,-1951},
        {10000,-0},
        {9808,1951},
        {9239,3827},
        {8315,5556},
        {7071,7071},
        {5556,8315},
        {3827,9239},
        {1951,9808}
};
VectorXY onWhiteV;


private:
RawSerial* pc;
uint8_t sensorNum;
uint16_t *dataThr;

BusOut channel;
AnalogIn analog;
};

#endif
