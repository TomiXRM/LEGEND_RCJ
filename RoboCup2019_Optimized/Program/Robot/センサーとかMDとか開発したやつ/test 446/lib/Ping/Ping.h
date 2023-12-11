/* mbed Ping Library
 * Copyright (c) 2007-2010 rosienej
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
    
 #ifndef MBED_PING_H
 #define MBED_PING_H

#include "mbed.h"
/** Ping  class, based on an InterruptIn pin, and a timer
 *  works with the parallax Ping))) sensor (www.parallax.com)
 *
 * Example:
 * @code
 * // Continuously send pings and read the sensor
 * #include "mbed.h"
 * #include "Ping.h"
 * 
 * Ping Pinger(p21);
 * 
 * int main() {
 *     int range;
 
 *     while(1) {
 *  
 *        Pinger.Send();    
 *        wait_ms(30);
 *        range = Pinger.Read_cm();
 *     }
 * }
 * @endcode
 */
class Ping {
  public:
   /** Create a Ping object connected to the specified InterruptIn pin
    *
    * @param PING_PIN InterruptIn pin to connect to 
    */
    Ping(PinName Trig_PIN,PinName Echo_PIN);
    
     /** Sends a Ping 
      *
      * @param none
      */
    void Send(void);
    
     /** Set the speed of sound, default 33 cm/ms 
      *
      * @param Speed of sound in centimeters per milliseconds
      */
    void Set_Speed_of_Sound(int SoS_ms);
    
    /** Read the result in centimeters
      *
      * @param none
      */
    int Read_cm(void);
    
  protected:
    
    InterruptIn     _event;
    DigitalOut      _trig;
    DigitalIn       _echo;
    Timer           _timer;

    bool _Valid;
    bool _Busy;
    int  _Time;
    int  _SPEED_OF_SOUND_CM; /* in milliseconds */
    
    void _Starts(void);
    void _Stops(void);
    
  };
  
  #endif
  