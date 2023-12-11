#ifndef _SYS_H_
#define _SYS_H_

class sys
{
//private:

public:
sys();
unsigned char status;
void sysLog(unsigned char Code,char msg[]);
/*
Status number and sysLog's Code has to use  same code mapping.
(Code Mapping)
0x00:Undefined
*/


};

#endif
