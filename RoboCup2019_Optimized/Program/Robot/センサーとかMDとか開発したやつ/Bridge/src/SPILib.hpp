#ifndef _SPILIB_H_
#define _SPILIB_H_

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
~Slave();
};


class Master : SPI {

public:
Master();
short Send(short value);
short data;
~Master();
};


#endif
