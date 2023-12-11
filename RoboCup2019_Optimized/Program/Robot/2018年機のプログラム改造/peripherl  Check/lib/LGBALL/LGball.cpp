#include "LGball.h"
#include "mbed.h"

LGball::LGball(PinName rx,PinName inter) : device(NC,rx),device_call(inter){
        highbyte = 0;
        lowbyte = 0;
        intdat = 0;
        device_call = 0;
        interval = 10000;
        device.baud(115200);
}

void LGball::frequency(int freq){
        device.baud(freq);
}

void LGball::Setinterval(uint16_t us){
        interval = us;
}

#ifdef Ver1
short LGball::read(){
        device_call = 1;//ピン変化割り込みするためにHIGHに
        timer.reset();
        timer.start();
        isok = 0;
        while (device.readable() != 1) {
                if(timer.read_us() > interval) {
                        isok = 1;//error
                        break;
                }
        }
        if(isok == 1) {
                device_call = 0;
                intdat = -5000;
        }else{
                c = device.getc();
                if(c != 'C') {
                        device_call = 0;
                        return 0;
                }
                timer.reset();
                timer.start();
                isok = 0;
                while (device.readable() != 1) {
                        if(timer.read_us() > interval) {
                                isok = 1;//error
                                break;
                        }
                }
                if(isok == 0) {
                        lowbyte = device.getc();
                }
                timer.reset();
                timer.start();
                isok = 0;
                while (device.readable() != 1) {
                        if(timer.read_us() > interval) {
                                isok = 1;//error
                                break;
                        }
                }
                if(isok == 0) {
                        highbyte = device.getc();
                }
                device_call = 0;//元に戻す
                intdat = highbyte*256+lowbyte;//復元

                intdat =-intdat;

        }
        intdat-=90;
        return intdat;
}
#else
short LGball::read(){
        device_call = 1;//ピン変化割り込みするためにHIGHに
        while (device.readable() != 1) ;
        c = device.getc();
        if(c != 'C') {
                device_call = 0;
                return 0;
        }
        while (device.readable() != 1) ;
        lowbyte = device.getc();
        while (device.readable() != 1) ;
        highbyte = device.getc();
        device_call = 0;        //元に戻す
        intdat = highbyte*256+lowbyte;        //復元

        //intdat =-intdat;
        return intdat;
}
#endif
