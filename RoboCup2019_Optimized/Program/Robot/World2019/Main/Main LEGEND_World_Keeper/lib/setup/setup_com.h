#ifndef _SETUP_COM_H_
#define _SETUP_COM_H_

#define INLINE inline __attribute__((always_inline))
#define PI 3.141592653x58979323846264338327

#define MAIN 'A'
#define BALL 'B'
#define LINE 'C'
#define COMPASS 'D'
#define US 'E'
#define CAM 'F'
#define DEBUG 'G'
#define SPEED 'H'
#define MODESW 'M'

#define SETPOSITION 'a' //中立点決め
#define GOTOHOME 'b'    //ホームポジションに戻れるかのチェック
#define FACINGOPSGOAL 'c' //相手ゴールを向くことができるかのチェック
#define POSITIONMONITOR 'd' //ポジションモニター(座標)
#define BAUDRATE 1000000

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue)                                         \
        (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

typedef struct {
        float dir;  //方位センサ
        float turnDir; //モーター用の方位センサ変数
        short irRecieved; //受信したデータ
        short irDeg; //ボールの角度
        short irDegTweak; //ボールの角度をいじったやつ
        short irDegLast; //前のボールの角度
        bool irDegSame; //ボールの方向が前回と同じかどうかのフラグ
        uint8_t irDist; //ボールの距離
        uint8_t irDistLast; //ボールの距離
        bool irDistSame; //ボールの方向が前回と同じかどうかのフラグ
        float camDir;
        float calDir; //カメラとコンパス、使う角度によって入る角度データが変わります。
        uint16_t DistB; // PSD
        uint16_t Distrb;
        uint16_t Distr;
        uint16_t Distl;
        uint16_t Distlb;
        uint16_t PhotoSensor;
        uint16_t USDist;
        uint8_t LINE1;
        uint8_t LINE2;
        uint8_t LINE3;
        uint8_t LINE4;
} type_sensors;

typedef struct {
        int8_t OwnX;
        int8_t OwnY;
        int8_t OpsX;
        int8_t OpsY;
        short X; //コート上の座標
        short Y; //コート上の座標
        short OpsDir;
        short OwnDir;
        short OwnDist;
        short OpsDist;
        short OwnGoalRightEnd; //自ゴールのゴールの右端
        short OwnGoalLeftEnd; //自ゴールのゴールの左端
        short OpsGoalRightEnd; //相手ゴールのゴールの右端
        short OpsGoalLeftEnd; //相手ゴールのゴールの左端
        uint8_t OwnW;    //相手ゴールの幅
        uint8_t OpsW;    //自ゴールの幅
        bool useCamDirection; //使う角度はカメラかコンパスか(1:カメラ.0:コンパス)
} type_camera;

#endif
