#include "LGLineManage_v2.h"
#include "mbed.h"

LineManage::LineManage(PinName LF,PinName LR,PinName LB,PinName LL,RawSerial *serial) : lineF(LF),lineR(LR),lineB(LB),lineL(LF){
        pc = serial;
        lineRead.attach(mbed::callback(this,&LineManage::LineCheck),1.0 / 12000);
}


void LineManage::LineCheck(){
        lineStates[0] = lineF.read();
        lineStates[1] = lineR.read();
        lineStates[2] = lineB.read();
        lineStates[3] = lineL.read();

        if(ON_THE_LINE) {
                if(FIRST_LINE_IS_NONE) {
                        for (uint8_t i = 0; i < 4||lineWhiteNum[0]==0; i++) {
                                if(lineStates[i]) {
                                        //1前,2右,3後ろ,4左
                                        lineWhiteNum[0] = (i+1);//ラインセンサ番号を代入
                                }
                        }
                        order += 1000*lineWhiteNum[0];
                }else if(SECOND_LINE_IS_NONE) {
                        for (uint8_t i = 0; i < 4||lineWhiteNum[1]==0; i++) {
                                if(i != (lineWhiteNum[0]-1)) {//1回目ライン踏んだ番号を確かめる回が回ってきてないかどうか
                                        if(lineStates[i]) {
                                                //1前,2右,3後ろ,4左
                                                lineWhiteNum[1] = (i+1); //ラインセンサ番号を代入
                                        }
                                }
                        }
                        order += 100*lineWhiteNum[1];
                }else if(THIRD_LINE_IS_NONE) {
                        for (uint8_t i = 0; i < 4||lineWhiteNum[2]==0; i++) {
                                if(i != (lineWhiteNum[0]-1)||i != (lineWhiteNum[1]-1)) {//1,2回目ライン踏んだ番号を確かめる回が回ってきてないかどうか
                                        if(lineStates[i]) {
                                                //1前,2右,3後ろ,4左
                                                lineWhiteNum[2] = (i+1); //ラインセンサ番号を代入
                                        }
                                }
                        }
                        order += 10*lineWhiteNum[2];
                }else if(THIRD_LINE_IS_NONE) {
                        for (uint8_t i = 0; i < 4||lineWhiteNum[3]==0; i++) {
                                if(i != (lineWhiteNum[0]-1)||i != (lineWhiteNum[1]-1)||i != (lineWhiteNum[0]-1)) {//1,2回目ライン踏んだ番号を確かめる回が回ってきてないかどうか
                                        if(lineStates[i]) {
                                                //1前,2右,3後ろ,4左
                                                lineWhiteNum[3] = (i+1); //ラインセンサ番号を代入
                                        }
                                }
                        }
                }
                order += lineWhiteNum[3];
                isOnLine = 1;
        }else{
                //踏んでない
                isOnLine = 0;
        }

        //深さ
        // 最初が1なら対は3
        // 最初が2なら対は4
        lineProgress = 0;
        for (uint8_t i = 0; i < 4; i++) {
                if(lineWhiteNum[i] != 0) {
                        lineProgress  = i+1;
                }
        }
        // lineDepth;
        if(lineWhiteNum[0] == 1) {
                switch (lineProgress) {
                case 1: lineDepth = 1; break;
                case 2: lineDepth = 2; break;
                case 3: if(order == 1240 || order == 1420) lineDepth = 3; break;
                case 4: if(order == 1243 || order == 1423) lineDepth = 4; break;
                }
        }else if(lineWhiteNum[0] == 2) {
                switch (lineProgress) {
                case 1: lineDepth = 1; break;
                case 2: lineDepth = 2; break;
                case 3: if(order == 2130 || order == 2310) lineDepth = 3; break;
                case 4: if(order == 2134 || order == 2314) lineDepth = 4; break;
                }
        }else if(lineWhiteNum[0] == 3) {
                switch (lineProgress) {
                case 1: lineDepth = 1; break;
                case 2: lineDepth = 2; break;
                case 3: if(order == 1240 || order == 1420) lineDepth = 3; break;
                case 4: if(order == 1243 || order == 1423) lineDepth = 4; break;
                }
        }else if(lineWhiteNum[0] == 4) {
                switch (lineProgress) {
                case 1: lineDepth = 1; break;
                case 2: lineDepth = 2; break;
                case 3: if(order == 4130 || order == 4310) lineDepth = 3; break;
                case 4: if(order == 4132 || order == 4312) lineDepth = 4; break;
                }
        }else{
                //0
        }

        //LINE RAW
        lineStateRaw16 = lineStates[0]+ (lineStates[1]*2) + (lineStates[2]* 4) + (lineStates[3] * 8);
        //踏んでいても踏んでいなくても処理する
        switch (lineStateRaw16) {

        case 1:     lineStateRaw360 =  0;   break;     //F
        case 2:     lineStateRaw360 = -90;  break;     //R
        case 3:     lineStateRaw360 = -45;  break;     //FR
        case 4:     lineStateRaw360 =  180; break;     //B
        case 5:     lineStateRaw360 =  360; break;     //error
        case 6:     lineStateRaw360 = -135; break;     //RB
        case 7:     lineStateRaw360 =  0;   break;     //FRB
        case 8:     lineStateRaw360 =  90;  break;      //L
        case 9:     lineStateRaw360 =  45;  break;     //FL
        case 10:    lineStateRaw360 =  360; break;     //error
        case 11:    lineStateRaw360 =  0;   break;      //FRL
        case 12:    lineStateRaw360 =  135; break;     //BL
        case 13:    lineStateRaw360 =  90;  break;      //FBL
        case 14:    lineStateRaw360 =  180; break;     //RBL
        case 15:    lineStateRaw360 =  360; break;     //error
        }


        #ifdef _LINE_DEBUG_
        pc->printf("LineState:%d%d%d%d\tfirst:%d second:%d third:%d forth:%d LineState360:%d\r\n",lineStates[0],lineStates[1],lineStates[2],lineStates[3],lineWhiteNum[0],lineWhiteNum[1],lineWhiteNum[2],lineWhiteNum[3],lineStateRaw16);
        #endif
}



void LineManage::SetLine(VectorXY *vector,VectorXY *irVector,VectorXY *lineVector,float *_dir,int8_t *_OwnX,int8_t *_OwnY,uint8_t _OwnW,int8_t *_OpsX,int8_t *_OpsY,uint8_t _OpsW){
        //カメラ,方位,後ろの距離センサ,ボールの角度
        // 深さを読み取るプロセスと戻る方向のプロセスを分ける

}
