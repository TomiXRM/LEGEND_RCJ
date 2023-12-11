#ifndef LGUI_H
#define LGUI_H
#include "mbed.h"
#include <vector>

#define DONOTHING callback(&doNothing)

void doNothing();

class LGUI {
public:
LGUI(RawSerial* _pc,DigitalOut *_slp,PinName tx,PinName rx);


//Values and objects
char mode;
char modeLast;//Require for diff check
uint8_t state;
uint8_t data_length;
RawSerial device;

uint8_t speed;//TODO:ここにこの定義はクソキモい。(モード専用機能はここにはつけたくない。)どこかに出せるようにしたいけどどうすればいいかわかりましぇん。


void attachRx (Callback< void()> method);
void Entry(char c,Callback<void()> before, Callback<void()>loop,Callback<void()> after);

void Run();//Called once a loop.

void  SetLED(bool r, bool g, bool b);
void  SetBZ(uint8_t i);
void  SetKICKER(bool flg);
//Wrapper functions
void putc(char c);
char getc();
void baud(uint64_t baudLate);


void Before_ModeSW();
void Body_ModeSW();
//MODESWで呼び出しする関数
void ModeSelect();

private:
RawSerial *pc;
DigitalOut *slp;
//Mode Tables
std::vector<char> modeList;
std::vector< Callback<void()> > Before;
std::vector< Callback<void()> > Loop;
std::vector< Callback<void()> > After;

//Table Search
unsigned char findIndex(char index);
};


#endif
