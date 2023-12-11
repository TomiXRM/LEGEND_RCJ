#include "LGCAM.h"
#include "mbed.h"

LGCAM::LGCAM() : device(NC,IMU_RX),device_call(IMU_INT){
        highbyte = 0;
        lowbyte = 0;
        intdat = 0;
        device_call = 0;
        interval = 1000;
        device.baud(230400);
}

void LGCAM::frequency(int freq){
        device.baud(freq);
}

void LGCAM::Setinterval(uint16_t us){
        interval = us;
}

char LGCAM::check(){
        device_call = 1;
        while (device.readable() != 1) ;
        c = device.getc();
        wait_ms(10);
        device_call = 0;
        wait_ms(10);
        return c;
}
short LGCAM::receive(){
        device_call = 1;//ピン変化割り込みするためにHIGHに
        timer.start();
        timer.reset();
        isok = 0;
        //while (device.readable() != 1) ;
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
                lowbyte = device.getc();
                /*c = device.getc();
                   if(c != 'H') {
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

                   }*/
                /*timer.reset();
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
                   }*/
                device_call = 0;//元に戻す
                intdat = lowbyte;//highbyte*256+lowbyte;//復元
        }
        /*intdat *=-1;
           intdat *=-1;
           if(c=='x') {
                x = intdat;
                return 'X';
           }else if(c == 'y') {
                y = intdat;
                return 'Y';
           }else if(c == 'H') {
                //IMU
                dir = (signed short)intdat;
                return 'H';
           }else{
                return 'Z';
           }*/
        if(intdat == -5000) {
                intdat = last_dir;
        }
        last_dir = intdat;
        if(intdat > 61) {
                intdat = -256 + intdat;
        }
        dir = (-intdat);
        return -intdat;

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
