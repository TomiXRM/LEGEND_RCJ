#include <mbed.h>
#include "config.h"


INLINE void setup(){
      pc.baud(230400);
      IMU.init();
      pc.printf("Boot!\r\n");
      pc.printf("\r\nHello world!! LEGEND\r\n");
      pc.printf("This STM32 works with %ld MHz clock\r\n", (SystemCoreClock / 1000000));
      LCD.splash_lg();
      for (size_t i = 0; i < 8; i++) {rgb.on(i); wait_ms(30);}  //チカチカさせる
      rgb.on(WHITE);
      btn.soundIndex(0); btn.soundIndex(1); btn.soundIndex(2);
      wait_ms(200); btn.soundIndex(3);
      getDx.attach_us(&getDxFromIMU,5000);
      //timer start
      pidDt.start();
      moveBackTimer.start();
      pidDir.target = 0;
      for (size_t i = 0; i < 30; i++) {
            IMU.setZero();
      }
//         drib.state(0);
}

int main() {
      // put your setup code here, to run once:
      setup();
      while(1) {
            modeRun();
      }
}
