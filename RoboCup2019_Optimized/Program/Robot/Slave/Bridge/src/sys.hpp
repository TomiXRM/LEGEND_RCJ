#ifndef _SYS_H_
#define _SYS_H_

class sys
{
//private:
public:
sys();
void AskMode();
unsigned char status;
unsigned char mode;
void sysLog(unsigned char Code,char msg[]);
/*
   Status number and sysLog's Code has to use  same code mapping.
   (Code Mapping)
   0x00:Undefined
   0x01:Started
   0x02:SPILib Init
   0x03:SPILib Communication Check
   0x04:SPILib Communication Fine
   0x05:SPILib Slave succeed
   0x06:SPILib Slave faild
   0x07:SPILib Master succeed
   0x08:SPILib Master faild
   0x09:SPP Init
   0x0A:SPP Communication Check
   0x0B:SPP Fine
   0x0C:SPP Sent
   0x0D:SPP Recived
   0x0E:Initializing Error
   :
   :
   0xFF: Info

 */



};

#endif
