#include "LGDotStar.h"

LGDotStar::LGDotStar(PinName mosi,PinName sck,uint8_t n) : spi(mosi,NC,sck){
        spi.frequency(LED_FREQ);
        red = new uint8_t[n];
        green = new uint8_t[n];
        blue = new uint8_t[n];
        led_num = n;
        for (size_t i = 0; i < led_num; i++) {
                red[i] = 0; green[i] = 0; blue[i] = 0;
        }
        write();
}
void LGDotStar::set(uint8_t r,uint8_t g,uint8_t b,uint8_t n){
        red[n] = r; green[n] = g; blue[n] = b;
}
void LGDotStar::write(){
        uint8_t i;
        // start frame
        for (i = 0; i < 4; i++) {
                spi.write(0);
        }
        for (i = 0; i < led_num; i++) {
                spi.write(255);
                spi.write(blue[i]);  // B
                spi.write(green[i]);  // G
                spi.write(red[i]);   // R
        }
        // end frame
        for(i = 0; i<4; i++) {
                spi.write(1);
        }

}
void LGDotStar::write(uint8_t groval_brightness ){
        // groval_brightness(0~31)
        uint8_t i;
        // start frame
        for (i = 0; i < 4; i++) {
                spi.write(0);
        }
        for (i = 0; i < led_num; i++) {
                spi.write((7<<5)|groval_brightness);
                spi.write(blue[i]); // B
                spi.write(green[i]); // G
                spi.write(red[i]); // R
        }
        // end frame
        for(i = 0; i<4; i++) {
                spi.write(1);
        }
}
