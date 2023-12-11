#include <mbed.h>
#include "RGB.h"

RGB::RGB(PinName r,PinName g, PinName b) : redPin(r),greenPin(g),bluePin(b){
        turnOff();
}

void RGB::turnOff(){
        redPin=1;
        greenPin=1;
        bluePin=1;
}
void RGB::turnOn(color targetColor){
        redPin=targetColor.r;
        greenPin=targetColor.g;
        bluePin=targetColor.b;
}

void RGB::on(char color){
        switch (color) {
        case RED_L:     redPin = LEDON; greenPin = !LEDON; bluePin = !LEDON; break;
        case GREEN_L:   redPin = !LEDON; greenPin = LEDON; bluePin = !LEDON; break;
        case BLUE_L:    redPin = !LEDON; greenPin = !LEDON; bluePin = LEDON; break;
        case WHITE_L:   redPin = LEDON; greenPin = LEDON; bluePin = LEDON; break;
        case YELLOW_L:  redPin = LEDON; greenPin = LEDON; bluePin = !LEDON; break;
        case MAGENTA_L: redPin = LEDON; greenPin = !LEDON; bluePin = LEDON; break;
        case CYAAN_L:   redPin = !LEDON; greenPin = LEDON; bluePin = LEDON; break;
        default:      redPin = LEDON; greenPin = LEDON; bluePin = LEDON; break;
        }
}