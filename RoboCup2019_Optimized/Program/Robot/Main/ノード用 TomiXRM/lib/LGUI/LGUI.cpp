#include "mbed.h"
#include "LGUI.h"

LGUI::LGUI(PinName tx,PinName rx,bool Workmode) : serial(tx,rx){
        serial.baud(9600);
        WorkMode = _Workmode;
}

void LGUI::baud(int baudlate){
        serial.baud(baudlate);
}

void LGUI::SetModeVariable(char *m){
        M = m;
}

void LGUI::SetDataVariable(char *d){
        D = d;
}

void LGUI::receive(){
        if(WorkMode == MAIN) {

        }else if(WorkMode == SLAVE) {

        }
}
