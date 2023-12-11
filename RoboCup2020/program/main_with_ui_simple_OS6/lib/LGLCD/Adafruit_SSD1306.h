/*********************************************************************
   This is a library for our Monochrome OLEDs based on SSD1306 drivers

   Pick one up today in the adafruit shop!
   ------> http://www.adafruit.com/category/63_98

   These displays use SPI to communicate, 4 or 5 pins are required to
   interface

   Adafruit invests time and resources providing this open source code,
   please support Adafruit and open-source hardware by purchasing
   products from Adafruit!

   Written by Limor Fried/Ladyada  for Adafruit Industries.
   BSD license, check license.txt for more information
   All text above, and the splash screen must be included in any redistribution
*********************************************************************/

/*
 *  Modified by Neal Horman 7/14/2012 for use in mbed
 */

#ifndef _ADAFRUIT_SSD1306_H_
#define _ADAFRUIT_SSD1306_H_

#include "mbed.h"
#include "Adafruit_GFX.h"
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define CenterX(_size,_charnum,_x)(_x-(((6*(_charnum-1)*_size)+(5*_size))/2))
#define CenterY(_size,_y)(_y-(7/2*_size))

#ifndef PI
    #define PI 3.14159265359
#endif

extern RawSerial pc;

#include <vector>
#include <algorithm>

// A DigitalOut sub-class that provides a constructed default state
class DigitalOut2 : public DigitalOut
{
public:
DigitalOut2(PinName pin, bool active = false) : DigitalOut(pin) {
        write(active);
};
DigitalOut2& operator= (int value) {
        write(value); return *this;
};
DigitalOut2& operator= (DigitalOut2& rhs) {
        write(rhs.read()); return *this;
};
operator int() {
        return read();
};
};

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

/** The pure base class for the SSD1306 display driver.
 *
 * You should derive from this for a new transport interface type,
 * such as the SPI and I2C drivers.
 */
class Adafruit_SSD1306 : public Adafruit_GFX
{
public:
Adafruit_SSD1306(PinName RST, uint8_t rawHeight = 32, uint8_t rawWidth = 128)
        : Adafruit_GFX(rawWidth,rawHeight)
        , rst(RST,false)
{
        buffer.resize(rawHeight * rawWidth / 8);
};

void begin(uint8_t switchvcc = SSD1306_SWITCHCAPVCC);

// These must be implemented in the derived transport driver
virtual void command(uint8_t c) = 0;
virtual void data(uint8_t c) = 0;
virtual void drawPixel(int16_t x, int16_t y, uint16_t color);

/// Clear the display buffer
void clearDisplay(void);
virtual void invertDisplay(bool i);

/// Cause the display to be updated with the buffer content.
void display();
/// Fill the buffer with the AdaFruit splash screen.
virtual void splash();

protected:
virtual void sendDisplayBuffer() = 0;
DigitalOut2 rst;

// the memory buffer for the LCD
std::vector<uint8_t> buffer;
};


/** This is the SPI SSD1306 display driver transport class
 *
 */
class Adafruit_SSD1306_Spi : public Adafruit_SSD1306
{
public:
/** Create a SSD1306 SPI transport display driver instance with the specified DC, RST, and CS pins, as well as the display dimentions
 *
 * Required parameters
 * @param spi - a reference to an initialized SPI object
 * @param DC (Data/Command) pin name
 * @param RST (Reset) pin name
 * @param CS (Chip Select) pin name
 *
 * Optional parameters
 * @param rawHeight - the vertical number of pixels for the display, defaults to 32
 * @param rawWidth - the horizonal number of pixels for the display, defaults to 128
 */
Adafruit_SSD1306_Spi(SPI &spi, PinName DC, PinName RST, PinName CS, uint8_t rawHieght = 32, uint8_t rawWidth = 128)
        : Adafruit_SSD1306(RST, rawHieght, rawWidth)
        , cs(CS,true)
        , dc(DC,false)
        , mspi(spi)
{
        begin();
        splash();
        display();
};

virtual void command(uint8_t c)
{
        cs = 1;
        dc = 0;
        cs = 0;
        mspi.write(c);
        cs = 1;
};

virtual void data(uint8_t c)
{
        cs = 1;
        dc = 1;
        cs = 0;
        mspi.write(c);
        cs = 1;
};

protected:
virtual void sendDisplayBuffer()
{
        cs = 1;
        dc = 1;
        cs = 0;

        for(uint16_t i=0, q=buffer.size(); i<q; i++)
                mspi.write(buffer[i]);

        if(height() == 32)
        {
                for(uint16_t i=0, q=buffer.size(); i<q; i++)
                        mspi.write(0);
        }

        cs = 1;
};

DigitalOut2 cs, dc;
SPI &mspi;
};

/** This is the I2C SSD1306 display driver transport class
 *
 */
class Adafruit_SSD1306_I2c : public Adafruit_SSD1306
{
public:
  #define SSD_I2C_ADDRESS     0x78
/** Create a SSD1306 I2C transport display driver instance with the specified RST pin name, the I2C address, as well as the display dimensions
 *
 * Required parameters
 * @param i2c - A reference to an initialized I2C object
 * @param RST - The Reset pin name
 *
 * Optional parameters
 * @param i2cAddress - The i2c address of the display
 * @param rawHeight - The vertical number of pixels for the display, defaults to 32
 * @param rawWidth - The horizonal number of pixels for the display, defaults to 128
 */
Adafruit_SSD1306_I2c(I2C &i2c, PinName RST, uint8_t i2cAddress = SSD_I2C_ADDRESS, uint8_t rawHeight = 64, uint8_t rawWidth = 128)
        : Adafruit_SSD1306(RST, rawHeight, rawWidth)
        , mi2c(i2c)
        , mi2cAddress(i2cAddress)
{
        begin();
        splash();
        display();
        mi2c.frequency(400000);
};



void Ball(short ballx,short bally){
    fillCircle(ballx+32,bally+32,4,WHITE);
}


void Ballrt(uint8_t r,int8_t t){
//X-Y inversion applied. Front:0 deg.
//r should be between 0 and 32
short    bally = (short)(r * -cos(t / 180.0 * PI));
short    ballx = (short)(r * sin(t / 180.0 * PI));
Ball(ballx,bally);
}


void bargraph(int r){
        //draw the bar graph
        fillRect(r, 50, 128 - r, 10, BLACK);
        fillRect(3, 50, r, 10, WHITE);

        for (uint8_t i = 1; i < 13; i++) {
                fillRect(i * 10, 50, 2, 10, BLACK);
        }
}

void drawMap(){
    drawCircle(32, 32, 31, WHITE);
    drawCircle(32, 32, 24, WHITE);
    drawCircle(32, 32, 16, WHITE);
    drawLine(0, 32, 64, 32, WHITE);
    drawLine(32, 0, 32, 64, WHITE);

}


void menuText(string str){
    clearDisplay();
    setTextSize(2);
    setTextCursor(CenterX(2,str.size(),64),10);
    printf("%s",str.c_str()); 
}

void splash_lg(){
    for (uint8_t i = 0; i < 65; i += 15) {
      clearDisplay();
      fillCircle(64,32,i, WHITE);
      display();
    }

  clearDisplay(); display();
  setTextSize(3); setTextCursor(CenterX(3,6,64),10);
  printf("LEGEND"); display();

}

virtual void command(uint8_t c)
{
        char buff[2];
        buff[0] = 0; // Command Mode
        buff[1] = c;
        mi2c.write(mi2cAddress, buff, sizeof(buff));
}

virtual void data(uint8_t c)
{
        char buff[2];
        buff[0] = 0x40; // Data Mode
        buff[1] = c;
        mi2c.write(mi2cAddress, buff, sizeof(buff));
};

protected:
virtual void sendDisplayBuffer()
{
        char buff[17];
        buff[0] = 0x40; // Data Mode

        for(uint16_t i=0, q=buffer.size(); i<q; i+=16 )
        { uint8_t x;

          // TODO - this will segfault if buffer.size() % 16 != 0
          for(x=1; x<sizeof(buff); x++)
                  buff[x] = buffer[i+x-1];
          mi2c.write(mi2cAddress, buff, sizeof(buff));
        }

        
        mi2c.stop();
};

I2C &mi2c;
uint8_t mi2cAddress;
};

#endif
