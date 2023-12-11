#ifndef _SPILIB_H_
#define _SPILIB_H_

/*
   - - - - - - - - - - - - - - - Slave- - - - - - - - - - - - - -
   初期化:   Slave(short FirstReply,InterruptIn& SSEL);
   FirstReplyは、最初に.replyに設定される値です。　SSELは、ssピンのInterruptInの変数の参照です。　
   MOSI,MISO,SCLK,SSのそれぞれのピンは、Define使ってプリプロセッサ側で設定してください。

   通信 : short SetData(short value);
   valueをreplyに格納して、受信した値を戻り値に出します。
   
   - - - - - - - - - - - - - -Master- - - - - - - - - - - - - - -
   初期化: Master();
   MOSI,MISO,SCLK,SSのそれぞれのピンは、Define使ってプリプロセッサ側で設定してください。
   送信:short Send(short value);
   valueを送信し、受信した値を戻します。

   - - - - - - - - - - -(Additional Information)- - - - - - - - - - - - - - -

   両クラスはそれぞれmbed::SPISlaveとmbed::SPI の継承(それぞれを引き継いで機能を拡張した感じのやつ)です。
   したがって、Slave::recieve() みたいな感じで、基底クラス(=SPISlave/SPI)のメソッドを呼ぶこともできます。
 */
#include "mbed.h"
#include "SysDefs.hpp"
#include "sys.hpp"


class Slave : SPISlave {

private:
Ticker CheckBuffer;
Timer Reset;
short NextSend;//The data that will be sent
void msgReceived();
void SPI_CHECK();

public:


bool sentonce;

Slave(short FirstReply,InterruptIn& SSEL);
bool SetData(short value);
short data;//Recived data

};


class Master : SPI {

public:
Master();
short Send(short value);
short data;
~Master();
};


#endif
