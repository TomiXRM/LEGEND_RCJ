#include "mbed.h"
#include "LGRGB.h"

LGRGB::LGRGB(PinName R,PinName G,PinName B) : Red(R),Green(G),Blue(B){
        state = 1; LEDON = 0;
}

void LGRGB::On(char color){
        if(state == 1) {
                switch (color) {
                case RED:     Red = LEDON; Green = !LEDON; Blue = !LEDON; break;
                case GREEN:   Red = !LEDON; Green = LEDON; Blue = !LEDON; break;
                case BLUE:    Red = !LEDON; Green = !LEDON; Blue = LEDON; break;
                case WHITE:   Red = LEDON; Green = LEDON; Blue = LEDON; break;
                case YELLOW:  Red = LEDON; Green = LEDON; Blue = !LEDON; break;
                case MAGENTA: Red = LEDON; Green = !LEDON; Blue = LEDON; break;
                case CYAAN:   Red = !LEDON; Green = LEDON; Blue = LEDON; break;
                default:      Red = LEDON; Green = LEDON; Blue = LEDON; break;
                }
        }else{

                Red = !LEDON; Green = !LEDON; Blue = !LEDON;
        }
}

void LGRGB::On(bool red,bool green,bool blue){
        if(state == 1) {
                if(LEDON) {
                        Red = red; Green = green; Blue = blue;
                }else{
                        Red = !red; Green = !green; Blue = !blue;
                }
        }else{
                Red = !LEDON; Green = !LEDON; Blue = !LEDON;
        }
}
void LGRGB::Off(){
        Red = !LEDON; Green = !LEDON; Blue = !LEDON;
}
void LGRGB::Disable(){
        state = 0;
}
