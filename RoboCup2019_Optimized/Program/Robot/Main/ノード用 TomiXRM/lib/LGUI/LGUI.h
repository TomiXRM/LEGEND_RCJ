#ifndef LGUI_H
#define LGHU_H
#include "mbed.h"

class LGUI {
public:
LGUI(PinName tx,PinName rx,bool Workmode);

void baud(int baudlate);
void SetModeVariable(char *m);
void SetDataVariable(char *d);
void receive();
private:
char  *M;
short *D;
};



#endif
