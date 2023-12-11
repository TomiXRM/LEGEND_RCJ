#ifndef _COLORS_H_
#define _COLORS_H_
#define LEDON 0
typedef struct{
 bool r;
 bool g;
 bool b;
} color;


const color red=  {r: LEDON,g:!LEDON,b:!LEDON};
const color green={r:!LEDON,g: LEDON,b:!LEDON};
const color blue={r:!LEDON,g:!LEDON,b: LEDON};
const color white={r: LEDON,g: LEDON,b: LEDON};
const color yellow={r:LEDON,g: LEDON,b: !LEDON};
const color magenta={r:LEDON,g:!LEDON,b:LEDON};
const color cyan={r:!LEDON,g:LEDON,b:LEDON};

const color colors[7]={red,green,blue,white,yellow,magenta,cyan};
#endif
