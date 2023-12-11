#include "LGCAM.h"
#include "mbed.h"

LGCAM::LGCAM(PinName rx,PinName inter) : device(NC,rx),device_call(inter){
        highbyte = 0;
        lowbyte = 0;
        intdat = 0;
        device_call = 0;
        interval = 1000;
        device.baud(115200);
}

void LGCAM::frequency(int freq){
        device.baud(freq);
}

void LGCAM::Setinterval(uint16_t us){
        interval = us;
}

char LGCAM::receive(){
        device_call = 1;//ピン変化割り込みするためにHIGHに
        //wait_ms(3);
        timer.start();
        timer.reset();
        isok = 1;
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
                /*if(c != 'X' && c != 'y'&&c != 'H') {
                        device_call = 0;
                        return 0;
                   }*/
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
        }
        if(c=='x') {
                x = intdat;
                return 'X';
        }else if(c == 'y') {
                y = intdat;
                return 'Y';
        }else if(c == 'H') {
                //IMU
                dir = intdat;
                return 'H';
        }

}
short LGCAM::read(int mode){
        switch (mode) {
        case 'X':
                return x;
                break;
        case 'Y':
                return y;
                break;
        case 'H':
                return dir;
                break;
        default:
                return -1;
                break;
        }
}
