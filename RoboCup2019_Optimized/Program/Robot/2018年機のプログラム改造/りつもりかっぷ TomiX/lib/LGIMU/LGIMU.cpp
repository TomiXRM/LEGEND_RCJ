#include "LGIMU.h"
#include "mbed.h"

LGIMU::LGIMU(PinName rx,PinName inter,float us)
        : device(NC,rx),device_call(inter)
{
        highbyte = 0;
        lowbyte = 0;
        intdat = 0;
        device_call = 0;
        interval = 0;
        device.baud(230400);
        device.attach(this,&LGIMU::IMU_callback,RawSerial::RxIrq);
        ticker.attach(this,&LGIMU::blink,us);
}



void LGIMU::frequency(int freq){
        device.baud(freq);
}
void LGIMU::Setinterval(uint16_t us){
        interval = us;
}

void LGIMU::enable(float us){
        ticker.attach(this,&LGIMU::blink,us);
}

void LGIMU::disable(){
        ticker.detach();
}

short LGIMU::read(){
        return intdat;
}

void LGIMU::blink(){
        device_call = !device_call;
}

void LGIMU::callback(){
        c = device.getc();
        if(c != 'H') {
                device_call = 0;
                return 0;
        }
        device_call = 0;
        timer.reset();
        timer.start();
        isok = 0;
        device_call = 0;
        while (device.readable() != 1) {
                if(timer.read_us() > interval) {
                        isok = 1;//error
                        break;
                }
        }
        if(isok == 0) {
                lowbyte = device.getc();
        }
        device_call = 0;//元に戻す
        intdat = lowbyte;//highbyte*256+lowbyte;//復元

        if(intdat > 61) {
                intdat = -256 + intdat;
        }
}
