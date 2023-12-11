/* Demo code for MPU6050 DMP
 * I thank Ian Hua.
 * Copyright (c) 2015 Match
 *
 * THE PROGRAM IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE PROGRAM OR THE USE OR OTHER DEALINGS IN
 * THE PROGRAM.
 */


// Define Necessary.
//#define OUTPUT_QUATERNION
#define OUTPUT_EULER
//#define OUTPUT_ROLL_PITCH_YAW
//#define OUTPUT_FOR_TEAPOT
//#define OUTPUT_TEMPERATURE


#include "MPU6050_6Axis_MotionApps20.h"
#include "mbed.h"
#include "config.h"
#include <stdio.h>


#define DEG_TO_RAD(x) ( x * 0.01745329 )
#define RAD_TO_DEG(x) ( x * 57.29578 )


RawSerial pc(USBTX, USBRX);
MPU6050 mpu(I2C_SDA, I2C_SCL);     // sda, scl pin
InterruptIn INT0(D12);     // INT0 pin

const int FIFO_BUFFER_SIZE = 128;
uint8_t fifoBuffer[FIFO_BUFFER_SIZE];
uint16_t fifoCount;
uint16_t packetSize;
bool dmpReady;
uint8_t mpuIntStatus;
const int snprintf_buffer_size = 100;
char snprintf_buffer[snprintf_buffer_size];
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

struct Offset {
        int16_t ax, ay, az;
        int16_t gx, gy, gz;
} offset = {150, -350, 1000, -110, 5, 0};    // Measured values

struct MPU6050_DmpData {
        Quaternion q;
        VectorFloat gravity; // g
        float roll, pitch, yaw; // rad
} dmpData;

bool Init();
void dmpDataUpdate();


int main() {
        MBED_ASSERT(Init() == true);

        while(1) {
        }
}


bool Init() {
        pc.baud(115200);

        INT0.mode(PullDown);
        INT0.fall(dmpDataUpdate);

        mpu.initialize();
        if (mpu.testConnection()) {
                pc.puts("MPU6050 test connection passed.\n");
        } else {
                pc.puts("MPU6050 test connection failed.\n");
                return false;
        }
        if (mpu.dmpInitialize() == 0) {
                pc.puts("succeed in MPU6050 DMP Initializing.\n");
        } else {
                pc.puts("failed in MPU6050 DMP Initializing.\n");
                return false;
        }
        mpu.setXAccelOffset(offset.ax);
        mpu.setYAccelOffset(offset.ay);
        mpu.setZAccelOffset(offset.az);
        mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
        mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
        mpu.setXGyroOffsetUser(offset.gx);
        mpu.setYGyroOffsetUser(offset.gy);
        mpu.setZGyroOffsetUser(offset.gz);
        mpu.setDMPEnabled(true); // Enable DMP
        packetSize = mpu.dmpGetFIFOPacketSize();
        dmpReady = true; // Enable interrupt.

        pc.puts("Init finish!\n");

        return true;
}


void dmpDataUpdate() {
        // Check that this interrupt has enabled.
        if (dmpReady == false) return;

        mpuIntStatus = mpu.getIntStatus();
        fifoCount = mpu.getFIFOCount();

        // Check that this interrupt is a FIFO buffer overflow interrupt.
        if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
                mpu.resetFIFO();
                pc.puts("FIFO overflow!\n");
                return;

                // Check that this interrupt is a Data Ready interrupt.
        } else if (mpuIntStatus & 0x02) {
                while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

                mpu.getFIFOBytes(fifoBuffer, packetSize);

    #ifdef OUTPUT_QUATERNION
                mpu.dmpGetQuaternion(&dmpData.q, fifoBuffer);
                if ( snprintf( snprintf_buffer, snprintf_buffer_size, "Quaternion : w=%f, x=%f, y=%f, z=%f\n", dmpData.q.w, dmpData.q.x, dmpData.q.y, dmpData.q.z ) < 0 ) return;
                pc.puts(snprintf_buffer);
    #endif

    #ifdef OUTPUT_EULER
                float euler[3];
                mpu.dmpGetQuaternion(&dmpData.q, fifoBuffer);
                mpu.dmpGetEuler(euler, &dmpData.q);
                if ( snprintf( snprintf_buffer, snprintf_buffer_size, "Euler : psi=%fdeg, theta=%fdeg, phi=%fdeg\n", RAD_TO_DEG(euler[0]), RAD_TO_DEG(euler[1]), RAD_TO_DEG(euler[2]) ) < 0 ) return;
                pc.puts(snprintf_buffer);
    #endif

    #ifdef OUTPUT_ROLL_PITCH_YAW
                mpu.dmpGetQuaternion(&dmpData.q, fifoBuffer);
                mpu.dmpGetGravity(&dmpData.gravity, &dmpData.q);
                float rollPitchYaw[3];
                mpu.dmpGetYawPitchRoll(rollPitchYaw, &dmpData.q, &dmpData.gravity);
                dmpData.roll = rollPitchYaw[2];
                dmpData.pitch = rollPitchYaw[1];
                dmpData.yaw = rollPitchYaw[0];

                if ( snprintf( snprintf_buffer, snprintf_buffer_size, "Roll:%6.2fdeg, Pitch:%6.2fdeg, Yaw:%6.2fdeg\n", RAD_TO_DEG(dmpData.roll), RAD_TO_DEG(dmpData.pitch), RAD_TO_DEG(dmpData.yaw) ) < 0 ) return;
                pc.puts(snprintf_buffer);
    #endif

    #ifdef OUTPUT_FOR_TEAPOT
                teapotPacket[2] = fifoBuffer[0];
                teapotPacket[3] = fifoBuffer[1];
                teapotPacket[4] = fifoBuffer[4];
                teapotPacket[5] = fifoBuffer[5];
                teapotPacket[6] = fifoBuffer[8];
                teapotPacket[7] = fifoBuffer[9];
                teapotPacket[8] = fifoBuffer[12];
                teapotPacket[9] = fifoBuffer[13];
                for (uint8_t i = 0; i < 14; i++) {
                        pc.putc(teapotPacket[i]);
                }
    #endif

    #ifdef OUTPUT_TEMPERATURE
                float temp = mpu.getTemperature() / 340.0 + 36.53;
                if ( snprintf( snprintf_buffer, snprintf_buffer_size, "Temp:%4.1fdeg\n", temp ) < 0 ) return;
                pc.puts(snprintf_buffer);
    #endif

                pc.puts("\n");
        }
}
