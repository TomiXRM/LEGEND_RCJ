#include "LGLCD.h"

LGLCD::LGLCD(PinName SDA,PinName SCL) : i2c(SDA,SCL),base(i2c,SDA,0x78,64,128){
        i2c.frequency(400000);//400kHz
        base.clearDisplay();
        base.fillScreen(BLACK);
}


void LGLCD::display(){
        base.display();
}

void LGLCD::clearDisplay(){
        base.clearDisplay();
}

void LGLCD::setTextSize(uint8_t size){
        base.setTextSize(size);
}

void LGLCD::setTextCursor(uint8_t x,uint8_t y){
        base.setTextCursor(x,y);
}


void LGLCD::menuText(string str){
        clearDisplay();
        setTextSize(2);
        setTextCursor(CenterX(2,str.size(),64),10);
        base.printf(str.c_str());
        display();
}

void LGLCD::splash_lg(){
        for (uint8_t i = 0; i < 65; i += 15) {
                base.clearDisplay();
                base.fillCircle(64,32,i, WHITE);
                base.display();
        }

        base.clearDisplay(); display();
        base.setTextSize(3); base.setTextCursor(CenterX(3,6,64),10);
        base.printf("LEGEND"); display();

}

void LGLCD::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
        base.fillRect(x,y,w,h,color);
}

void LGLCD::bargraph(int r){
        //draw the bar graph
        clearDisplay();
        fillRect(r, 50, 128 - r, 10, BLACK);
        fillRect(3, 50, r, 10, WHITE);

        for (uint8_t i = 1; i < 13; i++) {
                fillRect(i * 10, 50, 2, 10, BLACK);
        }
        display();
}


void LGLCD::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color){
        base.drawCircle(x0,y0,r,color);
}

void LGLCD::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color){
        base.fillCircle(x0,y0,r,color);
}

void LGLCD::drawLine(int16_t x0, int16_t y0,  int16_t x1, int16_t y1, uint16_t color){
        base.drawLine(x0,y0,x1,y1,color);
}


void LGLCD::drawMap(){
        drawCircle(32, 32, 31, WHITE);
        drawCircle(32, 32, 24, WHITE);
        drawCircle(32, 32, 16, WHITE);
        drawLine(0, 32, 64, 32, WHITE);
        drawLine(32, 0, 32, 64, WHITE);
        display();
}

void LGLCD::Ballrt(uint8_t r,int8_t t){
//X-Y inversion applied. Front:0 deg.
//r should be between 0 and 32
        drawMap();
        clearDisplay();
        short bally = (short)(r * -cos(t / 180.0 * PI));
        short ballx = (short)(r * sin(t / 180.0 * PI));
        Ball(ballx,bally); display();
}

void LGLCD::Ball(short ballx,short bally){
        fillCircle(ballx+32,bally+32,4,WHITE);
}
