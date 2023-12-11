#include "OpenMV.h"

OpenMV::OpenMV(PinName _RX,PinName _call,int16_t* _dir) : device(NC,_RX),call(_call){
        dir = _dir;//ポインタ受け渡し(方位センサ)
        data_length = 0;
        head = 'A';
        call = 0;
        device.baud(230400);
        device.attach(callback(this,&OpenMV::receiveXYW),Serial::RxIrq);
}

void OpenMV::baud(int b){
        device.baud(b);
}

void OpenMV::get(){
        call = 0;
        wait_us(3);
        call = 1;
}

//テストよう
void OpenMV::receive(){
        if(data_length == 0) {//ヘッダ（C）の受信
                head = device.getc();
                if(head == 'C') {
                        data_length += 1;
                        call = 0;
                }else{
                        data_length = 0;
                }
        }else if(data_length == 1) {//上バイト受信
                highbyte = device.getc();
                data_length += 1;
        }else if(data_length == 2) {//下バイト受信
                lowbyte = device.getc();
                data = (lowbyte<<8)|highbyte;//ビット演算
                data_length = 0;
        }
}
//XY
void OpenMV::receiveXY(){
        if(data_length == 0) {//ヘッダ（A）の受信
                head = device.getc();
                if(head == 'A') {
                        data_length += 1;
                        call = 0;
                }else{
                        data_length = 0;
                }
        }else if(data_length == 1) {//自ゴールX受信
                _OwnX = device.getc();
                data_length += 1;
        }else if(data_length == 2) {//自ゴールY受信
                _OwnY = device.getc();
                data_length += 1;
        }else if(data_length == 3) {//相手ゴールX受信
                _OpsX = device.getc();
                data_length += 1;
        }else if(data_length == 4) {//相手ゴールY受信
                _OpsY = device.getc();
                data_length = 0;

                //計算
                OwnX = _OwnX;
                OwnY = _OwnY;
                OpsX = _OpsX;
                OpsY = _OpsY;

        }
}
//XYW
void OpenMV::receiveXYW(){
        if(data_length == 0) {//ヘッダ（A）の受信
                head = device.getc();
                if(head == 'A') {
                        data_length += 1;
                        call = 0;
                }else{
                        data_length = 0;
                }
        }else if(data_length == 1) {//自ゴールX受信
                _OwnX = device.getc();
                data_length += 1;
        }else if(data_length == 2) {//自ゴールY受信
                _OwnY = device.getc();
                data_length += 1;
        }else if(data_length == 3) {//自ゴールW受信
                _OwnW = device.getc();
                data_length += 1;
        }else if(data_length == 4) {//相手ゴールX受信
                _OpsX = device.getc();
                data_length += 1;
        }else if(data_length == 5) {//相手ゴールY受信
                _OpsY = device.getc();
                data_length += 1;
        }else if(data_length == 6) {//相手ゴールW受信
                _OpsW = device.getc();
                data_length = 0;

                //計算

                OwnGoalRightEnd = _OwnX + (_OwnW/2);
                OwnGoalLeftEnd  = _OwnX - (_OwnW/2);
                OpsGoalRightEnd = _OpsX + (_OpsW/2);
                OpsGoalLeftEnd  = _OpsX - (_OpsW/2);
                OwnX = _OwnX;
                OwnY = _OwnY;
                OwnW = _OwnW;
                OpsX = _OpsX;
                OpsY = _OpsY;
                OpsW = _OpsW;
        }
}
//本当に見えているか見えていないかだけでチェックする方法
void OpenMV::recognizeCheck(){
        if(_OwnX == -1&&_OwnY == -1) {
                OwnRecognize = 0;
        }else{
                OwnRecognize = 1;
        }
        if(_OpsX == -1&&_OpsY == -1) {
                OpsRecognize = 0;
        }else{
                OpsRecognize = 1;
        }
}
//ワイドを見て閾値以下の場合は見えていないことにするフィルタリングをする方法
void OpenMV::recognizeCheck(uint8_t Own_Wide_Threshold,uint8_t Ops_Wide_Threshold){
        if(_OwnW < Own_Wide_Threshold) {
                OwnRecognize = 0;
        }else{
                OwnRecognize = 1;
        }
        if(_OpsW < Ops_Wide_Threshold) {
                OpsRecognize = 0;
        }else{
                OpsRecognize = 1;
        }
}

short OpenMV::calDegree(bool q){
        //q→相手ゴール:1 自ゴール:0
        int8_t Vx,Vy;
        if(q == 1) {
                Vx = OpsX;
                Vy = OpsY;
        }else{
                Vx = OwnX;
                Vy = OwnY;
        }
        if(Vx == -1&&Vy == -1) {
                return 5000;
        }else{
                return (short)(atan2(Vx,Vy)/PI*180.0);//ベクトルから角度の算出
        }

}
