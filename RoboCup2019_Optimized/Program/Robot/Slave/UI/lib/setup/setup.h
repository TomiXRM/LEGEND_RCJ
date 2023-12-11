#ifndef _SETUP_H_
#define _SETUP_H_

#include "mbed.h"
#include "LGSW_V2.h"
#include "LGBZ.h"
#include "LGRGB.h"
#include "LGKicker.h"
#include "Adafruit_SSD1306.h"

#define INLINE inline __attribute__((always_inline))
#define PI 3.14159265359

#define MODE_MAX 8
#define MODE_BALL_NUM 1

#define START_MODE    0
#define BALL_MODE     1
#define LINE_MODE     2
#define COMPASS_MODE  3
#define US_MODE       4
#define CAM_MODE      5
#define DEBUG_MODE    6
#define SPEED_MODE    7
#define KICKER_MODE   8
#define MOTOR_MODE    9


#define MAIN     'A'
#define BALL     'B'
#define LINE     'C'
#define COMPASS  'D'
#define US       'E'
#define CAM      'F'
#define DEBUG    'G'
#define SPEED    'H'
#define MOTOR    'I'
#define MODESW   'M'



#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define CenterX(_size,_charnum,_x)(_x-(((6*(_charnum-1)*_size)+(5*_size))/2))
#define CenterY(_size,_y)(_y-(7/2*_size))

extern RawSerial pc(PA_2,NC);
extern RawSerial Main(PA_9,PA_10);
extern I2C i2c(PB_7,PB_6);
extern Adafruit_SSD1306_I2c LCD(i2c,PB_7,0x78,64,128);//よくわからんけどリセットをどっかのピンにセットしないとバグる
extern AnalogIn Battery(PA_3);
extern InterruptIn INT(PA_8);
extern DigitalIn KickerIn(PA_11);
extern DigitalOut START(PA_0);
extern DigitalOut LED_2(PA_15);
extern DigitalOut LED_1(PB_3);

extern LGRGB LED(PF_0,PF_1,PA_7);
extern LGKicker Kicker(PA_12,PA_1);
extern LGSW SW(PB_4,PB_0,PB_1,PB_5);
extern LGBZ SP(PB_5);

extern DigitalOut led1(PB_3);
extern DigitalOut led2(PA_15);

struct {
        short Mode;
        bool Start;//スタートボタンのフラグ
} UI;

#endif // SETUP_H
