#ifndef _RGB_H_
#define _RGB_H_
#include <mbed.h>
#include "colors.h"


#define WHITE_L   1
#define RED_L     2
#define GREEN_L   3
#define BLUE_L    4
#define YELLOW_L  5
#define MAGENTA_L 6
#define CYAAN_L   7

#define LEDON_L 0

class RGB {

public:
RGB(PinName r,PinName g, PinName b);
void turnOff();
void turnOn(color targetColor);
void on(char color);
private:

DigitalOut redPin;
DigitalOut greenPin;
DigitalOut bluePin;

};

#endif
