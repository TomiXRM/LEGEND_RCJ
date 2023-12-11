#include "LGball.h"
#include "mbed.h"

LGball::LGball(RawSerial* device,PinName inter) : device_call(inter){
        dev = device;
        dev->attach(callback(this,&LGball::Receive),Serial::RxIrq);
}

short LGball::get(){
        device_call = 0;
        wait_us(5);
        device_call = 1;
        return data_;
}

void LGball::Receive(){
        received = 0;//デフォに戻しておく。
        if(data_length == 0) {//ヘッダ（H）の受信
                head =  dev->getc();
                if(head == 'H') {
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
                data_length = 0; //リセット
                received = 1;//受信完了

                data_ =  highbyte*256+ lowbyte; //復元

                if(data_ == 6000) {
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
                }
                if(dir_ > 180&&dir_!=5000) {
                        dir_ = dir_ - 360;
                }

                dir_*=dirTranslate;
                //返す用の値を更新
                if(received == 1) {
                        dist = dist_;
                }
                if(received == 1) {
                        dir = dir_;
                }
        }

}

uint8_t LGball::getDist(){
        return dist;
}

short LGball::getDir(){
        return dir;
}

short LGball::adjustment(uint8_t a, uint8_t b, uint8_t c,uint8_t d,uint8_t e){
        if(dir != 5000) {
                dirt = dir;
                switch (dist) {
                case 1:
                        if(dir < 0) {
                                dirt = dir - a;
                        }else if(dir > 0) {
                                dirt = dir + a;
                        }
                        break;
                case 2:
                        if(dir < 0) {
                                dirt = dir - b;
                        }else if(dir > 0) {
                                dirt = dir + b;
                        }
                        break;
                case 3:
                        if(dir < 0) {
                                dirt = dir - c;
                        }else if(dir > 0) {
                                dirt = dir + c;
                        }
                        break;
                case 4:
                        if(dir < 0) {
                                dirt = dir - d;
                        }else if(dir > 0) {
                                dirt = dir + d;
                        }
                        break;
                case 5:
                        if(dir < 0) {
                                dirt = dir - e;
                        }else if(dir > 0) {
                                dirt = dir + e;
                        }
                        break;
                default: break;
                }
        }
        return dirt;

}
