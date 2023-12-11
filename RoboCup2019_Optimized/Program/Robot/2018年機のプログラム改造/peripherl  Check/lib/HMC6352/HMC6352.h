/**
 * @author Aaron Berk
 * 
 * @section LICENSE
 *
 * Copyright (c) 2010 ARM Limited
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * Honeywell HMC6352 digital compass.
 *
 * Datasheet:
 *
 * http://www.ssec.honeywell.com/magnetic/datasheets/HMC6352.pdf
 */

#ifndef HMC6352_H
#define HMC6352_H

/**
 * Includes
 */
#include "mbed.h"

/**
 * Defines
 */
#define HMC6352_I2C_ADDRESS  0x21 //7-bit address. 0x43 write, 0x42 read.

//Commands.
#define HMC6352_EEPROM_WRITE 0x77
#define HMC6352_EEPROM_READ  0x72
#define HMC6352_RAM_WRITE    0x47
#define HMC6352_RAM_READ     0x67
#define HMC6352_ENTER_SLEEP  0x53
#define HMC6352_EXIT_SLEEP   0x57
#define HMC6352_SET_RESET    0x4F
#define HMC6352_ENTER_CALIB  0x43
#define HMC6352_EXIT_CALIB   0x45
#define HMC6352_SAVE_OPMODE  0x4C
#define HMC6352_GET_DATA     0x41

//EEPROM locations.
#define HMC6352_SLAVE_ADDR   0x00
#define HMC6352_MX_OFF_MSB   0x01
#define HMC6352_MX_OFF_LSB   0x02
#define HMC6352_MY_OFF_MSB   0x03
#define HMC6352_MY_OFF_LSB   0x04
#define HMC6352_TIME_DELAY   0x05
#define HMC6352_SUMMED       0x06
#define HMC6352_SOFT_VER     0x07
#define HMC6352_OPMODE       0x08

//RAM registers.
#define HMC6352_RAM_OPMODE   0x74
#define HMC6352_RAM_OUTPUT   0x4E

#define HMC6352_MX_OFFSET    0x00
#define HMC6352_MY_OFFSET    0x01

#define HMC6352_HEADING_MODE 0x00
#define HMC6352_RAWMAGX_MODE 0x01
#define HMC6352_RAWMAGY_MODE 0x02
#define HMC6352_MAGX_MODE    0x03
#define HMC6352_MAGY_MODE    0x04

//Operational mode register masks.
#define HMC6352_CM_MR_1HZ    0x00
#define HMC6352_CM_MR_5HZ    0x20
#define HMC6352_CM_MR_10HZ   0x40
#define HMC6352_CM_MR_20HZ   0x60

#define HMC6352_PERIODIC_SR  0x10

#define HMC6352_STANDBY      0x00
#define HMC6352_QUERY        0x01
#define HMC6352_CONTINUOUS   0x02

/**
 * Honeywell HMC6352 digital compass.
 */
class HMC6352 {

public:

    /**
     * Constructor.
     *
     * @param sda mbed pin to use for SDA line of I2C interface.
     * @param scl mbed pin to use for SCL line of I2C interface.
     */
    HMC6352(PinName sda, PinName scl);

    /**
     * Sample the device and return the result.
     *
     * @return In heading output mode, the current heading as a number between
     *         0-3599, representing 0-359.9 degrees.
     *         In raw magnetometer X output mode, the raw output of the X-axis
     *         magnetometer.
     *         In raw magnetometer Y mode, the raw output of the Y-axis
     *         magnetometer.
     *         In magnetometer X mode, the corrected output of the X-axis
     *         magnetometer.
     *         In magnetometer Y mode, the corrected output of the Y-axis
     *         magnetometer.
     */
    int get(void);

    /**
     * Enter into or exit from sleep mode.
     *
     * @param enterOrExit 0x57 -> Exit sleep mode
     *                    0x53 -> Enter
     */
    void setSleepMode(int enterOrExit);

    /**
     * Update bridge offsets.
     *
     * Performs a set/reset immediately.
     */
    void setReset(void);

    /**
     * Enter into or exit from calibration mode.
     *
     * @param enterOrExit 0x45 -> Exit
     *                    0x43 -> Enter
     */
    void setCalibrationMode(int enterOrExit);

    /**
     * Save the current operation mode byte to EEPROM.
     */
    void saveOpMode(void);

    /**
     * Read the memory location on the device which contains the slave address.
     *
     * @return The slave address of the device.
     */
    int getSlaveAddress(void);

    /**
     * Read the current offset for X or Y axis magnetometer.
     *
     * @param axis 0x00 -> X-axis
     *             0x01 -> Y-axis
     * @return The current offset for the axis as a 16-bit number.
     */
    int getOffset(int axis);

    /**
     * Set the offset for X or Y axis magnetometer.
     *
     * @param axis 0x00 -> X-axis
     *             0x01 -> Y-axis
     * @param offset The offset to set for the axis (set to a 16-bit number).
     */
    void setOffset(int axis, int offset);

    /**
     * Get the current time delay.
     *
     * @return The time delay in milliseconds.
     */
    int getTimeDelay(void);

    /**
     * Set the time delay of the device.
     *
     * @param delay The time delay to set in milliseconds (between 0-255);
     */
    void setTimeDelay(int delay);

    /**
     * Get the number of measurements being summed each sample.
     *
     * @return The number of summed measurement.
     */
    int getSumNumber(void);

    /**
     * Set the number of measurements being summed each sample.
     *
     * @param sum The number of measurements to be summed (between 0-16);
     */
    void setSumNumber(int sum);

    /**
     * Get the software version on the device.
     *
     * @return The software version number.
     */
    int getSoftwareVersion(void);

    /**
     * Get the current operation mode.
     *
     * @return 0x00 -> Standby mode
     *         0x01 -> Query mode
     *         0x02 -> Continuous mode
     */
    int getOpMode(void);

    /**
     * Set the operation mode.
     *
     * @param mode 0x00 -> Standby mode
     *             0x01 -> Query mode
     *             0x02 -> Continuous mode
     * @param periodicSetReset 0x00 -> No periodic set/reset
     *                         0x01 -> Periodic set/reset
     * @measurementRate Measurement rate in Hz for continuous rate.
     *                  Possible rates: {1, 5, 10, 20}Hz.
     */
    void setOpMode(int mode, int periodicSetReset, int measurementRate = 0);

    /**
     * Get the current output mode of the device.
     *
     * @return The current output mode.
     */
    int getOutputMode(void);

    /**
     * Set the output mode of the device.
     *
     * @param mode 0x00 -> Heading mode
     *             0x01 -> Raw magnetometer X mode
     *             0x02 -> Raw magnetometer Y mode
     *             0x03 -> Magnetometer X mode
     *             0x04 -> Magnetometer Y mode
     */
    void setOutputMode(int mode);

private:

    I2C* i2c_;
    int  operationMode_;

    /**
     * Write to EEPROM or RAM on the device.
     *
     * @param EepromOrRam 0x77 -> Writing to EEPROM
     *                    0x47 -> Writing to RAM
     * @param address Address to write to.
     * @param data Data to write.
     */
    void write(int EepromOrRam, int address, int data);

    /**
     * Read EEPROM or RAM on the device.
     *
     * @param EepromOrRam 0x72 -> Reading from EEPROM
     *                    0x67 -> Reading from RAM
     * @param address Address to read from.
     * @return The contents of the memory address.
     */
    int read(int EepromOrRam, int address);

};

#endif /* HMC6352_H */
