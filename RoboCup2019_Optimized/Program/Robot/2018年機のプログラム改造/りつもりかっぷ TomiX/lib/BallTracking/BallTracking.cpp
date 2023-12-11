#include "BallTracking.h"

void BallTracking(){
        sensors.IRreceive = Ball.read();
        sensors.IRangle = Ball.readIRangle();
        sensors.IRlong = Ball.readIRlong();
        movingdeg = Ball.adjustment(60, 30, 10);
        if (sensors.IRangle != 5000) {
                if (sensors.IRlong == 5) {
                        if(abs(sensors.IRangle) <=70) {//前方向
                                //減速する

                                if(abs(sensors.IRangle) <=30) {

                                        MD.drive(movingdeg, 100,sensors.dir );
                                        if (Photo.read() < 0.65) {
                                                if (Tikikker.read_ms() > 150) {
                                                        if (kicked == 0) {
                                                                kikker2.attach(&kick, 0.05);
                                                                kicked = 1;
                                                        }
                                                }
                                        } else {
                                                solenoid = 0;
                                        }
                                }else{
                                        MD.drive(movingdeg, 80,sensors.dir );
                                }
                        }else{
                                MD.drive(movingdeg, 100,sensors.dir );
                        }

                } else if (sensors.IRlong == 4) {

                        MD.drive(movingdeg, 100,sensors.dir );
                        if (abs(sensors.IRangle) <= 30) {
                                if (Photo.read() < 0.65) {
                                        if (Tikikker.read_ms() > 150) {
                                                if (kicked == 0) {
                                                        kikker2.attach(&kick, 0.05);
                                                        kicked = 1;
                                                }
                                        }
                                } else {
                                        solenoid = 0;
                                }
                        }
                }else if(sensors.IRlong == 3) {
                        MD.drive(movingdeg, 60,sensors.dir );
                }
        } else {
                /*if(US1.read()*300 > 50) {
                        MD.drive(180,100,sensors.dir);
                   }else*/{
                        MD.move(0, 0, 0, 0,sensors.dir );
                }
                //outofbounds.movingdeg = 5000;
        }
}
