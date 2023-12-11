#include "mbed.h"
#include "adns_9800.h"
#include "buffer.h"
#include "medianFilter.h"

#define DEBUG_BAUD      230400
#define WAIT_TIME       20      /// Unit:[us]

#define BUF_X           0
#define BUF_Y           1

#define HEADER_H        0xFA
#define HEADER_L        0xAF

#define MOUSE_ADDRESS   0x02

Buffer data;
Serial port(PA_9, PA_10);
DigitalOut REDE(PA_1);

Serial pc(USBTX, USBRX,DEBUG_BAUD);
DigitalOut sLED(D13);
Ticker sendDataTicker;
adns_9800 mouse_sensor(A6,A5,A4,A3);

int16_t delta_x, delta_y;

Median_create(int16_t, 16, medianX);
Median_create(int16_t, 16, medianY);

int16_t medianDataX,medianDataY;

void sendData(void) {
        int16_t X,Y;
        sLED = 1;
        port.putc(127); wait_us(3);
        X = (int16_t)data.get(BUF_X);
        Y = (int16_t)data.get(BUF_Y);
        port.putc((X >> 8)); wait_us(3);
        port.putc((X & 0x00FF)); wait_us(3);
        port.putc((Y >> 8)); wait_us(3);
        port.putc((Y & 0x00FF)); wait_us(3);
        sLED = 0;
        data.reset(BUF_X);
        data.reset(BUF_Y);
}

// ===========================================================================//
int main() {
        port.baud(DEBUG_BAUD);
        sendDataTicker.attach_us(sendData,2500);
        medianX.init(); medianY.init();
        while(true) {
                if(mouse_sensor.new_motion_data_available()) {
                        mouse_sensor.get_delta_x_y(delta_x, delta_y);
                        data.setWithIntegral(BUF_X, delta_x);
                        data.setWithIntegral(BUF_Y, delta_y);
                        medianDataX = medianX.calc((int16_t)data.get(BUF_X));
                        medianDataY = medianY.calc((int16_t)data.get(BUF_Y));
                }

                pc.printf("X:%d\tY:%d\r\n",medianDataX,medianDataY);
        }
        return 0;
}
// ===========================================================================//