
#pragma once
#include "Adafruit_SSD1306.h"

#ifndef PI
    #define PI 3.14159265359
#endif

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define CenterX(_size,_charnum,_x)(_x-(((6*(_charnum-1)*_size)+(5*_size))/2))
#define CenterY(_size,_y)(_y-(7/2*_size))

class LGLCD:public Stream{
public:
    LGLCD(PinName _SDA,PinName _SCL);

    I2C i2c;
    Adafruit_SSD1306_I2c base;
    void splash_lg();
    void display();
    void clearDisplay();
    void setTextSize(uint8_t size);
    void setTextCursor(uint8_t x,uint8_t y);
    void menuText(string str);
    void bargraph(int r);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void drawLine(int16_t x0, int16_t y0,  int16_t x1, int16_t y1, uint16_t color);
    void drawMap();
    void Ball(short ballx,short bally);
    void Ballrt(uint8_t r,int8_t t);

    virtual int _putc(int value) { return base.writeChar(value); };
    virtual int _getc() { return -1; };

};
