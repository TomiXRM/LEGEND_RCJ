#ifndef LGLINE_H
#define LGLINE_H
#include "mbed.h"

#define F 1
#define R 2
#define B 3
#define L 4

class LGLINE {
public:
void Setinterval(int interval);
bool IsOnLine();
int getdeg();

private:
DigitalIn lineF;
DigitalIn lineR;
DigitalIn lineB;
DigitalIn lineL;

Ticker lineOP;
};
#endif
