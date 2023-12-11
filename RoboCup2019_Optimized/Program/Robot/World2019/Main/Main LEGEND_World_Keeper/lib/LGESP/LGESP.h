#include "mbed.h"
#include "setup_com.h"
#include "stdarg.h"

class LGESP {
public:
LGESP(PinName tx, PinName rx, type_sensors *_sensors, type_camera *_camera,
      RawSerial *_pc);
RawSerial dev;
void collectData();
void debug(const char *format, ...);

private:
type_sensors *sensors;
type_camera *camera;
RawSerial *pc;
};
