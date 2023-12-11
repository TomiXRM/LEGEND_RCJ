#include "mbed.h"
#include "SerialDebug.h"


SerialDebug::SerialDebug() : pc(USBTX,USBRX){
        pc.baud(115200);
        Period = 0.1;
}

void SerialDebug::setDebug(float period){
        Period = period;
        ticker.attach(mbed::callback(this,&SerialDebug::callback),Period);
}

void SerialDebug::enable(){
        ticker.attach(mbed::callback(this,&SerialDebug::callback),Period);
}

void SerialDebug::disable(){
        ticker.detach();
}

void SerialDebug::callback(){
        //pc.printf("Hello world\t%d\r\n");
        Ballprint();
        //IMUprint();
        printf("\r\n");
}
/****************************************************************************************/
void SerialDebug::Ballprint(){
        //pc.printf("IRangle:%d\t",Ball.intdat);
        pc.printf("received:%d \tRawdeg:%d \tdeg:%d\tlong:%d \t",Ball.intdat, Ball.IRangle, Ball.adjustment(60, 30, 0),Ball.IRlong);
}

void SerialDebug::IMUprint(){
        pc.printf("dir:%d\t",IMU.dir);
}
