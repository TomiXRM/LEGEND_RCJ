#include "LGmouse.h"
#include "mbed.h"

LGMouse::LGMouse(RawSerial *port){
        mouse = port;
        mouseX = 0;
        mouseY = 0;
        mouse->baud(230400);
        mouse->attach(callback(this,&LGMouse::read),Serial::RxIrq);
        t.attach_us(callback(this,&LGMouse::calc),50000);
}

void LGMouse::read(){
        if(data_length == 0) {//ヘッダ（C）の受信
                head = mouse->getc();
                if(head == 127) {
                        data_length += 1;
                }else{
                        data_length = 0;
                }
        }else if(data_length == 1) {//上バイト受信
                dataYH = mouse->getc();
                data_length += 1;
        }else if(data_length == 2) {//下バイト受信
                dataYL = mouse->getc();
                data_length += 1;
        }else if(data_length == 3) {//上バイト受信
                dataXH = mouse->getc();
                data_length += 1;
        }else if(data_length == 4) { //下バイト受信
                dataXL = mouse->getc();
                data_length = 0;
                mouseXPrev = mouseX;
                mouseYPrev = mouseY;
                mouseX = (int16_t)(dataXH << 8) | dataXL;
                mouseY = (int16_t)(dataYH << 8) | dataYL;
                if(abs(mouseY - mouseYPrev) > 100 && mouseY < -100) mouseY = mouseYPrev;
        }
}

void LGMouse::reset(){
        x = 0; y = 0;
}

void LGMouse::calc(){
        x += -(mouseX*0.1);
        y += -(mouseY*0.1);
}

