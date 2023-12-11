/**
 * Rotational encoder Library
 *
 * 2 Phase-differnt signals (Signal A,B)
 *
 * <example part>
 *   parts no   : EC12PLGRBSDVBF
 *   ec site    : http://akizukidenshi.com/catalog/g/gP-05773/
 *   data sheet : http://akizukidenshi.com/download/ds/topup/EC12PLRGBSDVBF-D-25K-24-24C-61%EF%BC%8F08-6H(SPEC).pdf
 *
 * <scematic>
 *
 *    rotational     +Vcc  +Vcc
 *       encoder     |     |
 *      +--------+   R(*1) R(*1)
 *      |        |   |     |
 *      |     A  +---+-----+----- mbed (pXX)
 *      |        |         |
 *      |     B  +---------+----- mbed (pXX)
 *      |   C    |
 *      +---+----+    *1: mbed inernal pullup (The outside resistance is unnecessary)
 *          |
 *          - GND
 *
 * <timing chart>
 *
 *     interrupt          down(*1)   up(*2)     down
 *                        !          !          !
 *     stop point    stop !          !     stop !
 *                     !  !          !       !  !
 *              ----------+          +----------+
 *                     !  |          |       !  |
 *  Signal A           !  +----------+       !  +----------+
 *                     !  !          !       !  !
 *                   +----------+    !     +----------+
 *                   |    !     |    !     |    !     |
 *  Signal B  -------+    !     +----------+    !     +---------
 *                        !          !          !
 *  detect                !          !          !
 *          cw →          01    →    10   →     01
 *
 *         ccw ←          11    ←    00   ←     11
 *
 *   *1:interuupt down side is Provisionally process
 *   *2:interrupt up side is dicition process
 *
 * <example program>
 * @code
 * #include "mbed.h"
 * #include "RotationalEncoder.h"
 *
 * RotationalEncoder encoder(p21, p23);
 *
 * BusOut led(LED1, LED2, LED3, LED4);
 *
 * int main() {
 *   while(1) {
 *
 *   led = encoder.getAbsolutePulses();
 *   printf("abs = %4i   rel = %4i\r\n",encoder.getAbsolutePulses(), encoder.getRelativePulses());
 *
 *   }
 * }
 * @endcode
 */


#ifndef ROTATIONALENCODER_H
#define ROTATIONALENCODER_H

#include "mbed.h"

/**
 * rotational encoder interface.
 */


/**
 *  @class RotationalEncoder
 *  @brief nothine
 */
class RotationalEncoder
{

public:

    /** Create a RotationalEncoder port, connected to the specified A and B pins
     *  @param channelA encoder A pin
     *  @param channelB encoder B pin
     *  @note  nothing
     */
    RotationalEncoder(PinName channelA, PinName channelB);

    /**
     * Reset the number for absolute and relative.
     */
    void reset(void);

    /**
     * Get the number of absolute pulses recorded by the encoder.
     *
     * @return Number of absolute pulses.
     */
    int16_t getAbsolutePulses(void);

    /**
     * Get the number of relative pulses recorded by the encoder.
     *
     * @return Number of relative pulses.
     */
    int16_t getRelativePulses(void);

    /**
    *chenge the Number of the RotationalEncoder
    */
    void chenge(int16_t _chenge);

private:

    void encoderAUp(void);
    void encoderADwon(void);

    InterruptIn _channelA;
    InterruptIn _channelB;

    int16_t _channelBLevelOfchannelALow;
    #define INITAL 0xff

    volatile int16_t _absolutePulse;
    volatile int16_t _relativePulse;
};

#endif /* ROTATIONALENCODER_H */
