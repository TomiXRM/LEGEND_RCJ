#include "LGAngelLine.h"
#include "mbed.h"

LGAngelLine::LGAngelLine(PinName BUS1,PinName BUS2,PinName BUS3,PinName BUS4,PinName BUS5,PinName DATA,uint16_t *_dataThr) : channel(BUS1,BUS2,BUS3,BUS4,BUS5),analog(DATA){
        sensorFlag = false;
        sensorCount = 0;
        lineDegree = 0;
        lineVector[0] = 0;
        lineVector[1] = 0;
        dataThr = _dataThr;
        reverse_flag = false;
        result = 0;
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
        for (uint8_t i = 0; i < 32; i++) { //0が変なところから始まっているので正面にする
                if (i < center_init) {
                        channel = i + 32 - center_init;
                }else{
                        channel = i - center_init;
                }
                data[i] = (uint16_t)(analog.read_u16()/65.535);
                if(data[i] < dataThr[i]) {
                        onGreenFlag[i] = true;
                }else{
                        onGreenFlag[i] = false;
                }
        }
}

void LGAngelLine::sensorManage(){   //一番緑の上に乗っているセンサーが多い集団の端を求める関数

#ifndef LINE_TOMIX
        sensorNum = 0;
        sensorCount = 0;
        while(onGreenFlag[sensorNum] == true) { //くそ
                if(sensorNum == 0) {
                        sensorNum = 31;
                }else{
                        sensorNum -= 1;
                }
                if (sensorNum == 0) { //白線を踏んでいない状態
                        //pc.printf("no");
                        goto degreeCalEnd;
                }
        }

        //とりあえず問題なのは0と32のセンサーの両方が反応していた場合グループ化すること。
        //vectorにして方向を出すにはとりあえず反応しているセンサーをグループ化して一つのベクトルにしないとvectorが吹っ飛ぶ
        for (uint8_t i = 0; i < 32; i++) {
                if(onGreenFlag[sensorNum] == true && sensorFlag == false) {
                        sensorBeginTem = sensorNum;
                        sensorFlag = true;
                }
                if(onGreenFlag[sensorNum] == false && sensorFlag == true) {
                        if(sensorNum == 0) {
                                sensorFinishTem = 31;
                        }else{
                                sensorFinishTem = sensorNum -1;
                        }
                        sensorFlag = false;
                        if(sensorCountTem > sensorCount) { //最大個数を更新した場合、[始・カウント・終]の数字を更新する
                                sensorBegin = sensorBeginTem;
                                sensorFinish = sensorFinishTem;
                                sensorCount = sensorCountTem;
                        }
                }

                if(sensorFlag == true) {
                        sensorCountTem++;
                }else{
                        sensorCountTem = 0;
                }

                sensorNum++;
                if(sensorNum ==32) {
                        sensorNum = 0;
                }
        }
        //pc.printf("\tC:%d\tS:%d\tF:%d",sensorCount,sensorBegin,sensorFinish);
degreeCalEnd:;
#else

#endif

}

void LGAngelLine::lineManage(){   //集団の端からベクトル使って戻るべき方向を求める。これを使えばライン制御は大丈夫なはず
        readLine(); //ライン読みマース
        sensorManage(); //多い集団の端っこ見つけマース
        //sensorCountでラインの深さ
        last_lineDegree = lineDegree;
        last_lineVector[0] = lineVector[0];
        last_lineVector[1] = lineVector[1];

        lineVector[0] = sensorVector[sensorBegin][0] + sensorVector[sensorFinish][0];
        lineVector[1] = sensorVector[sensorBegin][1] + sensorVector[sensorFinish][1];

        lineDegree =  atan2(lineVector[1],lineVector[0]) * 180 / Pi;

        if(sensorCount == 17) {
                lineDegree = last_lineDegree;
                lineVector[0] = last_lineVector[0];
                lineVector[1] = last_lineVector[1];
        }

        naiseki = lineVector[0]*last_lineVector[0] + lineVector[1]*last_lineVector[1];

        if (naiseki < 0) {reverse_flag = !reverse_flag;}

        // if (reverse_flag == true) {
        //         if(lineDegree > 0) {
        //                 result = lineDegree - 180;
        //         }else{
        //                 result = lineDegree + 180;
        //         }
        // }
        

}

//メモ
//一つ前のlineのベクトルと今のlineのベクトルとの差分を求めたら進行方向がわかる説
