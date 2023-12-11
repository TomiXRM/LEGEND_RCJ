#include "LGball.h"
#include "mbed.h"

LGball::LGball() : device(NC,PD_2),device_call(PA_12){
        highbyte = 0;
        lowbyte = 0;
        intdat = 0;
        device_call = 0;
        interval = 10000;
        device.baud(115200);
        //A = B = C = 0;
}

void LGball::frequency(int freq){
        device.baud(freq);
}

short LGball::adjustment(short A,short B,short C){
        short _IR = IRangle;
        switch (IRlong) {
        case 3:
                //遠距離
                if (_IR > 0) {
                        _IR += C;
                } else if (_IR < 0) {
                        _IR -= C;
                } else {
                        _IR *= 1.1;
                }
                break;
        case 4:
                //中距離
                if (_IR > 0) {
                        _IR += B;
                } else if (_IR < 0) {
                        _IR -= B;
                } else {
                        _IR *= 1.1;
                }
                break;
        case 5:
                //近距離
                if (_IR > 0) {
                        _IR += A;
                } else if (_IR < 0) {
                        _IR -= A;
                }
                break;
        }
        return _IR;
}




void LGball::Setinterval(uint16_t us){
        interval = us;
}

short LGball::read(uint16_t error_angle){
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
                //intdat =-intdat;
        }
//処理２
        if(intdat >= 5000) {
                IRangle = intdat - 5000;
                IRlong = 5;

        }else if(intdat >= 4000) {
                IRangle = intdat - 4000;
                IRlong = 4;

        }else if(intdat >= 3000) {
                IRangle = intdat - 3000;
                IRlong = 3;
        }else if(intdat == 2000 || intdat == 0) {
                IRangle = 5000;
                IRlong = 2;
        }

        if(IRangle % 1000 == 0 && IRangle != 5000) {
                IRangle = 0;
        }

        if(IRangle != 5000) {
                IRangle %=360;
        }

        if(IRangle > 180 && IRangle != 5000) {
                IRangle = IRangle - 360;
                //sensors.IRangle -= 23;
        }

        /*if(IRangle != 5000) {
                //移動平均
                for(int i = 0; i < (average_num); i++) {
                        l[i] = l[i+1];
                }
                l[(average_num-1)] = IRangle;
                for (int i = 0; i < (average_num); i++) {
                        IRangle +=l[i];
                }
                IRangle = IRangle/average_num;
                //移動平均
           } //*/
        if(IRangle > (-error_angle) &&  IRangle < error_angle) {
                IRangle = 0;
        }


        /*if(IRangle > 60000) {
                IRangle -= 65535;
           }*/
        //intdat-=90;
        IRreceive = intdat;
        return intdat;
}

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

                //intdat =-intdat;

        }
//処理２
        if(intdat > 5000) {
                IRangle = intdat - 5000;
                IRlong = 5;

        }else if(intdat > 4000) {
                IRangle = intdat - 4000;
                IRlong = 4;

        }else if(intdat > 3000) {
                IRangle = intdat - 3000;
                IRlong = 3;

        }else if(intdat == 2000 || intdat == 0) {
                IRangle = 5000;
                IRlong = 2;
        }

        if(IRangle % 1000 == 0&&IRangle != 5000) {
                IRangle = 0;
        }

        if(IRangle != 5000) {
                IRangle %=360;
        }

        //IRangle %=360;



        if(IRangle > 180 && IRangle != 5000) {
                IRangle = IRangle - 360;
                //sensors.IRangle -= 23;
        }
        /*if(IRangle != 5000) {
                //移動平均
                for(int i = 0; i < (average_num); i++) {
                        l[i] = l[i+1];
                }
                l[(average_num-1)] = IRangle;
                for (int i = 0; i < (average_num); i++) {
                        IRangle +=l[i];
                }
                IRangle = IRangle/average_num;
                //移動平均
           } //*/

        return intdat;
}

uint16_t LGball::readIRlong(){
        return IRlong;
}
uint16_t LGball::readIRangle(){
        return IRangle;
}
