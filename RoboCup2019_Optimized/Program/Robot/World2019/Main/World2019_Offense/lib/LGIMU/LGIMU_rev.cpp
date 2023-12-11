#include "mbed.h"
#include "LGIMU_rev.h"

LGIMU::LGIMU(RawSerial* _serial){
        serial = _serial;
        //   serial->printf("Hello World！！！！！");
        serial->attach(callback(this,&LGIMU::receive),Serial::RxIrq);
}

int16_t LGIMU::getDeg(){
        return dir;
}
uint8_t LGIMU::getVelocity(){
        return velocity;
}

void LGIMU::receive(){
        int16_t _data;
        if(data_length == 0) {//ヘッダ（I）の受信
                head =  serial->getc();
                if(head == 'I') {
                        data_length += 1;
                }else{
                        data_length = 0;
                }
        }else if(data_length == 1) {//上バイト受信
                lowbyte = serial->getc();
                data_length += 1;
        }else if(data_length == 2) {//下バイト受信
                highbyte  =  serial->getc();
                data_length += 1;
        }else if(data_length == 3) {
                velocity = serial->getc();
                data_length = 0;   //リセット
                _data =  highbyte*256+lowbyte;//復元

                if(_data > 180) {
                        _data = _data - 360;
                }
                _data *= -1;
                dir = _data;
        }

}