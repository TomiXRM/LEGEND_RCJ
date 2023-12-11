/**
 * Rotational encoder example program
 *
 * <scamatic>
 *         EC12PLRGBSDVBF-D-25K-24-24C-61
 *           http://akizukidenshi.com/catalog/g/gP-05773/
 *           |
 *        rotatinal
 *           encoder          mbed:LPC1768
 *       +------+
 *       |      |
 *       |    A +------------ mbed p21
 *       |      |
 *       |      |
 *       |    B +------------ mbed p23
 *       |      |
 *       |  C   |
 *       +--+---+
 *          |
 *          _ GND
 *                            mbed:LED1 to LED4 : absorlute Pulses under 4bit display
 *                                :USB-Serial   : absorlute pulses and relative pulses output
 *                                                 9600bps data=8bit parity=none stopbit=1bit
 */

#include "mbed.h"
#include "RotationalEncoder.h"

RotationalEncoder encoder(p21, p23);

BusOut led(LED1, LED2, LED3, LED4);


int main() {
    while(1) {
        
    led = encoder.getAbsolutePulses();
    printf("abs = %4i   rel = %4i\r\n",encoder.getAbsolutePulses(), encoder.getRelativePulses());

    }
}
