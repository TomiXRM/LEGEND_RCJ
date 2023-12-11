#include "LGMouse.h"
#include "mbed.h"

LGMouse::LGMouse(PinName mouseRX) : mouse(NC,mouseRX){
    mouseX = 0;
    mouseY = 0;
    mouse.baud(230400);
    mouse.attach(callback(this,&LGMouse::read),Serial::RxIrq);
}

void LGMouse::read(){
        //pc.printf("%d\r\n",mouse.getc());
        if(data_length == 0) {//ヘッダ（C）の受信
                head = mouse.getc();
                if(head == 'H') {
                        data_length += 1;
                        //call = 0;
                }else{
                        data_length = 0;
                }
        }else if(data_length == 1) {//上バイト受信
                mouseX = mouse.getc();
                data_length += 1;
        }else if(data_length == 2) {//下バイト受信
                mouseY = mouse.getc();
                data_length = 0;
        }
        //pc.printf("h:%d\tX:%d\tY:%d\r\n",head,mouseX,mouseY);
}

