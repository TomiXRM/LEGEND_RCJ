#include "MPU6050_DMP6.h"
int main() {
        MPU6050DMP6::setup();
        while(1) {
                MPU6050DMP6::loop();
                //MPU6050DMP6::calibration();
                //while(1) ;
        }
}

/*#include "MPU6050_raw.h"

   int main() {
    MPU6050raw::setup();
    while(1) {
        MPU6050raw::loop();
    }
   }*/
