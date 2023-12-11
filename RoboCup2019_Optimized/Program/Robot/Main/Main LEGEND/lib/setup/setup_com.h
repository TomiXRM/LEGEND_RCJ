#ifndef _SETUP_COM_H_
#define _SETUP_COM_H_

#define INLINE inline __attribute__((always_inline))
#define PI 3.14159265358979323846264338327

#define MAIN     'A'
#define BALL     'B'
#define LINE     'C'
#define COMPASS  'D'
#define US       'E'
#define CAM      'F'
#define DEBUG    'G'
#define SPEED    'H'
#define MODESW   'M'

#define SETPOSITION     'a'//中立点決め
#define GOTOHOME        'b'//ホームポジションに戻れるかのチェック
#define FACINGOPSGOAL   'c'//相手ゴールを向くことができるかのチェック
#define POSITIONMONITOR 'd'//ポジションモニター(座標)
#define BAUDRATE 1000000

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))


#endif
