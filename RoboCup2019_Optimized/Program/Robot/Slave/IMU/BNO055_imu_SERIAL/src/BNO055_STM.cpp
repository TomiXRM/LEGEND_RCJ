#include "mbed.h"
#include "BNO055.h"

#define map(x,in_min,in_max,out_min,out_max)  ((x-in_min)*(out_max-out_min)/(in_max - in_min) + out_min)

Serial pc(USBTX,USBRX);
Serial Out(PA_9,PA_10);
DigitalOut LED(D13);
DigitalIn sw(D10);
// InterruptIn init(A3);
// AnalogOut Analog(A3);//DAC
I2C i2c(I2C_SDA, I2C_SCL);
BNO055 BNO(I2C_SDA,I2C_SCL);
short compussdeg,flontdeg;
int16_t degree;
uint8_t velocity;

Ticker Output;




int accel_X,accel_Y,accel_R;
int old_accel_X,old_accel_Y,old_accel_R;

float time_,time_old;

float filterCoefficient_X = 0.7;
float filterCoefficient_Y = 0.7;
float filterCoefficient_R = 0.7;
float highpassValue_X,highpassValue_Y,highpassValue_R;
float lowpassValue_X,lowpassValue_Y,lowpassValue_R;

// 時間差分
float timeSpan = 0.1;
// ひとつ前の加速度
float oldAccel_X,oldAccel_Y,oldAccel_R = 0;
//　加速度から算出した速度
float speed_X,speed_Y,speed_R = 0;
// ひとつ前の速度
float oldspeed_X,oldspeed_Y,oldspeed_R = 0;
// 速度から算出した変位
float difference_X,difference_Y,difference_R = 0;

uint8_t accel_count;
float acceldata_X[10],acceldata_Y[10],acceldata_R[10];


uint8_t sys,gyro,accel,mag = 0;




Timer integral_timer;


void write(){
        Out.putc('I');
        Out.putc(degree % 256);
        Out.putc(degree / 256);
        Out.putc(velocity);
        // Out.printf("Hello");
}

void Notice(){
        pc.printf("Rise!!\r\n");
}

int main() {
        pc.baud(230400);
        Out.baud(230400);
        Output.attach_us(&write,1000);
        // init.rise(&write);
        BNO.reset();
        BNO.setmode(OPERATION_MODE_IMUPLUS);
        sw.mode(PullUp);
        integral_timer.start();
        // Analog.write(0.5);
        for (char i = 0; i < 5; i++) {
                LED = !LED;
                wait_ms(100);
        }
        // while(mag != 3) {
        //         BNO.get_calib();
        //         sys = (BNO.calib >> 6) & 0x03;
        //         gyro = (BNO.calib >> 4) & 0x03;
        //         accel = (BNO.calib >> 2) & 0x03;
        //         mag = BNO.calib & 0x03;
        //         pc.printf("%d\t%d\t%d\t%d\t%d\n",BNO.calib,sys,gyro,accel,mag);
        // }
        for (uint8_t i = 0; i < 6; i++) {
                LED = !LED;
                wait_ms(220);
        }
        flontdeg = (short)(BNO.euler.yaw);

        while(1) {
                if(sw == 0) {
                        flontdeg = (short)(BNO.euler.yaw);
                }
                BNO.get_angles();
                compussdeg = ((short)(BNO.euler.yaw)-flontdeg);
                if(compussdeg<0) {
                        compussdeg+=360;
                }
                /*
                   if (compussdeg > 180) {
                        compussdeg -= 360;
                   } else if (compussdeg < -179) {
                        compussdeg += 360;
                   }*/
                //compussdeg = -compussdeg;

                if(abs(compussdeg) < 5) {
                        LED = 1;
                        compussdeg = 0;
                }else{
                        LED=0;
                }
                float an = map((int)(compussdeg),-180,180,0,100);
                // Analog.write((float)(an/100));










                BNO.get_accel();
                accel_X = (int)(BNO.accel.x * 100);
                accel_Y = (int)(BNO.accel.y * 100);
                accel_R = sqrt(accel_X*accel_X + accel_Y*accel_Y);
cal:
                // time_ = integral_timer.read_ms();
                // timeSpan = time_- time_old;
                // time_old = time_;
                timeSpan = 0.1;
                if(accel_count < 5) {
                        acceldata_X[accel_count] = accel_X;//代入X
                        acceldata_Y[accel_count] = accel_Y;//代入X
                        acceldata_R[accel_count] = accel_R;//代入X

                        lowpassValue_X = lowpassValue_X * filterCoefficient_X + acceldata_X[accel_count] * (1 - filterCoefficient_X);
                        lowpassValue_Y = lowpassValue_Y * filterCoefficient_Y + acceldata_Y[accel_count] * (1 - filterCoefficient_Y);
                        lowpassValue_R = lowpassValue_R * filterCoefficient_R + acceldata_R[accel_count] * (1 - filterCoefficient_R);
                        // ハイパスフィルター(センサの値 - ローパスフィルターの値)
                        highpassValue_X = acceldata_X[accel_count] - lowpassValue_X;
                        highpassValue_Y = acceldata_Y[accel_count] - lowpassValue_Y;
                        highpassValue_R = acceldata_R[accel_count] - lowpassValue_R;

                        // 速度計算(加速度を台形積分する)
                        speed_X = ((highpassValue_X + oldAccel_X) * timeSpan) / 2 + speed_X;
                        speed_Y = ((highpassValue_Y + oldAccel_Y) * timeSpan) / 2 + speed_Y;
                        speed_R = ((highpassValue_R + oldAccel_R) * timeSpan) / 2 + speed_R;
                        oldAccel_X = highpassValue_X;
                        oldAccel_Y = highpassValue_Y;
                        oldAccel_R = highpassValue_R;

                        // 変位計算(速度を台形積分する)
                        difference_X = ((speed_X + oldspeed_X) * timeSpan) / 2 + difference_X;
                        difference_Y = ((speed_Y + oldspeed_Y) * timeSpan) / 2 + difference_Y;
                        difference_R = ((speed_R + oldspeed_R) * timeSpan) / 2 + difference_R;
                        oldspeed_X = speed_X;
                        oldspeed_Y = speed_Y;
                        oldspeed_R = speed_R;

                        accel_count++;



                }else{
                        accel_count = 0;
                        goto cal;
                }
                speed_R = sqrt(speed_X*speed_X+speed_Y*speed_Y);


                if(speed_R < 7) {
                        pc.printf("degree:%d\tStop!!Speed:%d\n",compussdeg,(int)(speed_R));
                }else{
                        pc.printf("degree:%d\tMoving!!Speed:%d\n",compussdeg,(int)(speed_R));
                }

                degree = (int16_t)compussdeg;
                velocity =(uint8_t)(speed_R);
                // write();
        }
}
