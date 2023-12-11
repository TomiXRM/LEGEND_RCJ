#include "LGAngelLine2.h"
#include "mbed.h"

LGAngelLine::LGAngelLine(PinName BUS1,PinName BUS2,PinName BUS3,PinName BUS4,PinName BUS5,PinName DATA,uint16_t *_dataThr,RawSerial *_pc) : channel(BUS1,BUS2,BUS3,BUS4,BUS5),analog(DATA){
        onWhiteV.x = 0;
        onWhiteV.y = 0;
        dataThr = _dataThr;
        onWhiteSensorQty = 0;
        pc = _pc;
}

//OK
void LGAngelLine::checkThr(){ //閾値確認用の関数です。EPROM使う時も使える
        for (uint8_t i = 0; i < 32; i++) {
                if (i < center_init) {
                        channel = i + 32 - center_init;
                }else{
                        channel = i - center_init;
                }
                data[i] = analog.read_u16();
                if(data[i] > maxData[i]) {maxData[i] = data[i];}
        }
}

//OK
void LGAngelLine::readLine(){ //正確には緑のところを読んでいますがセンサーが白線を踏んでいるのかどうかの処理
        onWhiteSensorQty = 0;
        for (uint8_t i = 0; i < 32; i++) { //0が変なところから始まっているので正面にする
                if (i < center_init) {
                        channel = i + 32 - center_init;
                }else{
                        channel = i - center_init;
                }
                data[i] = (uint16_t)(analog.read_u16()/65.535);
                onGreenFlag[i] = data[i] < dataThr[i];
                onWhiteFlag[i] = !(data[i] < dataThr[i]);
                onWhiteSensorQty += onWhiteFlag[i];//白線の上に乗っているセンサの数剛家
        }
}


void LGAngelLine::lineConpute(){
        if(onWhiteSensorQty == 0) {
                int16_t numSensorOnWhite[onWhiteSensorQty];//白線を踏んでいるセンサのアドレス
                int16_t sensorOnWhitePosiX[onWhiteSensorQty];//白線を踏んでいるセンサの位置X
                int16_t sensorOnWhitePosiY[onWhiteSensorQty]; //位置X
                uint8_t k = 0;//for的なこと
                for (uint8_t i = 0; i < 32; i++) {//白色を踏んでいるセンサの位置情報XYをゲットしたい
                        if(onWhiteFlag[i] == 1) {//onWhiteSensorQty個しかないので、複雑ですが耐え
                                numSensorOnWhite[k] = i;//白線を踏んでいるどこ番目のラインかなというセンサ番号代入
                                sensorOnWhitePosiX[k] = sensorVector[numSensorOnWhite[k]][0];//白色踏んでるセンサの位置X
                                sensorOnWhitePosiY[k] = sensorVector[numSensorOnWhite[k]][1];//白色踏んでるセンサの位置Y
                                k++;//for的なことがしたいためのカウンターkインクリメント
                        }
                }
                //白色センサ位置情報XYをゲットしたので次は白線ONのセンサー位置の距離計算をしたい。これで連続かどうかを調べたい。けどそれってセンサー番号で良くない？
                //というわけで上で作ったせんさーの位置情報XYのやつ使いません...
                //とりあえず、白線を踏んでいるセンサーのnumber(番号)を格納したnumSensorOnWhiteを使えば連続してるかどうかがわかる
                //→集団(cluster)が発生しているかがわかる。
                //集団で平均を取って、集団同士の位置平均をすることで正しい角度を導けそうと判断した。
                //とりあえずclusterを認識できるようなプログラム書きます。
                //NOTE:0と31は隣ということを気を付ける。
                int16_t intervalSensorOnWhite[onWhiteSensorQty];//白線の上のセンサ同士の間隔
                for(uint8_t i = 0; i < onWhiteSensorQty-1; i++) {
                        intervalSensorOnWhite[i] = numSensorOnWhite[i + 1] - numSensorOnWhite[i];//負のindex参照できないからn+1番目とnの差を出す
                }
                intervalSensorOnWhite[onWhiteSensorQty-1] = numSensorOnWhite[0] - numSensorOnWhite[onWhiteSensorQty+1] + LINESENSOR_QTY;//32番目の例外処理
                // pc->printf("\033[2J");//くりあ
        #ifdef DEBUG_ANGEL
        #ifdef DEGUB_ANGEL_ONLINE_SENSORNUM//ONホワイトラインのセンサー番号(可変)
                for (uint8_t i = 0; i < onWhiteSensorQty; i++) {
                        pc->printf("on:%d\t",numSensorOnWhite[i]);
                }
                pc->printf("\r\n");
        #endif
        #ifdef DEGUB_ANGEL_ONLINE_SENSOR_INTERVAL
                for (uint8_t i = 0; i < onWhiteSensorQty-1; i++) {
                        pc->printf("intrvl:%d\t",intervalSensorOnWhite[i]);
                }
                pc->printf("\r\n");
        #endif
        #endif

                //ONWHITEな近隣のセンサとの距離計算おわり
                //次はどこ番目のセンサ同士の感覚が離れているのかの計算、これでcluster計算に
                int16_t intervalOnWhite_Max = 0;//数値のMax
                int16_t posIntervalMaxNum = 0;//場所記憶
                for(uint8_t i = 0; i < onWhiteSensorQty; i++) {
                        if(intervalOnWhite_Max < intervalSensorOnWhite[i]) {
                                intervalOnWhite_Max = intervalSensorOnWhite[i];
                                if(intervalOnWhite_Max > 16) intervalOnWhite_Max = 32 - intervalOnWhite_Max; //センサーの間隔の個数計算、32/2の16を最大にしたい。
                                //NOTE intervalOnWhite_Maxについて
                                //onWhiteSensorQty == 1の時 intervalOnWhite_Maxは差がないので0
                                //2個以上から0以外の数値が入る
                                posIntervalMaxNum = i;
                        }
                }
                lineCurrentDegree = ((posIntervalMaxNum + intervalOnWhite_Max/2)*360)/LINESENSOR_QTY;
                linePreviousDegree = lineCurrentDegree;
        #ifdef DEBUG_ANGEL
        #ifdef DEGUB_ANGEL_ONLINE_SENSOR_INTERVAL_MAX
                pc->printf("onWhiteQty:%d MAX:%d\tMAX_POS:%d\t",onWhiteSensorQty,intervalOnWhite_Max,posIntervalMaxNum);//
        #endif
        #endif


                // onWhiteV.x /= onWhiteSensorQty;
                // onWhiteV.y /= onWhiteSensorQty;
                // lineCurrentDegree =  atan2(onWhiteV.y,onWhiteV.x) * 180 / PI;
        #ifdef DEBUG_ANGEL
                pc->printf("CurrentDeg:%d\r\n",lineCurrentDegree);
        #endif
        }
        //白線踏んでないですなう。
}
//LDF : Library Dependency Finder->http: //bit.ly/configure-pio-ldf
//LDF Modes: Finder ~chain, Compatibility ~soft
//メモ
//一つ前のlineのベクトルと今のlineのベクトルとの差分を求めたら進行方向がわかる説


//cluster chack
//数列的にすればbeforeCurrentの違いいらない案件。
// uint8_t numNeighborBefore[2];//前回 となりはNeighbor [0]→X [1]→Y
// uint8_t numNeighborCurrent[2];//今回 [0]→X [1]→Y
// int16_t numNeighborError[3];//センサー番号の差(error) [0]X [1]Y [3]R
// uint8_t clusterQty;//集団の数
// //1回目は隣とかいう比べる対象が存在しないので1つめをbeforeに予め代入しとく
// numNeighborBefore[0] = sensorVector[numSensorOnWhite[0]][0];//X
// numNeighborBefore[1] = sensorVector[numSensorOnWhite[0]][1];//Y
// if(onWhiteSensorQty > 1) {//一個以上のセンサーが反応していないと比べる対象もない,無視
//         for (uint8_t i = 1; i < onWhiteSensorQty; i++) { //1回目は隣とかいう比べる対象が存在しないので2回目から
//                 numNeighborCurrent[0] = sensorVector[numSensorOnWhite[0]][0];//X
//                 numNeighborCurrent[1] = sensorVector[numSensorOnWhite[0]][1];//Y

//                 //差の計算
//                 numNeighborError[0] = (numNeighborCurrent[0] - numNeighborBefore[0]);
//                 numNeighborError[1] = (numNeighborCurrent[1] - numNeighborBefore[1]);
//                 numNeighborError[3] = sqrt((numNeighborError[0]^2) + (numNeighborError[1]^2));
//                 //1960はエクセルで計算したら出てきた値、11.25度刻みの曲座標R=1のとき0と11.25度の間は0.1960 の10000枚でint16_t格納にした
//                 if(numNeighborError[3] > 1960) { clusterQty++;}//白線反応してるセンサー番号の間隔が1960以上だとクラスター複数個目発生
//                 numNeighborBefore[0] = numNeighborCurrent[0];
//                 numNeighborBefore[1] = numNeighborCurrent[1];
//         }

// }
// for (uint8_t i = 0; i < onWhiteSensorQty; i++) {
//         onWhiteV.x += sensorPosiX[i];
//         onWhiteV.y += sensorPosiY[i];
// }