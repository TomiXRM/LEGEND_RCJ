#include "setup_com.h"
#include "setup.h"

float CompassModedir;
float CompassModeTurndir;


extern LGUI ui;
extern RawSerial pc;
extern Motors MD;
extern LGIMU IMU;
PID CompassModePidDir(&CompassModedir, &CompassModeTurndir,0, 1.4, 0, 0.07,DIRECT);//方向制御のPID);

void Body_Compass();
void stopAll();

void initCompassProcess(){
        ui.Entry(COMPASS,DONOTHING,callback(&Body_Compass),DONOTHING);
        CompassModePidDir.SetMode(AUTOMATIC);
        CompassModePidDir.SetOutputLimits(-70,70);
        CompassModePidDir.SetSampleTime(9);//5
}
void Body_Compass(){
        // pc.printf("Compass Mode!!\r\n");
        // CompassModedir = (0.5-IMU.read()* -360)-180;//向いている角度の取得
        IMU.get();
        CompassModedir = IMU.dir;
        CompassModePidDir.Compute();
        if(abs(CompassModedir) < 4) {CompassModeTurndir = 0;}
        MD.run(CompassModeTurndir,CompassModeTurndir,CompassModeTurndir);
        pc.printf("%d\n",(short)(CompassModedir));
}
