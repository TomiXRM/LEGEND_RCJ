#include "LGESP.h"
#include "stdarg.h"
#include "stdio.h"
/* macros */
#define MAX_LEN 256

#define SOH 0x01 // Start Headder
#define STX 0x02 // STX. Start Text
#define ETX 0x03 // ETX. Start Text

#define SENDTO_WiFi_SENSE 0x00 // Head.WiFi,SensorChanel
#define SENDTO_WiFi_DEBUG 0x05 // Head.WiFi,TerminalChanel

LGESP::LGESP(PinName tx, PinName rx, type_sensors *_sensors,
             type_camera *_camera, RawSerial *_pc)
        : dev(tx, rx) {
        sensors = _sensors;
        camera = _camera;
        dev.baud(230400);
        pc = _pc;
}

void LGESP::debug(const char *format, ...) {

        va_list args;
        char _buf[100];

        va_start(args, format);
        vsprintf(_buf, format, args);
        va_end(args);

        uint16_t len = (uint16_t)strlen(_buf);
        uint16_t i = 0;

        dev.putc(SOH);

        dev.putc(SENDTO_WiFi_DEBUG);

        dev.putc(STX);

        for (i = 0; i < len; i++) {
                dev.putc(_buf[i]);
        }

        dev.putc(ETX);
}

void LGESP::collectData() {

        dev.putc(SOH);

        dev.putc(SENDTO_WiFi_SENSE);

        dev.putc(STX);

        dev.printf("%d\t%d\t%d\t%d\t", sensors->LINE1, sensors->LINE2, sensors->LINE3,
                   sensors->LINE4);

        dev.printf("%d\t", camera->OwnGoalRightEnd);
        dev.printf("%d\t", camera->OwnGoalLeftEnd);
        dev.printf("%d\t", camera->OwnX);
        dev.printf("%d\t", camera->OwnY);
        dev.printf("%d\t", camera->OpsGoalRightEnd);
        dev.printf("%d\t", camera->OpsGoalLeftEnd);
        dev.printf("%d\t", camera->OpsX);
        dev.printf("%d\t", camera->OpsY);
        dev.printf("%d\t", sensors->irDeg);
        dev.printf("%d\t", sensors->irDist);
        dev.printf("%f\t", sensors->dir);
        dev.printf("%d\t", sensors->PhotoSensor);
        dev.printf("%d\t", 0);
        dev.printf("%d\t", sensors->Distrb);
        dev.printf("%d\t", sensors->DistB);
        dev.printf("%d\t", sensors->Distlb);
        dev.printf("\n");

        dev.putc(ETX);
};
