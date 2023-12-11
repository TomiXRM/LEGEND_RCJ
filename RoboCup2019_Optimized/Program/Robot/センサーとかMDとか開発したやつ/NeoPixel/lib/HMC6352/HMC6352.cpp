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

/**
 * Includes
 */
#include "HMC6352.h"

HMC6352::HMC6352(PinName sda, PinName scl) {

    i2c_ = new I2C(sda, scl);
    //100KHz, as specified by the datasheet.
    i2c_->frequency(200000);

    operationMode_ = getOpMode();

}

int HMC6352::get(void) {

    char tx[1];
    char rx[2];

    if (operationMode_ == HMC6352_STANDBY || operationMode_ == HMC6352_QUERY) {
        tx[0] = HMC6352_GET_DATA;

        i2c_->write((HMC6352_I2C_ADDRESS << 1) & 0xFE, tx, 1);
        wait_ms(1);
    }

    i2c_->read((HMC6352_I2C_ADDRESS << 1) | 0x01, rx, 2);
    wait_ms(1);

    return (((int)rx[0] << 8) | (int)rx[1]);

}

void HMC6352::setSleepMode(int exitOrEnter) {

    char tx[1];

    tx[0] = exitOrEnter;

    i2c_->write((HMC6352_I2C_ADDRESS << 1) & 0xFE, tx, 1);
    wait_ms(1);

}

void HMC6352::setReset(void) {

    char tx[1];

    tx[0] = HMC6352_SET_RESET;

    i2c_->write((HMC6352_I2C_ADDRESS << 1) & 0xFE, tx, 1);
    wait_ms(7);

}

void HMC6352::setCalibrationMode(int exitOrEnter) {

    char tx[1];
    int delay = 0;

    tx[0] = exitOrEnter;

    if (exitOrEnter == HMC6352_EXIT_CALIB) {
        delay = 15;
    } else if (exitOrEnter == HMC6352_ENTER_CALIB) {
        delay = 1;
    }

    i2c_->write((HMC6352_I2C_ADDRESS << 1) & 0xFE, tx, 1);
    wait_ms(delay);

}

void HMC6352::saveOpMode(void) {

    char tx[1];

    tx[0] = HMC6352_SAVE_OPMODE;

    i2c_->write((HMC6352_I2C_ADDRESS << 1) & 0xFE, tx, 1);
    wait_ms(1);

}

int HMC6352::getSlaveAddress(void) {

    return read(HMC6352_EEPROM_READ, HMC6352_SLAVE_ADDR);

}

int HMC6352::getOffset(int axis) {

    char rx[2] = {0x00, 0x00};

    if (axis == HMC6352_MX_OFFSET) {

        rx[0] = read(HMC6352_EEPROM_READ, HMC6352_MX_OFF_MSB);
        rx[1] = read(HMC6352_EEPROM_READ, HMC6352_MX_OFF_LSB);

    } else if (axis == HMC6352_MY_OFFSET) {

        rx[0] = read(HMC6352_EEPROM_READ, HMC6352_MY_OFF_MSB);
        rx[1] = read(HMC6352_EEPROM_READ, HMC6352_MY_OFF_LSB);

    }

    return ((rx[0] << 8) | (rx[1]));

}

void HMC6352::setOffset(int axis, int offset) {

    char tx[2] = {0x00, 0x00};

    tx[0] = (offset & 0x0000FF00) >> 8;
    tx[1] = (offset & 0x000000FF);

    if (axis == HMC6352_MX_OFFSET) {

        write(HMC6352_EEPROM_WRITE, HMC6352_MX_OFF_MSB, tx[0]);
        write(HMC6352_EEPROM_WRITE, HMC6352_MX_OFF_MSB, tx[1]);

    } else if (axis == HMC6352_MY_OFFSET) {

        write(HMC6352_EEPROM_WRITE, HMC6352_MY_OFF_MSB, tx[0]);
        write(HMC6352_EEPROM_WRITE, HMC6352_MY_OFF_MSB, tx[1]);

    }

}

int HMC6352::getTimeDelay(void) {

    return read(HMC6352_EEPROM_READ, HMC6352_TIME_DELAY);

}

void HMC6352::setTimeDelay(int delay) {

    write(HMC6352_EEPROM_WRITE, HMC6352_TIME_DELAY, delay);

}

int HMC6352::getSumNumber(void) {

    return read(HMC6352_EEPROM_READ, HMC6352_SUMMED);

}

void HMC6352::setSumNumber(int sum) {

    write(HMC6352_EEPROM_WRITE, HMC6352_SUMMED, sum);

}

int HMC6352::getSoftwareVersion(void) {

    return read(HMC6352_EEPROM_READ, HMC6352_SOFT_VER);

}

int HMC6352::getOpMode(void) {

    int response = 0;

    response = read(HMC6352_RAM_READ, HMC6352_RAM_OPMODE);

    return (response & 0x00000003);

}

void HMC6352::setOpMode(int mode, int periodicSetReset, int measurementRate) {

    char opModeByte = mode;

    if (periodicSetReset == 1) {
        opModeByte |= HMC6352_PERIODIC_SR;
    }

    if (measurementRate == 5) {
        opModeByte |= HMC6352_CM_MR_5HZ;
    } else if (measurementRate == 10) {
        opModeByte |= HMC6352_CM_MR_10HZ;
    } else if (measurementRate == 20) {
        opModeByte |= HMC6352_CM_MR_20HZ;
    }

    write(HMC6352_RAM_WRITE, HMC6352_RAM_OPMODE, opModeByte);
    write(HMC6352_EEPROM_WRITE, HMC6352_OPMODE, opModeByte);

    operationMode_ = mode;

}

int HMC6352::getOutputMode(void) {

    return read(HMC6352_RAM_READ, HMC6352_RAM_OUTPUT);

}

void HMC6352::setOutputMode(int mode) {

    write(HMC6352_RAM_WRITE, HMC6352_RAM_OUTPUT, mode);

}

void HMC6352::write(int EepromOrRam, int address, int data) {

    char tx[3];

    tx[0] = EepromOrRam;
    tx[1] = address;
    tx[2] = data;

    i2c_->write((HMC6352_I2C_ADDRESS << 1) & 0xFE, tx, 3);
    wait_ms(1);

}

int HMC6352::read(int EepromOrRam, int address) {

    char tx[2];
    char rx[1];

    tx[0] = EepromOrRam;
    tx[1] = address;

    i2c_->write((HMC6352_I2C_ADDRESS << 1) & 0xFE, tx, 2);
    wait_ms(1);
    i2c_->read((HMC6352_I2C_ADDRESS << 1) | 0x01, rx, 1);
    wait_ms(1);

    return (rx[0]);

}
