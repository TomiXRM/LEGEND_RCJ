#define LGRGB_H
#ifdef  LGRGB_H
/*このライブラリはGND共有のLED用のライブラリです*/
#include "mbed.h"
#define WHITE   1
#define RED     2
#define GREEN   3
#define BLUE    4
#define YELLOW  5
#define MAGENTA 6
#define CYAAN   7



class LGRGB {
public:
LGRGB(PinName R,PinName G,PinName B);
void On(char color);
void On(bool red,bool green,bool blue);
void Off();
void Disable();
private:
DigitalOut Red;
DigitalOut Green;
DigitalOut Blue;
bool state;
bool LEDON;
};

#endif
