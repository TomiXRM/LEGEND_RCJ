#define LGBZ_H
#ifdef  LGBZ_H

#define tone_A  262
#define tone_S  294
#define tone_D  330
#define tone_F  349
#define tone_G  392
#define tone_H  440
#define tone_J  494
#define tone_K  523
#define BZoff 7

#define volume 0.5
#include "mbed.h"


class LGBZ {
public:
LGBZ(PinName pin);
void sound(short num);
void State(bool ONOFF);
bool GetState();
PwmOut Buzzer;
bool Bstate;        //state of sound
private:
uint8_t Lastmode;
};

#endif
