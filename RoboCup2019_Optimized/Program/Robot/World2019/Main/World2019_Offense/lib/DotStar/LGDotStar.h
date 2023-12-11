#ifndef LGDOTSTAR_H
#define LGDOTSTAR_H
#define LED_FREQ 800000 // spi

#include "mbed.h"


class LGDotStar {
public:
LGDotStar(PinName mosi,PinName sck,uint8_t lednumber);
void set(uint8_t r,uint8_t g,uint8_t b,uint8_t n);
void write();
void write(uint8_t groval);
private:
SPI spi;
uint8_t *red;
uint8_t *green;
uint8_t *blue;
uint8_t led_num;
};

#endif
