#ifndef LGANGELLINE_H
#define LGANGELLINE_H
#include "mbed.h"
#include "Vector.h"

// #define LINE_TOMIX

#define Pi 3.1415926535
#define center_init 8  //センサー番号0の場所を一番前に合わせる為の数字(何個ずれているのかの数字)

class LGAngelLine{
    public:
    LGAngelLine(PinName BUS1,PinName BUS2,PinName BUS3,PinName BUS4,PinName BUS5,PinName DATA,uint16_t *_dataThr);
    void checkThr();    //閾値確認用の関数です。EPROM使う時も使える
    void readLine(); //正確には緑のところを読んでいますがセンサーが白線を踏んでいるのかどうかの処理
    void sensorManage();  //一番緑の上に乗っているセンサーが多い集団の端を求める関数
    void lineManage();  //集団の端からベクトル使って戻るべき方向を求める。これを使えばライン制御は大丈夫なはず

    //今はとりあえずデバッグのためにパブリックにしてまーす
    uint8_t sensorCount;
    bool onGreenFlag[32];   //もし緑の上に乗っているならTrueを入れる
    uint16_t data[32];  //とりあえず現在のフォトトランジスタの値
    uint16_t maxData[32];   //checkThrので使います
    const float sensorVector[32][2]={//[0]はcosの値、[1]はsinの値を格納しています。
    {1.000000, 0.000000},
    {0.980785, 0.195090},
    {0.923880, 0.382683},
    {0.831470, 0.555570},
    {0.707107, 0.707107},
    {0.555570, 0.831470},
    {0.382683, 0.923880},
    {0.195090, 0.980785},
    {0.000000, 1.000000},
    {-0.195090, 0.980785},
    {-0.382683, 0.923880},
    {-0.555570, 0.831470},
    {-0.707107, 0.707107},
    {-0.831470, 0.555570},
    {-0.923880, 0.382683},
    {-0.980785, 0.195090},
    {-1.000000, 0.000000},
    {-0.980785, -0.195090},
    {-0.923880, -0.382683},
    {-0.831470, -0.555570},
    {-0.707107, -0.707107},
    {-0.555570, -0.831470},
    {-0.382683, -0.923880},
    {-0.195090, -0.980785},
    {-0.000000, -1.000000},
    {0.195090, -0.980785},
    {0.382683, -0.923880},
    {0.555570, -0.831470},
    {0.707107, -0.707107},
    {0.831470, -0.555570},
    {0.923880, -0.382683},
    {0.980785, -0.195090},
    };
    uint8_t sensorBegin;
    uint8_t sensorFinish;

    int lineDegree;
    int last_lineDegree;
    int result;//出るべき方向-180~180
    float lineVector[2];
    float last_lineVector[2];
    float naiseki;
    bool reverse_flag;



    private:
    bool sensorFlag;//なにこれ

    uint8_t sensorNum;
    uint8_t sensorBeginTem;
    uint8_t sensorFinishTem;
    uint8_t sensorCountTem;
    uint16_t *dataThr;

    BusOut channel;
    AnalogIn analog;
};

#endif
