#include "US_Improved.h"

LGUS::LGUS(PinName trigPin, PinName echoPin) : trig(trigPin), echo(echoPin) {
        t.reset();
        echo.rise(this, &LGUS::echoRise);
        echo.fall(this, &LGUS::echoFall);
        trig = 0;

        Trigger();
}

void LGUS::Trigger() {
        t.reset();
        trig = 1;
        wait_us(15);
        trig = 0;
}
void LGUS::echoFall() {
        t.stop();
        // distance = t.read_us();
        interval.attach(mbed::callback(this, &LGUS::Trigger), 0.07);
        for (uint8_t i = SAMPLE_NUMBER - 1; i > 0; i--)
                f[i] = f[i - 1];
        f[0] = t.read_us();
        uint16_t avr = 0;
        for (uint8_t i = 0; i < SAMPLE_NUMBER; i++)
                avr += f[i];
        avr = avr / SAMPLE_NUMBER;
        distance = avr;
}
void LGUS::echoRise() {
        t.start();
}
