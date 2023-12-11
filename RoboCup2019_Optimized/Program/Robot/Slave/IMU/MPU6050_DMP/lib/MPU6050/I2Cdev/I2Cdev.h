//ported from arduino library: https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050
//written by szymon gaertig (email: szymon@gaertig.com.pl)
// modified by shundo kishi
//
// Changelog:
// 2013-01-08 - first release
// 2016-01-31 - changed all functions and variables static to make porting from arduino easier(by shundo kishi)

#ifndef I2Cdev_h
#define I2Cdev_h

#include "mbed.h"

#define I2C_SDA PB_7
#define I2C_SCL PB_6

class I2Cdev {
private:
static I2C i2c;
public:
I2Cdev();

static int8_t readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout=I2Cdev::readTimeout());
static int8_t readBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data, uint16_t timeout=I2Cdev::readTimeout());
static int8_t readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, uint16_t timeout=I2Cdev::readTimeout());
static int8_t readBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data, uint16_t timeout=I2Cdev::readTimeout());
static int8_t readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout=I2Cdev::readTimeout());
static int8_t readWord(uint8_t devAddr, uint8_t regAddr, uint16_t *data, uint16_t timeout=I2Cdev::readTimeout());
static int8_t readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout=I2Cdev::readTimeout());
static int8_t readWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data, uint16_t timeout=I2Cdev::readTimeout());

static bool writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
static bool writeBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data);
static bool writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
static bool writeBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data);
static bool writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
static bool writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data);
static bool writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
static bool writeWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data);

static uint16_t readTimeout(void);
};

#endif
