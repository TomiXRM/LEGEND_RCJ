#include "LGUS.h"
#include "mbed.h"

LGUS::LGUS(PinName tx,PinName rx,PinName inter) : device(tx,rx),device_call(inter){
        highbyte = 0;
        lowbyte = 0;
        intdat = 0;
        device_call = 0;
        interval = 10000;
        device.baud(115200);
}
void LGUS::frequency(int freq){
        device.baud(freq);
}
void LGUS::Setinterval(uint16_t us){
        interval = us;
}
short LGUS::read(){
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
char LGUS::read1byte(){
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
                return device.getc();
        }else{
                return 'Z';
        }

}
void LGUS::receive(){
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
                if(c != 'S') {
                        device_call = 0;

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
                        us1 = (device.getc());
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
                        us2 = (device.getc());
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
                        us3 = (device.getc());
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
                        us4 = (device.getc());
                }
                device_call = 0;
        }
}

unsigned char LGUS::read_F(){
        return us1;
}

unsigned char LGUS::read_R(){
        return us2;
}

unsigned char LGUS::read_B(){
        return us3;
}

unsigned char LGUS::read_L(){
        return us4;
}
