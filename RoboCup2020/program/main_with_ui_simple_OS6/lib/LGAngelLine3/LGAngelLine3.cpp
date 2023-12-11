#include "LGAngelLine3.h"
#include "mbed.h"

LGAngelLine::LGAngelLine(PinName BUS1,PinName BUS2,PinName BUS3,PinName BUS4,PinName BUS5,PinName DATA,uint16_t *_dataThr,RawSerial *_pc) : channel(BUS1,BUS2,BUS3,BUS4,BUS5),analog(DATA){
        dataThr = _dataThr;
        onWhiteSensorQty = 0;
        isOnLine = 0;
        pc = _pc;
        lineWait.start();
        moveDeligateT.start();
}

//OK
void LGAngelLine::checkThr(){ //閾値確認用の関数です。EPROM使う時も使える
        for (uint8_t i = 0; i < LINESENSOR_QTY; i++) {
                if (i < CENTER_INIT) {
                        channel = i + LINESENSOR_QTY - CENTER_INIT;
                }else{
                        channel = i - CENTER_INIT;
                }
                data[i] = (uint16_t)(analog.read_u16()/65.535);
                if(data[i] > dataThr[i]) {dataThr[i] = data[i] + 120;}
        }
}

//OK
void LGAngelLine::readLine(){ //正確には緑のところを読んでいますがセンサーが白線を踏んでいるのかどうかの処理
        uint8_t k = 0;
        onWhiteSensorQty = 0;
        for (uint8_t i = 0; i < LINESENSOR_QTY; i++) { //0が変なところから始まっているので正面にする
                if (i < CENTER_INIT) {
                        channel = i + LINESENSOR_QTY - CENTER_INIT;
                }else{
                        channel = i - CENTER_INIT;
                }
                data[i] = (uint16_t)(analog.read_u16()/65.535);
                onGreenFlag[i] = data[i] < dataThr[i];
                onWhiteFlag[i] = !(data[i] < dataThr[i]);
                onWhiteSensorQty += onWhiteFlag[i];//白線の上に乗っているセンサの数剛家
                if(onWhiteFlag[i] == true) numSensorOnWhite[k] = i; k++;
        }
        lastOnLineFlag = isOnLine;
        isOnLine = onWhiteSensorQty > 0;
}


void LGAngelLine::lineConpute(){
        borderBetweenPosQty = 0;
        intervalOnWhite_Max = 0;
        if(isOnLine) {
                for (uint8_t i = 0; i < LINESENSOR_QTY-1; i++) {
                        relationNextTo[i] = onWhiteFlag[i] + onWhiteFlag[i+1];//階差数列的なことをしてる
                        if(relationNextTo[i] == 1) {
                                borderBetweenAddress[borderBetweenPosQty] = i;
                                borderBetweenPosQty++;
                        }
                        // pc->printf("relationNextTo%d\t:%d\r\n",i,relationNextTo[i]);
                }
                {//32
                        relationNextTo[LINESENSOR_QTY-1] = onWhiteFlag[0] + onWhiteFlag[LINESENSOR_QTY-1];//32
                        if(relationNextTo[LINESENSOR_QTY-1] == 1) {
                                borderBetweenAddress[borderBetweenPosQty] = LINESENSOR_QTY-1;
                                borderBetweenPosQty++;
                        }
                        // pc->printf("relationNextTo\t31:%d\r\n",relationNextTo[LINESENSOR_QTY-1]);
                }
                for(uint8_t i = 0; i < onWhiteSensorQty-1; i++) {
                        intervalOnWhite[i] = numSensorOnWhite[i + 1] - numSensorOnWhite[i];         //負のindex参照できないからn+1番目とnの差を出す
                }
                intervalOnWhite[onWhiteSensorQty-1] = numSensorOnWhite[0] - numSensorOnWhite[onWhiteSensorQty+1] + LINESENSOR_QTY;         //32番目の例外処理
                for(uint8_t i = 0; i < onWhiteSensorQty; i++) {
                        if(intervalOnWhite_Max < intervalOnWhite[i]) {
                                intervalOnWhite_Max = intervalOnWhite[i];
                                if(intervalOnWhite_Max > LINESENSOR_QTY/2) intervalOnWhite_Max = abs(LINESENSOR_QTY - intervalOnWhite_Max);         //センサーの間隔の個数計算、32/2の16を最大にしたい。
                        }
                }

                // pc->printf("BorderBetQty:%d\t",borderBetweenPosQty);
                // for (uint8_t i = 0; i < borderBetweenPosQty; i++){
                //         pc->printf("Ad:%d ",borderBetweenAddress[i]);
                // }
                clusterQty = borderBetweenPosQty/2;//という性質を導き出した
                uint8_t clusterEndCount = 0; uint8_t clusterBeginCount = 0;
                for (uint8_t i = 0; i < borderBetweenPosQty; i++) {
                        if(onWhiteFlag[borderBetweenAddress[i]] == 1 && onWhiteFlag[borderBetweenAddress[i+1]] == 0) {
                                clusterEnd[clusterEndCount] = borderBetweenAddress[i];
                                if(borderBetweenAddress[i] == LINESENSOR_QTY) borderBetweenAddress[i] = 0; //32
                                clusterEndPos[clusterEndCount][0] = sensorVector[clusterEnd[clusterEndCount]][0];
                                clusterEndPos[clusterEndCount][1] = sensorVector[clusterEnd[clusterEndCount]][1];
                                clusterEndCount++;
                        }else if(onWhiteFlag[borderBetweenAddress[i]] == 0 && onWhiteFlag[borderBetweenAddress[i+1]] == 1) {
                                clusterBegin[clusterBeginCount] = borderBetweenAddress[i]+1;
                                if(borderBetweenAddress[i] == LINESENSOR_QTY) borderBetweenAddress[i] = 0; //32
                                clusterBeginPos[clusterBeginCount][0] = sensorVector[clusterBegin[clusterBeginCount]][0];
                                clusterBeginPos[clusterBeginCount][1] = sensorVector[clusterBegin[clusterBeginCount]][1];
                                clusterBeginCount++;
                        }
                }
                // for (size_t i = 0; i < clusterBeginCount; i++) {
                //         pc->printf ("begin:%d\t",clusterBegin[i]);
                // }
                // for (size_t i = 0; i < clusterEndCount; i++) {
                //         pc->printf ("end:%d\t",clusterEnd[i]);
                // }
                // pc->printf("beginQ :%d \ tendQ:%d \ t ",clusterBeginCount,clusterEndCount);
                // pc->printf("BorderBetQty :%d \ t ",borderBetweenPosQty);
                int32_t vectorXYAveCluster[clusterQty][2]; //[0]X [1]Y 集団ごとのXY平均値
                lineCurrentX = 0; lineCurrentY = 0;
                for (uint8_t i = 0; i < clusterQty; i++) {
                        vectorXYAveCluster[i][0] = (clusterBeginPos[i][0] + clusterEndPos[i][0])/2;
                        vectorXYAveCluster[i][1] = (clusterBeginPos[i][1] + clusterEndPos[i][1])/2;
                        lineCurrentX += vectorXYAveCluster[i][0]; lineCurrentY += vectorXYAveCluster[i][1];
                }
                lineCurrentX /= clusterQty; lineCurrentY /= clusterQty;
                // if(lineCurrentX != 0 && lineCurrentY != 0) {//白線踏んでいてX,Y=0はバグ(仕様)なので
                lineCurrentDegree = atan2(lineCurrentX,lineCurrentY) * 180 / PI;

                linePreviousDegree = lineCurrentDegree;
                // pc->printf("Deg:%d\tQty:%d\t ",lineCurrentDegree,onWhiteSensorQty);
                // linePreviousX = lineCurrentX; linePreviousY = lineCurrentY;
                // innerProduct = (uint16_t)((uint32_t)(lineCurrentX*lineFirstX+lineCurrentY*lineFirstY)/(sqrt(lineCurrentX*lineCurrentX+lineCurrentY*lineCurrentY)*(sqrt(lineFirstX*lineFirstX+lineFirstY*lineFirstY))));//内積
                errorDeg = lineFirstDegree - lineCurrentDegree;
                if(errorDeg < 0) errorDeg+=360;
                if(errorDeg > 180) errorDeg -= 360;
                errorDeg = abs(errorDeg);
                lineDegree = lineCurrentDegree * AVE_CONSTANT_NUM + linePreviousDegree * (1 - AVE_CONSTANT_NUM);
                if(errorDeg >= 90) {
                        isHalfOut = true; lineDegree += 180;
                }else{
                        isHalfOut = false;
                }
        }else{
                linePreviousDegree = lineCurrentDegree = 0;
                lineCurrentX = 0; lineCurrentY = 0;
        }
        if(lastOnLineFlag != isOnLine && isOnLine) {
                lineFirstDegree = lineCurrentDegree;
                // onLineFirstV.x = lineCurrentX; onLineFirstV.y = lineCurrentY;
                isOutsideLine = false;
        }else if(lastOnLineFlag != isOnLine && isOnLine == 0 && errorDeg >= 90) {
                lineFinishDegree = linePreviousDegree;
                isOnLine = true;
                isOutsideLine = true;
        }
        // currentLineVPer45.x = currentLineVPer45.y = 0;

        #ifdef DEBUG_ANGEL
        pc->printf("C deg:%d\tF deg:%d\tinner Product:%d\terrorDeg:%d\tX:%d\tY:%d\tPer45:%d\r\n",lineCurrentDegree,lineFirstDegree,innerProduct,errorDeg,currentLineVPer45.x,currentLineVPer45.y,lineCurrentDegreePer45);
        #endif
}

void LGAngelLine::manage(VectorXY *DriveV,VectorXY *IRv,int16_t irDeg,int16_t compDir){
        // X = xcosθ - ysinθ
        // Y = xsinθ - ycosθ

        // IRv->x = (IRv->x*cos32_T(-compDir) - IRv->y*sin32_T(-compDir));
        // IRv->y = -(IRv->x*sin32_T(-compDir) - IRv->y*cos32_T(-compDir));
        // lineDegree_CompassRotate = lineDegree;
        // driveDeg = (int16_t)atan2(DriveV->x,DriveV->y) * 180 / PI;

        lineDegree_CompassRotate = lineDegree + compDir;
        if(lineDegree_CompassRotate < 0) lineDegree_CompassRotate+=360;
        if(lineDegree_CompassRotate > 180) lineDegree_CompassRotate -= 360;

        if(isOnLine) {//60度ずつが普通
                if(abs(lineDegree_CompassRotate) <= 40) currentLineVPer45.y = 100;
                if(abs(90 - lineDegree_CompassRotate) <= 60) currentLineVPer45.x = -100;
                if(abs(-90 - lineDegree_CompassRotate) <= 60) currentLineVPer45.x = 100;
                if(abs(lineDegree_CompassRotate) <= 180 && abs(lineDegree_CompassRotate) >= 140) currentLineVPer45.y = -100;
        }
        // currentLine_CompassRotate.x = (short)(currentLineVPer45.x*cos32_T(compDir) - currentLineVPer45.y*sin32_T(compDir));
        // currentLine_CompassRotate.y = -(short)(currentLineVPer45.x*sin32_T(compDir) - currentLineVPer45.y*cos32_T(compDir));
        if(isOnLine) {//|| (abs(lineDegree_CompassRotate) < 60 && isOnLine) || (abs(lineDegree_CompassRotate) > 120 && isOnLine
                lineWait.reset(); moveDeligateT.reset();
                DriveXCount = 0; DriveYCount = 0; manageIgnore = 0;
                *DriveV = makeXYFromRT(100,lineDegree+180);
        }else{
                if(lineWait.read_ms() < 750) {
                        if(IRv->x > 0 && currentLineVPer45.x > 0 && DriveV->x > 70 && manageIgnore == 0) {
                                DriveXCount > -30 ? manageIgnore = 1 : DriveV->x = 0;
                        }
                        if(IRv->x < 0 && currentLineVPer45.x < 0 && DriveV->x < 70 && manageIgnore == 0) {
                                DriveXCount < 30 ? manageIgnore = 1 :  DriveV->x = 0;
                        }
                        if(IRv->y > 0 && currentLineVPer45.y > 0 && DriveV->y > 70 && manageIgnore == 0) {
                                DriveYCount > -30 ?  manageIgnore = 1 : DriveV->y = 0;
                        }
                        if(IRv->y < 0 && currentLineVPer45.y < 0 && DriveV->y < 60 && manageIgnore == 0) {
                                DriveYCount < 30 ? manageIgnore = 1 : DriveV->y = 0;
                        }
                        if(moveDeligateT.read_ms() > 1) {
                                if(DriveV->x > 30) DriveXCount++;
                                if(DriveV->x < -30) DriveXCount--;
                                if(DriveV->y > 30) DriveYCount++;
                                if(DriveV->y < -30) DriveYCount--;
                                moveDeligateT.reset();
                        }
                }else if(lineWait.read_ms() < 1000 && manageIgnore == 0) {
                        if(abs(irDeg - lineDegree) < 45) {
                                DriveV->x *= 0.5; DriveV->y *= 0.5;
                        }
                }
        }
        pc->printf("X:%d\tY:%d\r\n",DriveXCount,DriveYCount);
        // DriveV->x = (DriveV->x*cos32_T(-compDir) - DriveV->y*sin32_T(-compDir));
        // DriveV->y = -(DriveV->x*sin32_T(-compDir) - DriveV->y*cos32_T(-compDir));

        // DriveV->x = (DriveV->x*cos32_T(compDir) - DriveV->y*sin32_T(compDir));
        // DriveV->y = -(DriveV->x*sin32_T(compDir) - DriveV->y*cos32_T(compDir));
        // pc->printf("C deg:%d\tR deg:%d\tLX:%d\tLY:%d\tBX:%d\tBY:%d\tDX:%d\tDY:%d\r\n",lineCurrentDegree,lineDegree_CompassRotate,currentLineVPer45.x,currentLineVPer45.y,IRv->x,IRv->y,DriveV->x,DriveV->y);
}

// if(abs(lineDegree) <=30) {
//         if(DriveV->y > 0) DriveV->y = 0;
// }else if(abs(45 - lineDegree) <= 30) {
//         if(DriveV->y > 0) DriveV->y = 0;
//         if(DriveV->x < 0) DriveV->x = 0;
// }else if(abs(90 - lineDegree) <= 30) {
//         if(DriveV->x < 0) DriveV->x = 0;
// }else if(abs(135 - lineDegree) <= 30) {
//         if(DriveV->y < 0) DriveV->y = 0;
//         if(DriveV->x < 0) DriveV->x = 0;
// }else if(abs(-45 - lineDegree) <= 30) {
//         if(DriveV->y > 0) DriveV->y = 0;
//         if(DriveV->x < 0) DriveV->x = 0;
// }else if(abs(-90 - lineDegree) <= 30) {
//         if(DriveV->x < 0) DriveV->x = 0;
// }else if(abs(135 - lineDegree) <= 30) {
//         if(DriveV->y < 0) DriveV->y = 0;
//         if(DriveV->x < 0) DriveV->x = 0;
// }else if(abs(lineDegree) <= 180 && abs(lineCurrentDegree) >= 150) {
//         if(DriveV->y < 0) DriveV->y = 0;
// }