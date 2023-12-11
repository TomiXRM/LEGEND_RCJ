#include "mbed.h"
#include "LGDotStar.h"
#include "TrigonometricTable.h"


LGDotStar LED(PB_15,PB_13,16);

int theta;

int main(){
        while (1) {
                theta++;
                for (size_t i = 0; i < 16; i++) {
                        LED.set(255*sin32_T(theta),255*sin32_T(theta+120),255*sin32_T(theta-120),i);

                }
                LED.write(31);
                wait_ms(7);
        }
}
