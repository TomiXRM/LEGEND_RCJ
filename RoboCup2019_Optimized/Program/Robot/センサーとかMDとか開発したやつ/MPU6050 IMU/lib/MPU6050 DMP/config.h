// FIFO rate = 200Hz / (1 + this value)
// For example, 0x01 is 100Hz, 0x03 is 50Hz.
// 0x00 to 0x09
#define IMU_FIFO_RATE_DIVIDER 0x09
 
// Sample rate = 1kHz / (1 + this valye)
// For example, 4 is 200Hz.
#define IMU_SAMPLE_RATE_DIVIDER 4
 
// measuring range of gyroscope (±n deg/s)
// But other value doesn't yet support.
#define MPU6050_GYRO_FS MPU6050_GYRO_FS_2000
 
// measuring range of acceleration sensor (±n g)
// But other value doesn't yet support.
#define MPU6050_ACCEL_FS MPU6050_ACCEL_FS_2
 
#define PC_BAUDRATE 921600
            