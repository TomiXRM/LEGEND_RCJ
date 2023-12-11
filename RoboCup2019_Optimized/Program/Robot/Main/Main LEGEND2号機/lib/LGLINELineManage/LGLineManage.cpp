#include "LGLineManage.h"
#include "mbed.h"


LineManage::LineManage(PinName LF,PinName LR,PinName LB,PinName LL) : lineF(LF),lineR(LR),lineB(LB),lineL(LL){
        line_wait.start();
        lineOP.attach(mbed::callback(this,&LineManage::LineCheck),1.0 / 12000);

        OutFlag = 0;
        OnWH1st = 0;
        OnWH2nd = 0;
        OnWH3rd = 0;
        OnWH4th = 0;
        Frised = 0;
        Rrised = 0;
        Brised = 0;
        Lrised = 0;
        Raw = 0;   //0~8(0は踏んでない)
        Raw_16 = 0;
        LastRaw = 0;
        order = 0;
        lineF.mode(PullDown);
        lineR.mode(PullDown);
        lineL.mode(PullDown);
        lineB.mode(PullDown);
}

void LineManage::EnableIRQ(uint16_t tm){
        lineOP.attach(mbed::callback(this,&LineManage::LineCheck),tm);
}

void LineManage::DisableIRQ(){
        lineOP.detach();
}

void LineManage::LineCheck(){
        OutFlag = 0;

        front= lineF.read();
        right = lineR.read();
        back= lineB.read();
        left = lineL.read();


        if(front== 1 || right == 1 || back== 1 || left == 1) {
                if( OnWH1st == 0) {//初回
                        if(front== 1) {
                                OnWH1st = 1;
                        }
                        if(right == 1) {
                                OnWH1st = 2;
                        }
                        if(back== 1) {
                                OnWH1st = 3;
                        }
                        if(left == 1) {
                                OnWH1st = 4;
                        }
                        OnWH2nd = 0;
                        OnWH3rd = 0;

                        order = ( OnWH1st * 1000);
                }else if( OnWH2nd == 0) {
                        switch ( OnWH1st) {
                        case 1:
                                if(right == 1) {
                                        OnWH2nd = 2;
                                }
                                if(back== 1) {
                                        OnWH2nd = 3;
                                }
                                if(left == 1) {
                                        OnWH2nd = 4;
                                }
                                break;
                        case 2:
                                if(front== 1) {
                                        OnWH2nd = 1;
                                }
                                if(back== 1) {
                                        OnWH2nd = 3;
                                }
                                if(left == 1) {
                                        OnWH2nd = 4;
                                }
                                break;
                        case 3:
                                if(front== 1) {
                                        OnWH2nd = 1;
                                }
                                if(right == 1) {
                                        OnWH2nd = 2;
                                }
                                if(left == 1) {
                                        OnWH2nd = 4;
                                }
                                break;
                        case 4:
                                if(front== 1) {
                                        OnWH2nd = 1;
                                }
                                if(right == 1) {
                                        OnWH2nd = 2;
                                }
                                if(back== 1) {
                                        OnWH2nd = 3;
                                }
                                break;
                        }
                        OnWH3rd == 0;
                        order =  order + ( OnWH2nd * 100);
                        /*TiLINE.reset();
                           //TiLINE.start();//2個目と3個目の 時間差 を計測開始*/
                }else if( OnWH3rd == 0) {//踏んでない

                        /*T = TiLINE.read_us(); //差を取得
                           TiLINE.reset();*/
                        if( OnWH1st == 1 &&  OnWH2nd == 2) {

                                if(back== 1) {
                                        OnWH3rd = 3;
                                }
                                if(left == 1) {
                                        OnWH3rd = 4;
                                }
                        }else if( OnWH1st == 1 &&  OnWH2nd == 3) {
                                //エラー
                                if(right == 1) {
                                        OnWH3rd= 2;
                                }

                                if(left == 1) {
                                        OnWH3rd = 4;
                                }
                        }else if( OnWH1st == 1 &&  OnWH2nd == 4) {

                                if(right == 1) {
                                        OnWH3rd = 2;
                                }
                                if(back== 1) {
                                        OnWH3rd = 3;
                                }

                        }else if( OnWH1st == 2 &&  OnWH2nd == 1) {

                                if(back== 1) {
                                        OnWH3rd = 3;
                                }
                                if(left == 1) {
                                        OnWH3rd = 4;
                                }
                        }else if( OnWH1st == 2 &&  OnWH2nd == 3) {

                                if(front== 1) {
                                        OnWH3rd= 1;
                                }
                                if(left == 1) {
                                        OnWH3rd = 4;
                                }
                        }else if( OnWH1st == 2 &&  OnWH2nd == 4) {
                                //エラー
                                if(front== 1) {
                                        OnWH3rd= 1;
                                }
                                if(back== 1) {
                                        OnWH3rd = 3;
                                }

                        }else if( OnWH1st == 3 &&  OnWH2nd == 1) {
                                //エラー
                                if(right == 1) {
                                        OnWH3rd= 2;
                                }

                                if(left == 1) {
                                        OnWH3rd = 4;
                                }
                        }else if( OnWH1st == 3 &&  OnWH2nd == 2) {
                                if(front== 1) {
                                        OnWH3rd= 1;
                                }
                                if(left == 1) {
                                        OnWH3rd = 4;
                                }
                        }else if( OnWH1st == 3 &&  OnWH2nd == 4) {
                                if(front== 1) {
                                        OnWH3rd= 1;
                                }
                                if(right == 1) {
                                        OnWH3rd= 2;
                                }

                        }else if( OnWH1st == 4 &&  OnWH2nd == 1) {

                                if(right == 1) {
                                        OnWH3rd= 2;
                                }
                                if(back== 1) {
                                        OnWH3rd = 3;
                                }

                        }else if( OnWH1st == 4 &&  OnWH2nd == 2) {
                                //エラー
                                if(front== 1) {
                                        OnWH3rd= 1;
                                }
                                if(back== 1) {
                                        OnWH3rd = 3;
                                }

                        }else if( OnWH1st == 4 &&  OnWH2nd == 3) {
                                if(front== 1) {
                                        OnWH3rd= 1;
                                }
                                if(right == 1) {
                                        OnWH3rd= 2;
                                }

                        }
                        order =  order + ( OnWH3rd * 10);
                }else if( OnWH1st != 0 &&  OnWH2nd != 0 &&  OnWH3rd != 0) {
                        //3つ全部踏んだ
                }else{
                        //踏んでない
                }
                OutFlag = 1;
                line_wait.reset();
        }else{
                if( OnWH2nd != 0) {
                        if(OnWH3rd != 0) {
                                wt = 500;
                        }else{
                                wt = 200;
                        }
                }else{
                        wt = 1;
                }

                if(line_wait.read_ms() >  wt) {
                        OutFlag = 0;
                        order = 0;
                        OnWH1st = 0;
                        OnWH2nd = 0;
                        OnWH3rd = 0;
                        OnWH4th = 0;
                        T = 0;
                }
        }

        //LINE RAW
        Raw_16 = front+ (right*2) + (back* 4) + (left * 8);
        //踏んでいても踏んでいなくても処理する
        switch (Raw_16) {

        case 1:     Raw =  0;   break;     //F
        case 2:     Raw = -90;  break;     //R
        case 3:     Raw = -45;  break;     //FR
        case 4:     Raw =  180; break;     //B
        case 5:     Raw =  360; break;     //error
        case 6:     Raw = -135; break;     //RB
        case 7:     Raw =  0;   break;     //FRB
        case 8:     Raw =  90;  break;      //L
        case 9:     Raw =  45;  break;     //FL
        case 10:    Raw =  360; break;     //error
        case 11:    Raw =  0;   break;      //FRL
        case 12:    Raw =  135; break;     //BL
        case 13:    Raw =  90;  break;      //FBL
        case 14:    Raw =  180; break;     //RBL
        case 15:    Raw =  360; break;     //error
        }

}

void LineManage::SetLine(VectorXY *vector,VectorXY *IRv,VectorXY *Bound){
        if(OutFlag == 1) {
                //sensors.IMUS = 1;
                if( order == 1000) {
                        vector->y = -90;
                }else if( order == 1200) {
                        switch ( Raw) {
                        case 0:
                                vector->x = 0; vector->y = -100; break;
                        case -90:
                                vector->x = -90; vector->y = -60; break;
                        case -45:
                                vector->x = -90; vector->y = -60; break;
                        default:
                                vector->x = -90; vector->y = -90; break;
                        }
                }else if( order == 1230) {
                        switch ( Raw) {
                        case 0:
                                vector->x = 0; vector->y = -100; break;
                        case 45:
                                vector->x = -90; vector->y = -90; break;
                        case -45:
                                vector->x = -90; vector->y = -90; break;
                        case 90:
                                vector->x = -90; vector->y = -60; break;
                        case -90:
                                vector->x = -90; vector->y = -60; break;
                        case 135:
                                vector->x = -90; vector->y = -90; break;
                        case -135:
                                vector->x = -30; vector->y = -90; break;
                        case 180:
                                vector->x = 0; vector->y = -100; break;
                        case 360:
                                vector->x = -90; vector->y = 0; break;
                        }
                }else if( order == 1240) {
                        switch ( Raw) {
                        case 0:
                                vector->x = 0; vector->y = -100; break;
                        case 45:
                                vector->x = 90; vector->y = -90; break;
                        case -45:
                                vector->x = -90; vector->y = -90; break;
                        case 90:
                                vector->x = 90; vector->y = -60; break;
                        case -90:
                                vector->x = -90; vector->y = -60; break;
                        case 135:
                                vector->x = 30; vector->y = -90; break;
                        case -135:
                                vector->x = -30; vector->y = -90; break;
                        case 180:
                                vector->x = 0; vector->y = -120; break;
                        case 360:
                                vector->x = 100; vector->y = 1800; break;
                        }
                }else if( order == 1300) {
                        //error
                        vector->y = 0;
                        vector->x *= 1.2;
                }else if( order == 1320) {
                        if(vector->x > 0) { vector->x = -90; vector->y = 0;}
                }else if( order == 1340) {
                        if(vector->x > 0) { vector->x = -90; vector->y = 0;}
                }else if( order == 1400) {
                        switch ( Raw) {
                        case 0:
                                vector->x = 0; vector->y = -100; break;
                        case 90:
                                vector->x = 90; vector->y = -60; break;
                        case 45:
                                vector->x = 90; vector->y = -60; break;
                        default:
                                vector->x = 90; vector->y = -90; break;
                        }
                }else if( order == 1420) {
                        switch ( Raw) {
                        case 0:
                                vector->x = 0; vector->y = -100; break;
                        case 45:
                                vector->x = 90; vector->y = -90; break;
                        case -45:
                                vector->x = -90; vector->y = -90; break;
                        case 90:
                                vector->x = 90; vector->y = -60; break;
                        case -90:
                                vector->x = -90; vector->y = -60; break;
                        case 135:
                                vector->x = 30; vector->y = -90; break;
                        case -135:
                                vector->x = -30; vector->y = -90; break;
                        case 180:
                                vector->x = 0; vector->y = -120; break;
                        case 360:
                                vector->x = 100; vector->y = -180; break;
                        }
                }else if( order == 1430) {
                        switch ( Raw) {
                        case 0:
                                vector->x = 0; vector->y = -100; break;
                        case 45:
                                vector->x = 90; vector->y = -90; break;
                        case -45:
                                vector->x = 90; vector->y = -90; break;
                        case 90:
                                vector->x = 90; vector->y = 0; break;
                        case -90:
                                vector->x = 90; vector->y = 0; break;
                        case 135:
                                vector->x = 90; vector->y = -90; break;
                        case -135:
                                vector->x = 30; vector->y = -90; break;
                        case 180:
                                vector->x = 0; vector->y = -100; break;
                        case 360:
                                vector->x = 90; vector->y = 0; break;
                        }
                }else if( order == 2000) {
                        if(vector->x >= 0) {
                                vector->x = -90;
                        }
                }else if( order == 2100) {
                        switch ( Raw) {
                        case 0:
                                vector->x = 0; vector->y = -100; break;
                        case -90:
                                vector->x = -90; vector->y = -60; break;
                        case -45:
                                vector->x = -90; vector->y = -60; break;
                        default:
                                vector->x = -90; vector->y = -90; break;
                        }
                }else if( order == 2130) {
                        vector->x = -90; vector->y = 0;
                }else if( order == 2140) {
                        vector->x = -90; vector->y = -90;
                }else if( order == 2300) {
                        switch ( Raw) {
                        case 180:
                                vector->x = 0; vector->y = 90; break;
                        case -90:
                                vector->x = -90; vector->y = 0; break;
                        case -135:
                                vector->x = -60; vector->y = 90; break;
                        default:
                                vector->x = -90; vector->y = 90; break;
                        }
                }else if( order == 2310) {
                        //vector->x = -90; vector->y = 0;
                        switch ( Raw) {
                        case 0:
                                vector->x = -30; vector->y = -100; break;
                        case 45:
                                vector->x = -30; vector->y = -100; break;
                        case -45:
                                vector->x = -90; vector->y = -90; break;
                        case 90:
                                vector->x = -90; vector->y = 0; break;
                        case -90:
                                vector->x = -90; vector->y = 0; break;
                        case 135:
                                vector->x = -90; vector->y = 90; break;
                        case -135:
                                vector->x = -90; vector->y = 90; break;
                        case 180:
                                vector->x = -30; vector->y = 100; break;
                        case 360:
                                vector->x = -90; vector->y = 0; break;
                        }
                }else if( order == 2340) {
                        switch ( Raw) {
                        case 0:
                                vector->x = -100; vector->y = 100; break;
                        case 45:
                                vector->x = -30; vector->y = 100; break;
                        case -45:
                                vector->x = 0; vector->y = -90; break;
                        case 90:
                                vector->x = -90; vector->y = 0; break;
                        case -90:
                                vector->x = -90; vector->y = 0; break;
                        case 135:
                                vector->x = 0; vector->y = 90; break;
                        case -135:
                                vector->x = -60; vector->y = 90; break;
                        case 180:
                                vector->x = 0; vector->y = 100; break;
                        case 360:
                                vector->x = -90; vector->y = 100; break;
                        }
                }else if( order == 2400) {
                        //error
                        vector->x = 0;
                }else if( order == 2410) {
                        vector->x = 0; vector->y = -50;
                }else if( order == 2430) {
                        vector->x = 0; vector->y = 50;
                }else if( order == 3000) {
                        if(vector->y <= 0) {vector->y = 90;}
                }else if( order == 3100) {
                        vector->y = 0;
                }else if( order == 3120) {
                        if(vector->x > 0) { vector->x = -90; vector->y = 0;}
                }else if( order == 3140) {
                        if(vector->x < 0) { vector->x = 90; vector->y = 0;}
                }else if( order == 3200) {
                        switch ( Raw) {
                        case 180:
                                vector->x = 0; vector->y = 90; break;
                        case -90:
                                vector->x = -90; vector->y = 0; break;
                        case -135:
                                vector->x = -60; vector->y = 90; break;
                        default:
                                vector->x = -90; vector->y = 90; break;
                        }
                }else if( order == 3210) {
                        if(vector->x > 0) { vector->x = -90; vector->y = 10;}
                }else if( order == 3240) {
                        switch ( Raw) {
                        case 0:
                                vector->x = 0; vector->y = 100; break;
                        case 45:
                                vector->x = 0; vector->y = 100; break;
                        case -45:
                                vector->x = 0; vector->y = 100; break;
                        case 90:
                                vector->x = -90; vector->y = 0; break;
                        case -90:
                                vector->x = 90; vector->y = 0; break;
                        case 135:
                                vector->x = 60; vector->y = 90; break;
                        case -135:
                                vector->x = -60; vector->y = 90; break;
                        case 180:
                                vector->x = 0; vector->y = 100; break;
                        case 360:
                                vector->x = 0; vector->y = 100; break;
                        }
                }else if( order == 3400) {
                        switch ( Raw) {
                        case 180:
                                vector->x = 0; vector->y = 90; break;
                        case 90:
                                vector->x = 90; vector->y = 0; break;
                        case 135:
                                vector->x = 60; vector->y = 90; break;
                        default:
                                vector->x = 90; vector->y = 90; break;
                        }
                }else if( order == 3410) {
                        if(vector->x < 0) { vector->x = 90; vector->y = 10;}
                }else if( order == 3420) {
                        switch ( Raw) {
                        case 0:
                                vector->x = 0; vector->y = 100; break;
                        case 45:
                                vector->x = 0; vector->y = 100; break;
                        case -45:
                                vector->x = 0; vector->y = 100; break;
                        case 90:
                                vector->x = -90; vector->y = 0; break;
                        case -90:
                                vector->x = 90; vector->y = 0; break;
                        case 135:
                                vector->x = 60; vector->y = 90; break;
                        case -135:
                                vector->x = -60; vector->y = 90; break;
                        case 180:
                                vector->x = 0; vector->y = 100; break;
                        case 360:
                                vector->x = 0; vector->y = 100; break;
                        }
                }else if( order == 4000) {
                        if(vector->x <= 0) { vector->x = 90; vector->y = 0;}
                }else if( order == 4100) {
                        switch ( Raw) {
                        case 0:
                                vector->x = 0; vector->y = -100; break;
                        case 90:
                                vector->x = 90; vector->y = -60; break;
                        case 45:
                                vector->x = 90; vector->y = -60; break;
                        default:
                                vector->x = 90; vector->y = -90; break;
                        }
                }else if( order == 4120) {
                        vector->x = 90; vector->y = -90;
                }else if( order == 4130) {
                        vector->x = 90; vector->y = 0;
                }else if( order == 4200) {
                        vector->x = 0;
                }else if( order == 4210) {
                        if(vector->x > 0) { vector->x = -90; vector->y = 0;}
                }else if( order == 4230) {
                        vector->x = 0; vector->y = 50;
                }else if( order == 4300) {
                        switch ( Raw) {
                        case 180:
                                vector->x = 0; vector->y = 90; break;
                        case 90:
                                vector->x = 90; vector->y = 0; break;
                        case 135:
                                vector->x = 60; vector->y = 90; break;
                        default:
                                vector->x = 90; vector->y = 90; break;
                        }
                }else if( order == 4310) {
                        vector->x = 90; vector->y = 0;
                }else if( order == 4320) {
                        switch ( Raw) {
                        case 0:
                                vector->x = 100; vector->y = 100; break;
                        case 45:
                                vector->x = 30; vector->y = 100; break;
                        case -45:
                                vector->x = 0; vector->y = -90; break;
                        case 90:
                                vector->x = 90; vector->y = 0; break;
                        case -90:
                                vector->x = 90; vector->y = 0; break;
                        case 135:
                                vector->x = 0; vector->y = 90; break;
                        case -135:
                                vector->x = 60; vector->y = 90; break;
                        case 180:
                                vector->x = 0; vector->y = 100; break;
                        case 360:
                                vector->x = 90; vector->y = 100; break;
                        }
                }
                line_wait.reset();
                LastRaw =  Raw;
        }else{
                if(line_wait.read_ms() < 1000) {
                        //sensors.IMUS = 1;
                        /*
                           if( LastRaw == 180 && sensors.IRangle < 0) {
                                if(abs((-sensors.IRangle) -  LastRaw) <= 45) {
                                        vector = SetZero();
                                }
                           }else{
                                if(abs(sensors.IRangle -  LastRaw) <= 45) {
                                        vector = SetZero();
                                }
                           }*/
                        if(IRv->x > 0 && Bound->x > 0 && vector->x > 0) {
                                vector->x = 0;
                        }
                        if(IRv->x < 0 && Bound->x < 0 && vector->x < 0) {
                                vector->x = 0;
                        }
                        if(IRv->y > 0 && Bound->y > 0 && vector->y > 0) {
                                vector->y = 0;
                        }
                        if(IRv->y < 0 && Bound->y < 0 && vector->y < 0) {
                                vector->y = 0;
                        }
                }
                //IMU_Chenge = 1;
        }

}
