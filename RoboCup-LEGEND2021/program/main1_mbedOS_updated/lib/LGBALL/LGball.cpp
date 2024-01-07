#include "LGball.h"
#include "mbed.h"

LGball::LGball(RawSerial* device,PinName inter) : device_call(inter){
        dev = device;
        dev->attach(callback(this,&LGball::Receive),Serial::RxIrq);
}

short LGball::get(){
        received = 0;//読み込み開始と同時に0に戻す
        device_call = 0;
        wait_us(5);
        device_call = 1;
        return data_;
}

void LGball::Receive(){
        if(data_length == 0) {//ヘッダ（H）の受信
                head = dev->getc();
                if(head == 'J') {
                        data_length += 1;
                        device_call = 0;
                }else{
                        data_length = 0;
                }
        }else if(data_length == 1) {//上バイト受信
                lowbyte = dev->getc();
                data_length += 1;
        }else if(data_length == 2) {//下バイト受信
                highbyte  =  dev->getc();
                data_length += 1;
        }else if(data_length == 3) {
                dist_2 = dev->getc();
                if(dist_2 != 74) dist = dist_2;
                data_length = 0;         //リセット

                data_ = highbyte*256+lowbyte; //復元

                /*if(data_ == 6000) {
                        dist_ = 0;
                        dir_ = 5000;
                   }else if(data_ >= 5000) {
                        dist_ = 5;
                        dir_ = data_ -5000;
                   }else if(data_ >= 4000) {
                        dist_ = 4;
                        dir_ = data_ -4000;
                   }else if(data_ >= 3000) {
                        dist_ = 3;
                        dir_ = data_ -3000;
                   }else if(data_ >= 2000) {
                        dist_ = 2;
                        dir_ = data_ -2000;
                   }else if(data_ >= 1000) {
                        dist_ = 1;
                        dir_ = data_ -1000;
                   }*/
                if(data_ < 1000) {
                        //ignore
                }else{
                        if(data_ == 6000) {
                                dist_ = 0;
                                dir_ = 5000;
                        }else{
                                distBuf = (data_ /1000);
                                dirBuf = data_ - distBuf*1000;
                                dir_ = dirBuf;
                                dist_ = distBuf;//3000とかのやつ
                        }
                        if(dir_ !=5000) {
                                if((dir_ < 360 && dir_ >= 0)) {
                                        if(dir_ > 180) {
                                                dir_ = dir_ - 360;
                                        }
                                        received = 1; //受信完了
                                        dir_*=dirTranslate;
                                        dir = dir_;
                                        //dist = dist_;
                                        data = data_;
                                        //返す用の値を更新
                                }
                        }else{
                                dir = -5000;
                        }
                }
        }
}

uint8_t LGball::getDist(){
        return dist;
}

short LGball::getDir(){
        return dir;
}

short LGball::adjustment(short Balldir,uint8_t a, uint8_t b, uint8_t c,uint8_t d,uint8_t e){
        if(Balldir != 5000) {
                dirt = Balldir;
                switch (dist) {
                case 1:
                        if(Balldir < 0) {
                                dirt = Balldir - a;
                        }else if(Balldir > 0) {
                                dirt = Balldir + a;
                        }
                        break;
                case 2:
                        if(Balldir < 0) {
                                dirt = Balldir - b;
                        }else if(Balldir > 0) {
                                dirt = Balldir + b;
                        }
                        break;
                case 3:
                        if(Balldir < 0) {
                                dirt = Balldir - c;
                        }else if(Balldir > 0) {
                                dirt = Balldir + c;
                        }
                        break;
                case 4:
                        if(Balldir < 0) {
                                dirt = Balldir - d;
                        }else if(Balldir > 0) {
                                dirt = Balldir + d;
                        }
                        break;
                case 5:
                        if(Balldir < 0) {
                                dirt = Balldir - e;
                        }else if(Balldir > 0) {
                                dirt = Balldir + e;
                        }
                        break;
                default: break;
                }
        }
        return dirt;

}
