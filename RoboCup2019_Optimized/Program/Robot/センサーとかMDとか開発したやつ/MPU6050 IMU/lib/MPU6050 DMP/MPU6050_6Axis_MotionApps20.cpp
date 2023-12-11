// I2Cdev library collection - MPU6050 I2C device class, 6-axis MotionApps 2.0 implementation
// Based on InvenSense MPU-6050 register map document rev. 2.0, 5/19/2011 (RM-MPU-6000A-00)
// 5/20/2013 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     ... - ongoing debug release
 
/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/
 
#include "MPU6050_6Axis_MotionApps20.h"
 
uint8_t MPU6050::dmpInitialize()
{
    // reset device
    wait_ms(50);
    reset();
    wait_ms(30);
 
    // enable sleep mode and wake cycle
    /*Serial.println(F("Enabling sleep mode..."));
    setSleepEnabled(true);
    Serial.println(F("Enabling wake cycle..."));
    setWakeCycleEnabled(true);*/
 
    // disable sleep mode
    //DEBUG_PRINT("Disabling sleep mode...\n");
    setSleepEnabled(false);
 
    // get MPU hardware revision
    //DEBUG_PRINT("Selecting user bank 16...\n");
    setMemoryBank(0x10, true, true);
    //DEBUG_PRINT("Selecting memory byte 6...\n");
    setMemoryStartAddress(0x06);
    //DEBUG_PRINT("Checking hardware revision...\n");
    uint8_t hwRevision = readMemoryByte();
    //DEBUG_PRINT("Revision @ user[16][6] = ");
    //DEBUG_PRINTF("%x\n",hwRevision);
    //DEBUG_PRINT("Resetting memory bank selection to 0...\n");
    setMemoryBank(0, false, false);
 
    // check OTP bank valid
    //DEBUG_PRINT("Reading OTP bank valid flag...\n");
    uint8_t otpValid = getOTPBankValid();
 
    //DEBUG_PRINT("OTP bank is ");
    if(otpValid);  //DEBUG_PRINT("valid!\n");
    else;  //DEBUG_PRINT("invalid!\n");
 
    // get X/Y/Z gyro offsets
    /*
    DEBUG_PRINT("\nReading gyro offset TC values...\n");
    int8_t xgOffsetTC = mpu.getXGyroOffsetTC();
    int8_t ygOffsetTC = getYGyroOffsetTC();
    int8_t zgOffsetTC = getZGyroOffsetTC();
    DEBUG_PRINTF("X gyro offset = %u\n",xgOffset);
    DEBUG_PRINTF("Y gyro offset = %u\n",ygOffset);
    DEBUG_PRINTF("Z gyro offset = %u\n",zgOffset);
    */
    // setup weird slave stuff (?)
    //DEBUG_PRINT("Setting slave 0 address to 0x7F...\n");
    setSlaveAddress(0, 0x7F);
 
    //DEBUG_PRINT("Disabling I2C Master mode...");
    setI2CMasterModeEnabled(false);
    //DEBUG_PRINT("Setting slave 0 address to 0x68 (self)...");
    setSlaveAddress(0, 0x68);
    //DEBUG_PRINT("Resetting I2C Master control...\n");
    resetI2CMaster();
 
    wait_ms(20);
 
    // load DMP code into memory banks
    //DEBUG_PRINT("Writing DMP code to MPU memory banks (");
    //DEBUG_PRINTF("%u",MPU6050_DMP_CODE_SIZE);
    //DEBUG_PRINT(" bytes)\n");
    if (writeProgMemoryBlock(dmpMemory, MPU6050_DMP_CODE_SIZE)) {
        //DEBUG_PRINT("Success! DMP code written and verified.\n");
 
        // write DMP configuration
        //DEBUG_PRINT("Writing DMP configuration to MPU memory banks (");
        //DEBUG_PRINTF("%u",MPU6050_DMP_CONFIG_SIZE);
        //DEBUG_PRINT(" bytes in config def)\n");
        if (writeProgDMPConfigurationSet(dmpConfig, MPU6050_DMP_CONFIG_SIZE)) {
            //DEBUG_PRINT("Success! DMP configuration written and verified.\n");
 
            //DEBUG_PRINT("Setting clock source to Z Gyro...\n");
            setClockSource(MPU6050_CLOCK_PLL_ZGYRO);
 
            //DEBUG_PRINT("Setting DMP and FIFO_OFLOW interrupts enabled...\n");
            setIntEnabled(0x12);
 
            //DEBUG_PRINT("Setting sample rate to 200Hz...");
            setRate(IMU_SAMPLE_RATE_DIVIDER); // 1khz / (1 + 4) = 200 Hz
 
            //DEBUG_PRINT("Setting external frame sync to TEMP_OUT_L[0]...\n");
            setExternalFrameSync(MPU6050_EXT_SYNC_TEMP_OUT_L);
 
            //DEBUG_PRINT("Setting DLPF bandwidth to 42Hz...\n");
            setDLPFMode(MPU6050_DLPF_BW_42);
 
            //DEBUG_PRINT("Setting gyro sensitivity to +/- 2000 deg/sec...\n");
            setFullScaleGyroRange(MPU6050_GYRO_FS_1000);
 
            //DEBUG_PRINT("Setting DMP configuration bytes (function unknown)...\n");
            setDMPConfig1(0x03);
            setDMPConfig2(0x00);
 
            //DEBUG_PRINT("Clearing OTP Bank flag...");
            setOTPBankValid(false);
 
            //DEBUG_PRINT("Setting X/Y/Z gyro offset TCs to previous values...\n");
            //setXGyroOffsetTC(xgOffsetTC);
            //setYGyroOffsetTC(ygOffsetTC);
            //setZGyroOffsetTC(zgOffsetTC);
 
            //DEBUG_PRINTLN(F("Setting X/Y/Z gyro user offsets to zero..."));
            //setXGyroOffset(0);
            //setYGyroOffset(0);
            //setZGyroOffset(0);
 
            //DEBUG_PRINT("Writing final memory update 1/7 (function unknown)...\n");
            uint8_t dmpUpdate[16], j;
            uint16_t pos = 0;
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);
 
            //DEBUG_PRINT("Writing final memory update 2/7 (function unknown)...\n");
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);
 
            //DEBUG_PRINT("Resetting FIFO...\n");
            resetFIFO();
 
            //DEBUG_PRINT("Reading FIFO count...\n");
            uint16_t fifoCount = getFIFOCount();
            uint8_t fifoBuffer[128];
 
            //DEBUG_PRINT("Current FIFO count=");
            //DEBUG_PRINTF("%u\n",fifoCount);
            getFIFOBytes(fifoBuffer, fifoCount);
 
            //DEBUG_PRINT("Setting motion detection threshold to 2...\n");
            setMotionDetectionThreshold(2);
 
            //DEBUG_PRINT("Setting zero-motion detection threshold to 156...\n");
            setZeroMotionDetectionThreshold(156);
 
            //DEBUG_PRINT("Setting motion detection duration to 80...");
            setMotionDetectionDuration(80);
 
            //DEBUG_PRINT("Setting zero-motion detection duration to 0...");
            setZeroMotionDetectionDuration(0);
 
            //DEBUG_PRINT("Resetting FIFO...\n");
            resetFIFO();
 
            //DEBUG_PRINT("Enabling FIFO...\n");
            setFIFOEnabled(true);
 
            //DEBUG_PRINT("Enabling DMP...\n");
            setDMPEnabled(true);
 
            //DEBUG_PRINT("Resetting DMP...\n");
            resetDMP();
 
            //DEBUG_PRINT("Writing final memory update 3/7 (function unknown)...\n");
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);
 
            //DEBUG_PRINT("Writing final memory update 4/7 (function unknown)...\n");
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);
 
            //DEBUG_PRINT("Writing final memory update 5/7 (function unknown)...\n");
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);
 
            //DEBUG_PRINT("Waiting for FIFO count > 2...\n");
            while ((fifoCount = getFIFOCount()) < 3);
 
            //DEBUG_PRINT("Current FIFO count=");
            //DEBUG_PRINTF("%u\n",fifoCount);
            //DEBUG_PRINT("Reading FIFO data...\n");
            getFIFOBytes(fifoBuffer, fifoCount);
 
            //DEBUG_PRINT("Reading interrupt status...\n");
            uint8_t mpuIntStatus = getIntStatus();
 
            //DEBUG_PRINT("Current interrupt status=");
            //DEBUG_PRINTF("%x\n",mpuIntStatus);
 
            //DEBUG_PRINT("Reading final memory update 6/7 (function unknown)...\n");
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            readMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);
 
            //DEBUG_PRINT("Waiting for FIFO count > 2...\n");
            while ((fifoCount = getFIFOCount()) < 3);
 
            //DEBUG_PRINT("Current FIFO count=");
            //DEBUG_PRINTF("%u\n",fifoCount);
 
            //DEBUG_PRINT("Reading FIFO data...\n");
            getFIFOBytes(fifoBuffer, fifoCount);
 
            //DEBUG_PRINT("Reading interrupt status...\n");
            mpuIntStatus = getIntStatus();
 
            //DEBUG_PRINT("Current interrupt status=");
            //DEBUG_PRINTF("%x\n",mpuIntStatus);
 
            //DEBUG_PRINT("Writing final memory update 7/7 (function unknown)...");
            for (j = 0; j < 4 || j < dmpUpdate[2] + 3; j++, pos++) dmpUpdate[j] = pgm_read_byte(&dmpUpdates[pos]);
            writeMemoryBlock(dmpUpdate + 3, dmpUpdate[2], dmpUpdate[0], dmpUpdate[1]);
 
            //DEBUG_PRINT("DMP is good to go! Finally.\n");
 
            //DEBUG_PRINT("Disabling DMP (you turn it on later)...\n");
            setDMPEnabled(false);
 
            //DEBUG_PRINT("Setting up internal 42-byte (default) DMP packet buffer...\n");
            dmpPacketSize = 42;
            /*if ((dmpPacketBuffer = (uint8_t *)malloc(42)) == 0) {
                return 3; // TODO: proper error code for no memory
            }*/
 
            //DEBUG_PRINT("Resetting FIFO and clearing INT status one last time...\n");
            resetFIFO();
            getIntStatus();
        } else {
            //DEBUG_PRINT("ERROR! DMP configuration verification failed.\n");
            return 2; // configuration block loading failed
        }
    } else {
        //DEBUG_PRINT("ERROR! DMP code verification failed.");
        return 1; // main binary block loading failed
    }
    return 0; // success
}
 
bool MPU6050::dmpPacketAvailable()
{
    return getFIFOCount() >= dmpGetFIFOPacketSize();
}
 
// uint8_t MPU6050::dmpSetFIFORate(uint8_t fifoRate);
// uint8_t MPU6050::dmpGetFIFORate();
// uint8_t MPU6050::dmpGetSampleStepSizeMS();
// uint8_t MPU6050::dmpGetSampleFrequency();
// int32_t MPU6050::dmpDecodeTemperature(int8_t tempReg);
 
//uint8_t MPU6050::dmpRegisterFIFORateProcess(inv_obj_func func, int16_t priority);
//uint8_t MPU6050::dmpUnregisterFIFORateProcess(inv_obj_func func);
//uint8_t MPU6050::dmpRunFIFORateProcesses();
 
// uint8_t MPU6050::dmpSendQuaternion(uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendGyro(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendAccel(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendLinearAccel(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendLinearAccelInWorld(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendControlData(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendSensorData(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendExternalSensorData(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendGravity(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendPacketNumber(uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendQuantizedAccel(uint_fast16_t elements, uint_fast16_t accuracy);
// uint8_t MPU6050::dmpSendEIS(uint_fast16_t elements, uint_fast16_t accuracy);
 
uint8_t MPU6050::dmpGetAccel(int32_t *data, const uint8_t* packet)
{
    // TODO: accommodate different arrangements of sent data (ONLY default supported now)
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = ((packet[28] << 24) + (packet[29] << 16) + (packet[30] << 8) + packet[31]);
    data[1] = ((packet[32] << 24) + (packet[33] << 16) + (packet[34] << 8) + packet[35]);
    data[2] = ((packet[36] << 24) + (packet[37] << 16) + (packet[38] << 8) + packet[39]);
    return 0;
}
uint8_t MPU6050::dmpGetAccel(int16_t *data, const uint8_t* packet)
{
    // TODO: accommodate different arrangements of sent data (ONLY default supported now)
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = (packet[28] << 8) + packet[29];
    data[1] = (packet[32] << 8) + packet[33];
    data[2] = (packet[36] << 8) + packet[37];
    return 0;
}
uint8_t MPU6050::dmpGetAccel(VectorInt16 *v, const uint8_t* packet)
{
    // TODO: accommodate different arrangements of sent data (ONLY default supported now)
    if (packet == 0) packet = dmpPacketBuffer;
    v -> x = (packet[28] << 8) + packet[29];
    v -> y = (packet[32] << 8) + packet[33];
    v -> z = (packet[36] << 8) + packet[37];
    return 0;
}
uint8_t MPU6050::dmpGetQuaternion(int32_t *data, const uint8_t* packet)
{
    // TODO: accommodate different arrangements of sent data (ONLY default supported now)
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = ((packet[0] << 24) + (packet[1] << 16) + (packet[2] << 8) + packet[3]);
    data[1] = ((packet[4] << 24) + (packet[5] << 16) + (packet[6] << 8) + packet[7]);
    data[2] = ((packet[8] << 24) + (packet[9] << 16) + (packet[10] << 8) + packet[11]);
    data[3] = ((packet[12] << 24) + (packet[13] << 16) + (packet[14] << 8) + packet[15]);
    return 0;
}
uint8_t MPU6050::dmpGetQuaternion(int16_t *data, const uint8_t* packet)
{
    // TODO: accommodate different arrangements of sent data (ONLY default supported now)
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = ((packet[0] << 8) + packet[1]);
    data[1] = ((packet[4] << 8) + packet[5]);
    data[2] = ((packet[8] << 8) + packet[9]);
    data[3] = ((packet[12] << 8) + packet[13]);
    return 0;
}
uint8_t MPU6050::dmpGetQuaternion(Quaternion *q, const uint8_t* packet)
{
    // TODO: accommodate different arrangements of sent data (ONLY default supported now)
    int16_t qI[4];
    uint8_t status = dmpGetQuaternion(qI, packet);
    if (status == 0) {
        q -> w = (float)qI[0] / 16384.0f;
        q -> x = (float)qI[1] / 16384.0f;
        q -> y = (float)qI[2] / 16384.0f;
        q -> z = (float)qI[3] / 16384.0f;
        return 0;
    }
    return status; // int16 return value, indicates error if this line is reached
}
// uint8_t MPU6050::dmpGet6AxisQuaternion(long *data, const uint8_t* packet);
// uint8_t MPU6050::dmpGetRelativeQuaternion(long *data, const uint8_t* packet);
uint8_t MPU6050::dmpGetGyro(int32_t *data, const uint8_t* packet)
{
    // TODO: accommodate different arrangements of sent data (ONLY default supported now)
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = ((packet[16] << 24) + (packet[17] << 16) + (packet[18] << 8) + packet[19]);
    data[1] = ((packet[20] << 24) + (packet[21] << 16) + (packet[22] << 8) + packet[23]);
    data[2] = ((packet[24] << 24) + (packet[25] << 16) + (packet[26] << 8) + packet[27]);
    return 0;
}
uint8_t MPU6050::dmpGetGyro(int16_t *data, const uint8_t* packet)
{
    // TODO: accommodate different arrangements of sent data (ONLY default supported now)
    if (packet == 0) packet = dmpPacketBuffer;
    data[0] = (packet[16] << 8) + packet[17];
    data[1] = (packet[20] << 8) + packet[21];
    data[2] = (packet[24] << 8) + packet[25];
    return 0;
}
// uint8_t MPU6050::dmpSetLinearAccelFilterCoefficient(float coef);
// uint8_t MPU6050::dmpGetLinearAccel(long *data, const uint8_t* packet);
uint8_t MPU6050::dmpGetLinearAccel(VectorInt16 *v, VectorInt16 *vRaw, VectorFloat *gravity)
{
    // get rid of the gravity component (+1g = +8192 in standard DMP FIFO packet, sensitivity is 2g)
    v -> x = vRaw -> x - gravity -> x*8192;
    v -> y = vRaw -> y - gravity -> y*8192;
    v -> z = vRaw -> z - gravity -> z*8192;
    return 0;
}
// uint8_t MPU6050::dmpGetLinearAccelInWorld(long *data, const uint8_t* packet);
uint8_t MPU6050::dmpGetLinearAccelInWorld(VectorInt16 *v, VectorInt16 *vReal, Quaternion *q)
{
    // rotate measured 3D acceleration vector into original state
    // frame of reference based on orientation quaternion
    memcpy(v, vReal, sizeof(VectorInt16));
    v -> rotate(q);
    return 0;
}
// uint8_t MPU6050::dmpGetGyroAndAccelSensor(long *data, const uint8_t* packet);
// uint8_t MPU6050::dmpGetGyroSensor(long *data, const uint8_t* packet);
// uint8_t MPU6050::dmpGetControlData(long *data, const uint8_t* packet);
// uint8_t MPU6050::dmpGetTemperature(long *data, const uint8_t* packet);
// uint8_t MPU6050::dmpGetGravity(long *data, const uint8_t* packet);
uint8_t MPU6050::dmpGetGravity(VectorFloat *v, Quaternion *q)
{
    v -> x = 2 * (q -> x*q -> z - q -> w*q -> y);
    v -> y = 2 * (q -> w*q -> x + q -> y*q -> z);
    v -> z = q -> w*q -> w - q -> x*q -> x - q -> y*q -> y + q -> z*q -> z;
    return 0;
}
// uint8_t MPU6050::dmpGetUnquantizedAccel(long *data, const uint8_t* packet);
// uint8_t MPU6050::dmpGetQuantizedAccel(long *data, const uint8_t* packet);
// uint8_t MPU6050::dmpGetExternalSensorData(long *data, int size, const uint8_t* packet);
// uint8_t MPU6050::dmpGetEIS(long *data, const uint8_t* packet);
 
uint8_t MPU6050::dmpGetEuler(float *data, Quaternion *q)
{
    data[0] = atan2(2*q -> x*q -> y - 2*q -> w*q -> z, 2*q -> w*q -> w + 2*q -> x*q -> x - 1);   // psi
    data[1] = -asin(2*q -> x*q -> z + 2*q -> w*q -> y);                              // theta
    data[2] = atan2(2*q -> y*q -> z - 2*q -> w*q -> x, 2*q -> w*q -> w + 2*q -> z*q -> z - 1);   // phi
    return 0;
}
 
uint8_t MPU6050::dmpGetYawPitchRoll(float *data, Quaternion *q, VectorFloat *gravity)
{
    // yaw: (about Z axis)
    data[0] = atan2(2*q -> x*q -> y - 2*q -> w*q -> z, 2*q -> w*q -> w + 2*q -> x*q -> x - 1);
    // pitch: (nose up/down, about Y axis)
    data[1] = atan(gravity -> x / sqrt(gravity -> y*gravity -> y + gravity -> z*gravity -> z));
    // roll: (tilt left/right, about X axis)
    data[2] = atan(gravity -> y / sqrt(gravity -> x*gravity -> x + gravity -> z*gravity -> z));
    return 0;
}
 
// uint8_t MPU6050::dmpGetAccelFloat(float *data, const uint8_t* packet);
// uint8_t MPU6050::dmpGetQuaternionFloat(float *data, const uint8_t* packet);
 
uint8_t MPU6050::dmpProcessFIFOPacket(const unsigned char *dmpData)
{
    /*for (uint8_t k = 0; k < dmpPacketSize; k++) {
        if (dmpData[k] < 0x10) Serial.print("0");
        Serial.print(dmpData[k], HEX);
        Serial.print(" ");
    }
    Serial.print("\n");*/
    //Serial.println((uint16_t)dmpPacketBuffer);
    return 0;
}
uint8_t MPU6050::dmpReadAndProcessFIFOPacket(uint8_t numPackets, uint8_t *processed)
{
    uint8_t status;
    uint8_t buf[dmpPacketSize];
    for (uint8_t i = 0; i < numPackets; i++) {
        // read packet from FIFO
        getFIFOBytes(buf, dmpPacketSize);
 
        // process packet
        if ((status = dmpProcessFIFOPacket(buf)) > 0) return status;
 
        // increment external process count variable, if supplied
        if (processed != 0) *processed++;
    }
    return 0;
}
 
// uint8_t MPU6050::dmpSetFIFOProcessedCallback(void (*func) (void));
 
// uint8_t MPU6050::dmpInitFIFOParam();
// uint8_t MPU6050::dmpCloseFIFO();
// uint8_t MPU6050::dmpSetGyroDataSource(uint_fast8_t source);
// uint8_t MPU6050::dmpDecodeQuantizedAccel();
// uint32_t MPU6050::dmpGetGyroSumOfSquare();
// uint32_t MPU6050::dmpGetAccelSumOfSquare();
// void MPU6050::dmpOverrideQuaternion(long *q);
uint16_t MPU6050::dmpGetFIFOPacketSize()
{
    return dmpPacketSize;
}