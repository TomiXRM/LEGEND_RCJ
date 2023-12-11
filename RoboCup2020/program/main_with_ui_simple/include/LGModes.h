#ifndef _LGMODES_H_
#define _LGMODES_H_
#include <mbed.h>
#include "colors.h"

typedef struct{
  char* name; // 機能の名前. メニューに出る
  mbed::Callback<void()> before;
  mbed::Callback<void()> body;
  mbed::Callback<void()> after;
  color modeColor;
} LG_mode_t;

#endif
