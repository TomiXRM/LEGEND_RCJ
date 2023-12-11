#include "MPU6050_DMP6.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file



/* =========================================================================
   NOTE: In addition to connection 3.3v, GND, SDA, and SCL, this sketch
   depends on the MPU-6050's INT pin being connected to the Arduino's
   external interrupt #0 pin. On the Arduino Uno and Mega 2560, this is
   digital I/O pin 2.
 * ========================================================================= */

// uncomment "OUTPUT_READABLE_QUATERNION" if you want to see the actual
// quaternion components in a [w, x, y, z] format (not best for parsing
// on a remote host such as Processing or something though)
//#define OUTPUT_READABLE_QUATERNION

// uncomment "OUTPUT_READABLE_EULER" if you want to see Euler angles
// (in degrees) calculated from the quaternions coming from the FIFO.
// Note that Euler angles suffer from gimbal lock (for more info, see
// http://en.wikipedia.org/wiki/Gimbal_lock)
//#define OUTPUT_READABLE_EULER

// uncomment "OUTPUT_READABLE_YAWPITCHROLL" if you want to see the yaw/
// pitch/roll angles (in degrees) calculated from the quaternions coming
// from the FIFO. Note this also requires gravity vector calculations.
// Also note that yaw/pitch/roll angles suffer from gimbal lock (for
// more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)
#define OUTPUT_READABLE_YAWPITCHROLL

// uncomment "OUTPUT_READABLE_REALACCEL" if you want to see acceleration
// components with gravity removed. This acceleration reference frame is
// not compensated for orientation, so +X is always +X according to the
// sensor, just without the effects of gravity. If you want acceleration
// compensated for orientation, us OUTPUT_READABLE_WORLDACCEL instead.
//#define OUTPUT_READABLE_REALACCEL

// uncomment "OUTPUT_READABLE_WORLDACCEL" if you want to see acceleration
// components with gravity removed and adjusted for the world frame of
// reference (yaw is relative to initial orientation, since no magnetometer
// is present in this case). Could be quite handy in some cases.
//#define OUTPUT_READABLE_WORLDACCEL

// uncomment "OUTPUT_TEAPOT" if you want output that matches the
// format used for the InvenSense teapot demo
//#define OUTPUT_TEAPOT

#ifndef M_PI
#define M_PI 3.14159265358979
#endif

#define map(x,in_min,in_max,out_min,out_max)  ((x-in_min)*(out_max-out_min)/(in_max - in_min) + out_min)

namespace MPU6050DMP6 {

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu;
//MPU6050 mpu(0x69); // <-- use for AD0 high

Serial pc(D1,D0);
Serial SerialDebug(USBTX,NC);
// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
float gyro=0;
// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
int ok=0;
float rem=0;
// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };
int uart=0;
DigitalOut myled(LED3);
int led1=0;
InterruptIn checkpin(PA_12);
//I2C     i2c(PB_7,PB_6);
DigitalIn switch1(D10);
DigitalOut RLED(D13);
DigitalOut WLED(D8);
AnalogOut Analog(A3);//DAC
AnalogIn AIN(A0);
Timer t;
InterruptIn Uart(D9);//メインマイコンの割り込みピン
float FloatData= 0;
long intdat=0,gyro_rem=0;
//pc.baud(115200);
struct {
        char highbyte;
        char lowbyte;
        // short intdat = 0;
} data;

struct {
        int16_t XGy;//getXGyroOffset
        int16_t YGy;//getYGyroOffset
        int16_t ZGy;//getZGyroOffset
        int16_t ZAc;//getZAccelOffset
} calib;

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady()
{
        mpuInterrupt = true;
}



// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================
/*void sendTomaster(){
        myled=1;
        //data.highbyte = intdat%256;//分解
        //data.lowbyte = intdat/256;//分解
        pc.putc('H');//ヘッダをつける
        pc.putc(data.highbyte);
        pc.putc(data.lowbyte);
        myled = 0;
   }*/

void calibration(){
        mpu.setXGyroOffset(0);
        mpu.setYGyroOffset(0);
        mpu.setZGyroOffset(0);
        mpu.setZAccelOffset(0);
        wait(30);
        calib.XGy = mpu.getXGyroOffset();
        calib.YGy = mpu.getYGyroOffset();
        calib.ZGy = mpu.getZGyroOffset();
        calib.ZAc = mpu.getZAccelOffset();
        wait(1);
        SerialDebug.printf("1st  XGY:%d \tYGY:%d \tZGY:%d \tZAC:%d\r\n",calib.XGy,calib.YGy,calib.ZGy,calib.ZAc);
        wait(3);
        calib.XGy = mpu.getXGyroOffset();
        calib.YGy = mpu.getYGyroOffset();
        calib.ZGy = mpu.getZGyroOffset();
        calib.ZAc = mpu.getZAccelOffset();
        wait(2);
        SerialDebug.printf("2nd  XGY:%d \tYGY:%d \tZGY:%d \tZAC:%d\r\n",calib.XGy,calib.YGy,calib.ZGy,calib.ZAc);
}

void SetOffset(int16_t GyX,int16_t GyY,int16_t GyZ,int16_t AcZ){
        mpu.setXGyroOffset(GyX);
        mpu.setYGyroOffset(GyY);
        mpu.setZGyroOffset(GyZ);
        mpu.setZAccelOffset(AcZ);
}


void setup()
{
        //i2c.frequency(400000);
        switch1.mode(PullUp);
        //checkpin.mode(PullUp);
        // initialize arduinoSerial communication
        // (115200 chosen because it is required for Teapot Demo output, but it's
        // really up to you depending on your project)

        pc.baud(115200);
        SerialDebug.baud(115200);
        // initialize device
        // pc.printf(F("Initializing I2C devices..."));
        mpu.initialize();

        // verify connection
        // pc.printf(F("Testing device connections..."));
        //  pc.printf(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

        // wait for ready
        // arduinoSerial.println(F("\nSend any character to begin DMP programming and demo: "));
        // while (arduinoSerial.available() && arduinoSerial.read()); // empty buffer
        // while (!arduinoSerial.available());                 // wait for data
        // while (arduinoSerial.available() && arduinoSerial.read()); // empty buffer again

        // load and configure the DMP
        //  pc.printf("Initializing DMP...");
        devStatus = mpu.dmpInitialize();

        // supply your own gyro offsets here, scaled for min sensitivity
        mpu.setXGyroOffset(63);
        mpu.setYGyroOffset(19);
        mpu.setZGyroOffset(-24);
        mpu.setZAccelOffset(1585);   //*/// 1688 factory default for my test chip
        //SetOffset(0,0,0,756);
        // make sure it worked (returns 0 if so)
        if (devStatus == 0) {
                // turn on the DMP, now that it's ready
                // pc.printf("Enabling DMP...");
                mpu.setDMPEnabled(true);

                // enable Arduino interrupt detection
                //  pc.printf("Enabling interrupt detection (Arduino external interrupt 0)...");
                checkpin.rise(&dmpDataReady);
                //Uart.rise(&sendTomaster);
                mpuIntStatus = mpu.getIntStatus();

                // set our DMP Ready flag so the main loop() function knows it's okay to use it
                //pc.printf("DMP ready! Waiting for first interrupt...");
                dmpReady = true;

                // get expected DMP packet size for later comparison
                packetSize = mpu.dmpGetFIFOPacketSize();
        } else {
                // ERROR!
                // 1 = initial memory load failed
                // 2 = DMP configuration updates failed
                // (if it's going to break, usually the code will be 1)
                SerialDebug.printf("DMP Initialization failed (code ");
                SerialDebug.printf("%d",devStatus);
                SerialDebug.printf(")");
        }
        //NVIC_SetPriority(ADCD2_IRQn,1);
        //NVIC_SetPriority(ADC1_IRQn,2);

}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop()
{
        //I2C i2c(PB_7,PB_6);
        // if programming failed, don't try to do anything
        if (!dmpReady) return;

        // wait for MPU interrupt or extra packet(s) available
        while (!mpuInterrupt && fifoCount < packetSize) {
                // other program behavior stuff here
                // .
                // .
                // .
                // if you are really paranoid you can frequently test in between other
                // stuff to see if mpuInterrupt is true, and if so, "break;" from the
                // while() loop to immediately process the MPU data
                // .
                // .
                // .
                //mpuInterrupt = true;
                //myled=1;
        }
//myled=0;
        // reset interrupt flag and get INT_STATUS byte
        mpuInterrupt = false;
        mpuIntStatus = mpu.getIntStatus();

        // get current FIFO count
        fifoCount = mpu.getFIFOCount();

        // check for overflow (this should never happen unless our code is too inefficient)
        if ((mpuIntStatus & 0x10 || fifoCount == 1024)) {

                // reset so we can continue cleanly
                mpu.resetFIFO();
                //arduinoSerial.println(F("FIFO overflow!"));

                // otherwise, check for DMP data ready interrupt (this should happen frequently)
        } else if (mpuIntStatus & 0x02) {
                // }else{
                // wait for correct available data length, should be a VERY short wait
                // while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

                // read a packet from FIFO
                mpu.getFIFOBytes(fifoBuffer, packetSize);

                // track FIFO count here in case there is > 1 packet available
                // (this lets us immediately read more without waiting for an interrupt)
                fifoCount -= packetSize;
                mpu.dmpGetQuaternion(&q, fifoBuffer);
                mpu.dmpGetGravity(&gravity, &q);
                mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
                //arduinoSerial.print("ypr\t");

                gyro=ypr[0];
                gyro*=180/M_PI;
                if(gyro<0) {
                        gyro+=360;
                }
                ok=switch1;
                if(ok==0) {
                        rem=gyro;
                }else{
                        //myled=0;
                }
                gyro=gyro-rem;
                if(gyro<0) {
                        gyro+=360;
                }

                if (gyro > 180) {
                        gyro -= 360;
                } else if (gyro < -179) {
                        gyro += 360;
                }
                gyro = -gyro;
                //SerialDebug.printf("=%f\r\n",gyro);
                if(abs(gyro) < 5) {
                        RLED = 1;
                }else{
                        RLED=0;
                }



                //gyro*=182;//むくよ
                /*intdat=gyro;
                   data.highbyte = intdat%256;//分解
                   data.lowbyte = intdat/256;//分解*/
                //FloatData = gyro/360;

                if(gyro < -90) gyro = -90;
                if(gyro > 90) gyro = 90;
                float a = map((int)(gyro),-90,90,0,100);

                Analog.write((float)(a/100));
                SerialDebug.printf("%d\t%d\t%d\r\n",(int)gyro,(int)a,(int)(AIN.read()*1000)+16);
                //pc.putc('H');//ヘッダをつける
                //pc.putc(data.highbyte);
                //pc.putc(data.lowbyte);
                //yro;

                //arduinoSerial.println(gyro);
/*#ifdef OUTPUT_READABLE_QUATERNION
        // display quaternion values in easy matrix form: w x y z
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        arduinoSerial.print("quat\t");
        arduinoSerial.print(q.w);
        arduinoSerial.print("\t");
        arduinoSerial.print(q.x);
        arduinoSerial.print("\t");
        arduinoSerial.print(q.y);
        arduinoSerial.print("\t");
        arduinoSerial.println(q.z);
 #endif

 #ifdef OUTPUT_READABLE_EULER
        // display Euler angles in degrees
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetEuler(euler, &q);
        arduinoSerial.print("euler\t");
        arduinoSerial.print(euler[0] * 180/M_PI);
        //arduinoSerial.print("\t");
        //arduinoSerial.print(euler[1] * 180/M_PI);
        //arduinoSerial.print("\t");
        //arduinoSerial.println(euler[2] * 180/M_PI);
 #endif

 #ifdef OUTPUT_READABLE_YAWPITCHROLL
        // display Euler angles in degrees
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        arduinoSerial.print("ypr\t");
        arduinoSerial.print(ypr[0] * 180/M_PI);
        arduinoSerial.print("\t");
        arduinoSerial.print(ypr[1] * 180/M_PI);
        arduinoSerial.print("\t");
        arduinoSerial.println(ypr[2] * 180/M_PI);
 #endif

 #ifdef OUTPUT_READABLE_REALACCEL
        // display real acceleration, adjusted to remove gravity
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetAccel(&aa, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
        arduinoSerial.print("areal\t");
        arduinoSerial.print(aaReal.x);
        arduinoSerial.print("\t");
        arduinoSerial.print(aaReal.y);
        arduinoSerial.print("\t");
        arduinoSerial.println(aaReal.z);
 #endif

 #ifdef OUTPUT_READABLE_WORLDACCEL
        // display initial world-frame acceleration, adjusted to remove gravity
        // and rotated based on known orientation from quaternion
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetAccel(&aa, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
        mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
        arduinoSerial.print("aworld\t");
        arduinoSerial.print(aaWorld.x);
        arduinoSerial.print("\t");
        arduinoSerial.print(aaWorld.y);
        arduinoSerial.print("\t");
        arduinoSerial.println(aaWorld.z);
 #endif

 #ifdef OUTPUT_TEAPOT
        // display quaternion values in InvenSense Teapot demo format:
        teapotPacket[2] = fifoBuffer[0];
        teapotPacket[3] = fifoBuffer[1];
        teapotPacket[4] = fifoBuffer[4];
        teapotPacket[5] = fifoBuffer[5];
        teapotPacket[6] = fifoBuffer[8];
        teapotPacket[7] = fifoBuffer[9];
        teapotPacket[8] = fifoBuffer[12];
        teapotPacket[9] = fifoBuffer[13];
        arduinoSerial.write(teapotPacket, 14);
        teapotPacket[11]++; // packetCount, loops at 0xFF on purpose
 #endif
 */
                // blink LED to indicate activity
                if( led1 == 0 ) {
                        led1 = 0;
                }else{
                        led1 = 1;
                }
        }
}

};
