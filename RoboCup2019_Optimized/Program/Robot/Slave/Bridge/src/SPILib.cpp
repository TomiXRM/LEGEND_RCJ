/*SPI abstruction ibrary.
   Prototype is at SPILib.hpp
   Documentation is also at SPILib.hpp.
 */

//Defines
#include "mbed.h"
#include "SysDefs.hpp"
#include "sys.hpp"
#include "SPILib.hpp"

#define SPIFreq 8000000
#define SPIDelay_us 20

extern sys sysobj;
extern InterruptIn ss;

//Slave Codes
void Slave::SPI_CHECK(){
        if(SPISlave::receive()) {
                data = SPISlave::read();
                sentonce=true;

                char Logmsg[]="";
                sprintf(Logmsg,"Recieved:%d Sent:%d",data,NextSend);
                sysobj.sysLog(0x05,Logmsg);
        }

};

void Slave::msgReceived(){

        CheckBuffer.detach();//タイマー割り込みの禁止
        if(SPISlave::receive()) {//データが来ていたら読み込み
                data = SPISlave::read();//読み込み
        }
        SPISlave::reply(NextSend);        //返信データのセット
        sentonce=true;
        char Logmsg[]="";
        sprintf(Logmsg,"Recieved:%d Sent:%d",data,NextSend);
        sysobj.sysLog(0x05,Logmsg);

        Reset.reset();        //監視タイマーリセット

        CheckBuffer.attach_us(callback(this,&Slave::SPI_CHECK),50); //タイマー割り込みのセット
}

Slave::Slave(short FirstReply,InterruptIn& SSEL) : SPISlave(MOSI,MISO,SCLK,SS){


        data=0x00;

        char str[]="Slave Init";

        sysobj.sysLog(0x02,str);

        SPISlave::format(16,3);
        SPISlave::frequency(SPIFreq); //SPI Frequency
        SPISlave::reply(FirstReply);//最初の返信データのセット
        sentonce=true;

        SSEL.mode(PullUp);//プルアップしておけばバグりにくい
        Reset.start();  //ソフトウェアリセットをする為の監視タイマー
        SSEL.fall(callback(this,&Slave::msgReceived));//SSピンのピン割り込み
        CheckBuffer.attach_us(callback(this,&Slave::SPI_CHECK),50); //タイマー割り込みのセット

}



bool Slave::SetData(short value){
        sentonce=false;
        NextSend=value;

        return true;
}

//Master Codes

DigitalOut SSEL(SS);

Master::Master() : SPI(MOSI,MISO,SCLK){


        SPI::format(16, 3);//16bit.mode3
        SPI::frequency(SPIFreq); // SPI Frequency
        SSEL= 1;//HIGHにしておかないとスレーブが誤認識する。
        sysobj.sysLog(0x00,"SPI Communication Mode.");

}


short Master::Send(short value){

        SSEL= 0;        //アドレスで指定した通信相手のCSピンをLOWにして呼び出し
        data = SPI::write(value);        //読み込み
        wait_us(SPIDelay_us);
        SSEL = 1;        //SSピンをHIGHにして通信終了
        return data;
}



Master::~Master(){
}
