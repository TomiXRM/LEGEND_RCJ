#include "LGAngelLine.h"
#include "mbed.h"

AngelLine::AngelLine(PinName BUS1,PinName BUS2,PinName BUS3,PinName BUS4,PinName BUS5,PinName DATA) : channel(BUS1,BUS2,BUS3,BUS4,BUS5),analog(DATA){
    uint16_t maxData[32]={   //checkThrので使います
    43458,9306,9178,9050,7593,8329,8810,8586,6921,7225,7241,6841,6569,8329,7945,8073,7305,7529,8650,8169,7945,10266,10042,10922,8714,10010,9754,904,6921,9290,8410,9306
    };
    sensorFlag = false;
    sensorCount = 0;
}

void AngelLine::checkThr(){
    for (uint8_t i = 0; i < 32; i++){
        channel = i;
        data[i] = analog.read_u16();
        if(data[i] > maxData[i]){maxData[i] = data[i];}
        //pc.printf("%d,", maxData[i]+200);
    }
}

void AngelLine::readLine(){
    for (uint8_t i = 0; i < 32; i++){
        channel = i;
        data[i] = analog.read_u16();
        if(data[i] < dataThr[i]){
            onGreenFlag[i] = true;
        }else{
            onGreenFlag[i] = false;
        }
        //pc.printf("%d ", onGreenFlag[i]);
    }
}

void AngelLine::lineManage(){
    sensorNum = 0;
    sensorCount = 0;

    while(onGreenFlag[sensorNum] == true){
        if(sensorNum == 0){
            sensorNum = 31;
        }else{
            sensorNum -= 1;
        }
        if (sensorNum == 0){
            //pc.printf("no");
            goto degreeCalEnd;  //白線を踏んでいない状態
        }
    }
    for (uint8_t i = 0; i < 32; i++){
        if(onGreenFlag[sensorNum] == true && sensorFlag == false){
            sensorBeginTem = sensorNum;
            sensorFlag = true;
        }
        if(onGreenFlag[sensorNum] == false && sensorFlag == true){
            if(sensorNum == 0){
                sensorFinishTem = 31;
            }else{
                sensorFinishTem = sensorNum -1;
            }
            sensorFlag = false;
            if(sensorCountTem > sensorCount){   //最大個数を更新した場合、[始・カウント・終]の数字を更新する
                sensorBegin = sensorBeginTem;
                sensorFinish = sensorFinishTem;
                sensorCount = sensorCountTem;
            }
        }

        if(sensorFlag == true){
            sensorCountTem ++;
        }else{
            sensorCountTem = 0;
        }

        sensorNum++;
        if(sensorNum ==32){
            sensorNum = 0;
        }        
    }
    //pc.printf("\tC:%d\tS:%d\tF:%d",sensorCount,sensorBegin,sensorFinish);
degreeCalEnd:;
}