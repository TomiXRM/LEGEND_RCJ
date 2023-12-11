#ifndef LGANGELLINE_H
#define LGANGELLINE_H
#include "mbed.h"
#include "Vector.h"
#include "TrigonometricTable.h"


#define PI 3.141592653589
#define CENTER_INIT 8//センサー番号0の場所を一番前に合わせる為の数字(何個ずれているのかの数字)
#define LINESENSOR_QTY 32//数
#define AVE_CONSTANT_NUM 0.3

//CONFIG
// #define DEBUG_ANGEL


class LGAngelLine {
public:
LGAngelLine(PinName BUS1,PinName BUS2,PinName BUS3,PinName BUS4,PinName BUS5,PinName DATA,uint16_t *_dataThr,RawSerial *_pc);
void checkThr(); //閾値確認用の関数です。EPROM使う時も使える
void readLine(); //正確には緑のところを読んでいますがセンサーが白線を踏んでいるのかどうかの処理
//void sensorManage();//一番緑の上に乗っているセンサーが多い集団の端を求める関数
void lineConpute();
void manage(VectorXY *DriveV,VectorXY *IRv,int16_t irDeg,int16_t compDir);
//今はとりあえずデバッグのためにパブリックにしてまーす
bool onGreenFlag[LINESENSOR_QTY]; //もし緑の上に乗っているならTrueを入れる
bool onWhiteFlag[LINESENSOR_QTY]; //もし白の上に乗っているならTrueを入れる
uint16_t data[LINESENSOR_QTY]; //とりあえず現在のフォトトランジスタの値
// uint16_t maxData[LINESENSOR_QTY]; //checkThrので使います
uint8_t onWhiteSensorQty; //白線の上のセンサの数合計
int8_t numSensorOnWhite[LINESENSOR_QTY];
int16_t intervalOnWhite_Max;
int16_t intervalOnWhite[LINESENSOR_QTY];
int16_t linePreviousDegree;
// int16_t linePreviousX;
// int16_t linePreviousY;
int16_t lineCurrentDegree;//現在のラインのベクトル平均からの角度
int16_t lineCurrentX;
int16_t lineCurrentY;
int16_t lineFirstDegree;
int16_t lineFinishDegree;
int16_t lineDegree;
int16_t innerProduct;//内積
uint8_t relationNextTo[LINESENSOR_QTY];//隣のセンサとの関係 0:どっちも緑　1:どっちか白,境目 2:どっちも白
uint8_t borderBetweenPosQty;//白線ふんでるセンサがでてくるところ、
uint8_t borderBetweenAddress[LINESENSOR_QTY];//あり得んけど
uint8_t clusterBegin[LINESENSOR_QTY];
uint8_t clusterEnd[LINESENSOR_QTY];
int16_t clusterBeginPos[LINESENSOR_QTY][2];//[0]X [1]Y
int16_t clusterEndPos[LINESENSOR_QTY][2];
uint8_t clusterQty;
int8_t intervalBetween;
bool isHalfOut;
bool isOnLine;
bool lastOnLineFlag;
bool isOutsideLine;
int16_t errorDeg;
int16_t lineDegree_CompassRotate;
int16_t driveDeg;

// VectorXY onLineFirstV;
VectorXY currentLine_CompassRotate;
VectorXY currentLineVPer45;
int16_t lineCurrentDegreePer45;
int16_t DriveXCount;
int16_t DriveYCount;
bool manageIgnore;
Timer lineWait;

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

private:
inline void calDegreeAboutPer45();
RawSerial* pc;
uint8_t sensorNum;
uint16_t *dataThr;

BusOut channel;
AnalogIn analog;

Timer moveDeligateT;
};

#endif
