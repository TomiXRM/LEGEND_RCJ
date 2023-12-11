#include "LGIMU.h"
#include "mbed.h"


LGIMU::LGIMU(PinName _init,PinName _rx) : serial(NC,_rx),init(_init) {
        serial.baud(230400);
        init = 0;
        data_length=0;
        serial.attach(callback(this,&LGIMU::receive),Serial::RxIrq);
        ti.start();
}

void LGIMU::get(){
        init.write(0); wait_us(5);
        init.write(1);
}
void LGIMU::setGetInterval(uint16_t interval){
        TrigerInterrupt.attach_us(mbed::callback(this,&LGIMU::get),interval);
}
void LGIMU::disableGetInterrupt(){
        TrigerInterrupt.detach();
}

int16_t LGIMU::getDeg(){
        return dir;
}
uint8_t LGIMU::getVelocity(){
        return velocity;
}

void LGIMU::receive(){
        if(data_length == 0) {//ヘッダ（I）の受信
                head =  serial.getc();
                if(head == 'I') {
                        data_length += 1;
                        init = 0;
                }else{
                        data_length = 0;
                }
        }else if(data_length == 1) {//上バイト受信
                lowbyte = serial.getc();
                data_length += 1;
        }else if(data_length == 2) {//下バイト受信
                highbyte  =  serial.getc();
                data_length += 1;
        }else if(data_length == 3) {
                velocity = serial.getc();
                data_length = 0;   //リセット
                _data =  highbyte*256+lowbyte;//復元

                if(_data > 180) {
                        _data = _data - 360;
                }
                _data *= -1;
                if(abs(_data - _datalast) > 30 && abs(_datalastlast - _data) > 30 && ti.read_ms() < 40) {
                        dir = _datalast;
                }else{
                        dir = _data;
                        _datalastlast = _datalast;
                        _datalast = _data;
                        ti.reset();
                }

        }

}
