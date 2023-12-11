#include "LGAngelLine3.h"
#include "mbed.h"

LGAngelLine::LGAngelLine(PinName BUS1,PinName BUS2,PinName BUS3,PinName BUS4,PinName BUS5,PinName DATA,uint16_t *_dataThr,RawSerial *_pc) : channel(BUS1,BUS2,BUS3,BUS4,BUS5),analog(DATA){
        dataThr = _dataThr;
        onWhiteSensorQty = 0;
        isOnLine = 0;
        pc = _pc;
        lineWait.start();
        moveDeligateT.start();
        slowStart.start();
        //   lineWaitOnstart();
        manageIgnore = 0;
        DriveLCount = 0;
        errorDeg = 0;
        isHalfOut = 0;
        clusterQty = 0;
        lineFirstCenteredDegree = 0;
        if(degHistory.empty()) {
                for(int i = 0; i < DEG_HISTORY_QTY; ++i)
                        degHistory.push_back(0);
        }
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
                if(data[i] > dataThr[i]) {dataThr[i] = data[i] + 50;}
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
                if(onWhiteFlag[i] == true) {
                        numSensorOnWhite[k] = i; k++;
                }
                //     pc->printf("i:%d ",i);
        }
        lastOnLineFlag = isOnLine;
        isOnLine = onWhiteSensorQty > 0;

}


void LGAngelLine::lineConpute(int16_t imuDir){
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
                linePreviousX = lineCurrentX; linePreviousY = lineCurrentY;
                lineCurrentX = 0; lineCurrentY = 0;
                for (uint8_t i = 0; i < clusterQty; i++) {
                        vectorXYAveCluster[i][0] = (clusterBeginPos[i][0] + clusterEndPos[i][0])/2;
                        vectorXYAveCluster[i][1] = (clusterBeginPos[i][1] + clusterEndPos[i][1])/2;
                        lineCurrentX += vectorXYAveCluster[i][0]; lineCurrentY += vectorXYAveCluster[i][1];
                }
                lineCurrentX /= clusterQty; lineCurrentY /= clusterQty;
                lineDepth = sqrt(lineCurrentX*lineCurrentX + lineCurrentY*lineCurrentY);
                // if(lineCurrentX != 0 && lineCurrentY != 0) {//白線踏んでいてX,Y=0はバグ(仕様)なので
                //     alpacoX = (lastOnLineFlag == 1 && abs(lineCurrentX - linePreviousX) < 5000 && abs(lineCurrentX - linePreviousX) != (abs(linePreviousX) - abs(lineCurrentX)) || lineCurrentX == 0);
                //     alpacoY = (lastOnLineFlag == 1 && abs(lineCurrentY - linePreviousY) < 5000 && abs(lineCurrentY - linePreviousY) != (abs(linePreviousY) - abs(lineCurrentY)) || lineCurrentY == 0);
                //     alpacoX = (lastOnLineFlag == 1 && abs(lineDepth) < 2500 &&lineCurrentX*linePreviousX < 0 || lineCurrentX == 0);
                //     alpacoY = (lastOnLineFlag == 1 && abs(lineDepth) < 2500 &&lineCurrentY*linePreviousY < 0 || lineCurrentY == 0);

                linePreviousDegree = lineCurrentDegree;
                degHistory.push_front(linePreviousDegree);
                degHistory.pop_back();
                lineCurrentDegree = atan2(lineCurrentX,lineCurrentY) * 180 / PI + imuDir;
                //     if((alpacoX + alpacoY > 0) && degBetween(lineCurrentDegree,degHistory[15]) > 60 &&degBetween(lineFirstDegree,lineCurrentDegree) > 160 && lineFirstCenteredDegree == 5000) {
                //             int16_t ii = 0;
                //             for (size_t i = 0; i < 16; i++) {
                //                     ii += degHistory[i];
                //             } ii /= 16;

                //             lineFirstCenteredDegree = ii;
                //     }
                // pc->printf("Deg:%d\tQty:%d\t ",lineCurrentDegree,onWhiteSensorQty);
                //     innerProduct = (uint16_t)((uint32_t)(lineCurrentX*lineFirstX+lineCurrentY*lineFirstY)/(sqrt(lineCurrentX*lineCurrentX+lineCurrentY*lineCurrentY)*(sqrt(lineFirstX*lineFirstX+lineFirstY*lineFirstY))));//内積
                //     errorDeg = lineFirstDegree - lineCurrentDegree;
                //     if(lineFirstCenteredDegree != 5000)
                if(isHalfOut != isHalfOutPrev && isHalfOut) {
                        degHistory.push_front(linePreviousDegree);
                        degHistory.pop_back();
                        lineFirstDegree = degHistory[31];
                }
                errorDeg = degBetween(lineFirstDegree,lineCurrentDegree); //degBetween(lineFirstCenteredDegree,lineCurrentDegree);
                lineDegree = lineCurrentDegree * AVE_CONSTANT_NUM + linePreviousDegree * (1 - AVE_CONSTANT_NUM);
                isHalfOutPrev = isHalfOut;
                if(errorDeg > 80) {    //角対応で80にしている
                        isHalfOut = true;
                        lineDegree += 180;
                        lineDepth = -lineDepth;
                }else{
                        isHalfOut = false;
                }

        }else{
                linePreviousDegree = lineCurrentDegree = 0;
                for(int i = 0; i < DEG_HISTORY_QTY; ++i) {
                        degHistory.push_front(0);
                        degHistory.pop_back();
                }
                lineCurrentX = 0; lineCurrentY = 0;
                //     lineFirstCenteredDegree = 5000;
        }
        if(IS_ONLINE_FIRSTTIME) {//最初に白線に触れた瞬間
                lineFirstDegree = lineCurrentDegree;
                for(int i = 0; i < DEG_HISTORY_QTY; ++i) {
                        degHistory.push_front(lineFirstDegree);
                        degHistory.pop_back();
                }
                // onLineFirstV.x = lineCurrentX; onLineFirstV.y = lineCurrentY;
                isOutsideLine = false;
                lineFirstCenteredDegree = 5000;
        }else if(lastOnLineFlag != isOnLine && isOnLine == 0 && errorDeg >= 90) {
                lineFinishDegree = linePreviousDegree;
                isOnLine = true;
                isOutsideLine = true;
        }

        #ifdef DEBUG_ANGEL
        pc->printf("C deg:%d\tF deg:%d\tinner Product:%d\terrorDeg:%d\tX:%d\tY:%d\r\n",lineCurrentDegree,lineFirstDegree,innerProduct,errorDeg,lineCurrentX,lineCurrentY);
        #endif
}

void LGAngelLine::manage(VectorXY *DriveV,VectorXY *IRv,VectorXY *MouseV,int16_t irDeg,int16_t imuDir,type_camera *camera){

        // X = xcosθ - ysinθ
        // Y = xsinθ + ycosθ

        if(isOnLine) {
                if((lineDepth - 9000) < 0 || isHalfOut || (abs(lineCurrentDegree) < 60 && IS_ONLINE_FIRSTTIME)) {//|| lineWaitOnread_ms() > 1000
                        mouseDeg = atan2(-MouseV->x,MouseV->y) * 180 / PI + imuDir;
                        if(degBetween(mouseDeg,lineDegree) < 80) {
                                *DriveV = makeXYFromRT(80,lineDegree+180);
                        }else{
                                *DriveV = makeXYFromRT(80,(mouseDeg)+180);
                        }
                        lineWait.reset();
                        moveDeligateT.reset();
                        DriveLCount = 0; DriveNCount = 0;
                        if(abs(lineCurrentDegree) < 60 || onWhiteSensorQty > 10) {
                                slowStart.reset();
                        }
                }else{//ペナルティエリア
                        if(camera->probabilityNearOwnGoal < 20) {//自陣
                                if(camera->OwnX < 0 && abs(90 - lineDegree) < 40) {
                                        lineTrace(DriveV,70);
                                }else if(camera->OwnX > 0 && abs(90 + lineDegree) < 40) {
                                        lineTrace(DriveV,-70);
                                }
                        }else{
                                rotateX = -(DriveV->x*cos32_T(lineDegree) + DriveV->y*sin32_T(lineDegree));
                                lineDirVector = -(DriveV->x*sin32_T(lineDegree) - DriveV->y*cos32_T(lineDegree));
                                if(lineDirVector > 0) lineDirVector = 0;
                                DriveV->x = -(rotateX*cos32_T(lineDegree) + lineDirVector*sin32_T(lineDegree));
                                DriveV->y = -(rotateX*sin32_T(lineDegree) - lineDirVector*cos32_T(lineDegree));
                                if(slowStart.read_ms() < 400 && degBetween(irDeg,lineDegree) < 50) { DriveV->x *= 0.3; DriveV->y *= 0.3;}
                                // if(degBetween(irDeg,lineDegree) < 30 && onWhiteSensorQty > 10) DriveV->x = 0; DriveV->y = 0;
                        }
                }
        }else if(IS_INFIELD_FIRSTTIME) {

        }else{
                //     lineWaitOnreset();
        }
        {
                if (lineWait.read_ms() < 1000) {
                        //座標回転
                        rotateX = -(DriveV->x*cos32_T(lineDegree) + DriveV->y*sin32_T(lineDegree));
                        lineDirVector = -(DriveV->x*sin32_T(lineDegree) - DriveV->y*cos32_T(lineDegree));
                        //座標復元
                        if(lineDirVector > 0 && DriveLCount > -20 && abs(DriveNCount) < (abs(lineCurrentDegree) < 60 ? 40:20)) lineDirVector = 0;
                        if(moveDeligateT.read_ms() < 900) {
                                if(lineDirVector < -20) DriveLCount--;         //離れたことをカウントする
                                if(rotateX < -20) DriveNCount--;         //離れたことをカウントする
                                if(rotateX > 20) DriveNCount++;         //離れたことをカウントする
                                lineFirstCenteredDegree = 5000;
                        }
                        DriveV->x = -(rotateX*cos32_T(lineDegree) + lineDirVector*sin32_T(lineDegree));
                        DriveV->y = -(rotateX*sin32_T(lineDegree) - lineDirVector*cos32_T(lineDegree));
                }else{
                        if(lineDirVector > 60 && DriveLCount > -20 && lineDirVector > 70) {
                                DriveV->x *= 0.5; DriveV->y *= 0.5;
                        }
                }
        }
        //   pc->printf("L:%d\t",DriveLCount);
        //   pc->printf("isOnLine:%d lastOn:%d Cdeg:%d\tLErrorDeg:%d\tLDepth:%d\tApaco:%d clusterQty:%d onWhiteSensorQty:%d degBetween:%d\t BX:%d\tBY:%d\tDX:%d\tDY:%d\r\n",isOnLine,lastOnLineFlag,lineCurrentDegree,errorDeg,lineDepth,alpacoX + alpacoY,clusterQty,onWhiteSensorQty,degBetween(irDeg,lineDegree),IRv->x,IRv->y,DriveV->x,DriveV->y);
}

void LGAngelLine::lineTrace(VectorXY *DriveV,int8_t speed){
        //speedを正の数にした時に前進し，負の数の時は後進する．
        if(isOnLine) {
                *DriveV = makeXYFromRT(speed,lineDegree - 90);        //(lineDepth >= 0 ? -90 : 90)

                rotateX = -(DriveV->x*cos32_T(lineDegree) + DriveV->y*sin32_T(lineDegree));
                lineDirVector = -(DriveV->x*sin32_T(lineDegree) - DriveV->y*cos32_T(lineDegree));
                lineDirVector = (lineDepth -8000) / 80;
                //座標復元
                DriveV->x = -(rotateX*cos32_T(lineDegree) + lineDirVector*sin32_T(lineDegree));
                DriveV->y = -(rotateX*sin32_T(lineDegree) - lineDirVector*cos32_T(lineDegree));
                //     pc->printf("isOnLine:%d CurrentDeg:%d\tPrevDeg:%d\tLdeg:%d\tLErrorDeg:%d\tLDepth:%d\tDX:%d\tDY:%d\r\n",isOnLine,lineCurrentDegree,linePreviousDegree,lineDegree,errorDeg,lineDepth,DriveV->x,DriveV->y);
        }
}



