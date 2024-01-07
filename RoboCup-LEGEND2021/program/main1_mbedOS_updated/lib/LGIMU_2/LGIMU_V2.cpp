#include "LGIMU_V2.h"
#include "mbed.h"

// BNO055::BNO055(PinName SDA, PinName SCL) : _i2c(SDA, SCL) {

BNO055::BNO055(I2C *i2c) {
    // Set I2C fast and bring reset line high
    _i2c = i2c;
    _i2c->frequency(200000);
    address = BNOAddress;
    accel_scale = 0.001f;
    rate_scale = 1.0f / 16.0f;
    angle_scale = 1.0f / 16.0f;
    temp_scale = 1;

    filterCoefficient_X = 0.7;
    filterCoefficient_Y = 0.7;
    filterCoefficient_R = 0.7;
    timeSpan = 0.1;
    oldAccel_X = oldAccel_Y = oldAccel_R = 0;
    speed_X = speed_Y = speed_R = 0;
    oldspeed_X, oldspeed_Y, oldspeed_R = 0;
    difference_X, difference_Y, difference_R = 0;
}

void BNO055::reset() {
    // Perform a power-on-reset
    readchar(BNO055_SYS_TRIGGER_ADDR);
    rx = rx | 0x20;
    writechar(BNO055_SYS_TRIGGER_ADDR, rx);
    // Wait for the system to come back up again (datasheet says 650ms)
    wait_us(675000);
}

bool BNO055::check() {
    // Check we have communication link with the chip
    readchar(BNO055_CHIP_ID_ADDR);
    if (rx != 0xA0)
        return false;
    // Grab the chip ID and software versions
    tx[0] = BNO055_CHIP_ID_ADDR;
    _i2c->write(address, tx, 1, true);
    _i2c->read(address + 1, rawdata, 7, false);
    ID.id = rawdata[0];
    ID.accel = rawdata[1];
    ID.mag = rawdata[2];
    ID.gyro = rawdata[3];
    ID.sw[0] = rawdata[4];
    ID.sw[1] = rawdata[5];
    ID.bootload = rawdata[6];
    setpage(1);
    tx[0] = BNO055_UNIQUE_ID_ADDR;
    _i2c->write(address, tx, 1, true);
    _i2c->read(address + 1, ID.serial, 16, false);
    setpage(0);
    return true;
}

void BNO055::SetExternalCrystal(bool yn) {
    // Read the current status from the device
    readchar(BNO055_SYS_TRIGGER_ADDR);
    if (yn)
        rx = rx | 0x80;
    else
        rx = rx & 0x7F;
    writechar(BNO055_SYS_TRIGGER_ADDR, rx);
}

void BNO055::set_accel_units(char units) {
    readchar(BNO055_UNIT_SEL_ADDR);
    if (units == MPERSPERS) {
        rx = rx & 0xFE;
        accel_scale = 0.01f;
    } else {
        rx = rx | units;
        accel_scale = 0.001f;
    }
    writechar(BNO055_UNIT_SEL_ADDR, rx);
}

void BNO055::set_anglerate_units(char units) {
    readchar(BNO055_UNIT_SEL_ADDR);
    if (units == DEG_PER_SEC) {
        rx = rx & 0xFD;
        rate_scale = 1.0f / 16.0f;
    } else {
        rx = rx | units;
        rate_scale = 1.0f / 900.0f;
    }
    writechar(BNO055_UNIT_SEL_ADDR, rx);
}

void BNO055::set_angle_units(char units) {
    readchar(BNO055_UNIT_SEL_ADDR);
    if (units == DEGREES) {
        rx = rx & 0xFB;
        angle_scale = 1.0f / 16.0f;
    } else {
        rx = rx | units;
        rate_scale = 1.0f / 900.0f;
    }
    writechar(BNO055_UNIT_SEL_ADDR, rx);
}

void BNO055::set_temp_units(char units) {
    readchar(BNO055_UNIT_SEL_ADDR);
    if (units == CENTIGRADE) {
        rx = rx & 0xEF;
        temp_scale = 1;
    } else {
        rx = rx | units;
        temp_scale = 2;
    }
    writechar(BNO055_UNIT_SEL_ADDR, rx);
}

void BNO055::set_orientation(char units) {
    readchar(BNO055_UNIT_SEL_ADDR);
    if (units == WINDOWS)
        rx = rx & 0x7F;
    else
        rx = rx | units;
    writechar(BNO055_UNIT_SEL_ADDR, rx);
}

void BNO055::setmode(char omode) {
    writechar(BNO055_OPR_MODE_ADDR, omode);
    op_mode = omode;
}

void BNO055::setpowermode(char pmode) {
    writechar(BNO055_PWR_MODE_ADDR, pmode);
    pwr_mode = pmode;
}

void BNO055::get_accel(void) {
    tx[0] = BNO055_ACCEL_DATA_X_LSB_ADDR;
    _i2c->write(address, tx, 1, true);
    _i2c->read(address + 1, rawdata, 6, 0);
    accel.rawx = (rawdata[1] << 8 | rawdata[0]);
    accel.rawy = (rawdata[3] << 8 | rawdata[2]);
    accel.rawz = (rawdata[5] << 8 | rawdata[4]);
    accel.x = float(accel.rawx) * accel_scale;
    accel.y = float(accel.rawy) * accel_scale;
    accel.z = float(accel.rawz) * accel_scale;
}

void BNO055::get_gyro(void) {
    tx[0] = BNO055_GYRO_DATA_X_LSB_ADDR;
    _i2c->write(address, tx, 1, true);
    _i2c->read(address + 1, rawdata, 6, 0);
    gyro.rawx = (rawdata[1] << 8 | rawdata[0]);
    gyro.rawy = (rawdata[3] << 8 | rawdata[2]);
    gyro.rawz = (rawdata[5] << 8 | rawdata[4]);
    gyro.x = float(gyro.rawx) * rate_scale;
    gyro.y = float(gyro.rawy) * rate_scale;
    gyro.z = float(gyro.rawz) * rate_scale;
}

void BNO055::get_mag(void) {
    tx[0] = BNO055_MAG_DATA_X_LSB_ADDR;
    _i2c->write(address, tx, 1, true);
    _i2c->read(address + 1, rawdata, 6, 0);
    mag.rawx = (rawdata[1] << 8 | rawdata[0]);
    mag.rawy = (rawdata[3] << 8 | rawdata[2]);
    mag.rawz = (rawdata[5] << 8 | rawdata[4]);
    mag.x = float(mag.rawx);
    mag.y = float(mag.rawy);
    mag.z = float(mag.rawz);
}

void BNO055::get_lia(void) {
    tx[0] = BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR;
    _i2c->write(address, tx, 1, true);
    _i2c->read(address + 1, rawdata, 6, 0);
    lia.rawx = (rawdata[1] << 8 | rawdata[0]);
    lia.rawy = (rawdata[3] << 8 | rawdata[2]);
    lia.rawz = (rawdata[5] << 8 | rawdata[4]);
    lia.x = float(lia.rawx) * accel_scale;
    lia.y = float(lia.rawy) * accel_scale;
    lia.z = float(lia.rawz) * accel_scale;
}

void BNO055::get_grv(void) {
    tx[0] = BNO055_GRAVITY_DATA_X_LSB_ADDR;
    _i2c->write(address, tx, 1, true);
    _i2c->read(address + 1, rawdata, 6, 0);
    gravity.rawx = (rawdata[1] << 8 | rawdata[0]);
    gravity.rawy = (rawdata[3] << 8 | rawdata[2]);
    gravity.rawz = (rawdata[5] << 8 | rawdata[4]);
    gravity.x = float(gravity.rawx) * accel_scale;
    gravity.y = float(gravity.rawy) * accel_scale;
    gravity.z = float(gravity.rawz) * accel_scale;
}

void BNO055::get_quat(void) {
    tx[0] = BNO055_QUATERNION_DATA_W_LSB_ADDR;
    _i2c->write(address, tx, 1, true);
    _i2c->read(address + 1, rawdata, 8, 0);
    quat.raww = (rawdata[1] << 8 | rawdata[0]);
    quat.rawx = (rawdata[3] << 8 | rawdata[2]);
    quat.rawy = (rawdata[5] << 8 | rawdata[4]);
    quat.rawz = (rawdata[7] << 8 | rawdata[6]);
    quat.w = float(quat.raww) / 16384.0f;
    quat.x = float(quat.rawx) / 16384.0f;
    quat.y = float(quat.rawy) / 16384.0f;
    quat.z = float(quat.rawz) / 16384.0f;
}

void BNO055::get_angles(void) {
    tx[0] = BNO055_EULER_H_LSB_ADDR;
    _i2c->write(address, tx, 1, true);
    _i2c->read(address + 1, rawdata, 6, 0);
    euler.rawyaw = (rawdata[1] << 8 | rawdata[0]);
    euler.rawroll = (rawdata[3] << 8 | rawdata[2]);
    euler.rawpitch = (rawdata[5] << 8 | rawdata[4]);
    euler.yaw = float(euler.rawyaw) * angle_scale;
    euler.roll = float(euler.rawroll) * angle_scale;
    euler.pitch = float(euler.rawpitch) * angle_scale;
}

void BNO055::get_temp(void) {
    readchar(BNO055_TEMP_ADDR);
    temperature = rx / temp_scale;
}

void BNO055::get_calib(void) {
    readchar(BNO055_CALIB_STAT_ADDR);
    calib = rx;
}

void BNO055::read_calibration_data(void) {
    char tempmode = op_mode;
    setmode(OPERATION_MODE_CONFIG);
    wait_us(20000);
    tx[0] = ACCEL_OFFSET_X_LSB_ADDR;
    _i2c->write(address, tx, 1, true);
    _i2c->read(address, calibration, 22, false);
    setmode(tempmode);
    wait_us(10000);
}

void BNO055::write_calibration_data(void) {
    char tempmode = op_mode;
    setmode(OPERATION_MODE_CONFIG);
    wait_us(20000);
    tx[0] = ACCEL_OFFSET_X_LSB_ADDR;
    _i2c->write(address, tx, 1, true);
    _i2c->write(address, calibration, 22, false);
    setmode(tempmode);
    wait_us(10000);
}

void BNO055::set_mapping(char orient) {
    switch (orient) {
    case 0:
        writechar(BNO055_AXIS_MAP_CONFIG_ADDR, 0x21);
        writechar(BNO055_AXIS_MAP_SIGN_ADDR, 0x04);
        break;
    case 1:
        writechar(BNO055_AXIS_MAP_CONFIG_ADDR, 0x24);
        writechar(BNO055_AXIS_MAP_SIGN_ADDR, 0x00);
        break;
    case 2:
        writechar(BNO055_AXIS_MAP_CONFIG_ADDR, 0x24);
        writechar(BNO055_AXIS_MAP_SIGN_ADDR, 0x00);
        break;
    case 3:
        writechar(BNO055_AXIS_MAP_CONFIG_ADDR, 0x21);
        writechar(BNO055_AXIS_MAP_SIGN_ADDR, 0x02);
        break;
    case 4:
        writechar(BNO055_AXIS_MAP_CONFIG_ADDR, 0x24);
        writechar(BNO055_AXIS_MAP_SIGN_ADDR, 0x03);
        break;
    case 5:
        writechar(BNO055_AXIS_MAP_CONFIG_ADDR, 0x21);
        writechar(BNO055_AXIS_MAP_SIGN_ADDR, 0x01);
        break;
    case 6:
        writechar(BNO055_AXIS_MAP_CONFIG_ADDR, 0x21);
        writechar(BNO055_AXIS_MAP_SIGN_ADDR, 0x07);
        break;
    case 7:
        writechar(BNO055_AXIS_MAP_CONFIG_ADDR, 0x24);
        writechar(BNO055_AXIS_MAP_SIGN_ADDR, 0x05);
        break;
    default:
        writechar(BNO055_AXIS_MAP_CONFIG_ADDR, 0x24);
        writechar(BNO055_AXIS_MAP_SIGN_ADDR, 0x00);
    }
}

void BNO055::init() {
    reset();
    setmode(OPERATION_MODE_IMUPLUS);
}

int16_t BNO055::getDeg() {
    get_angles();
    deg = (int16_t)(euler.yaw) - front;
    if (deg < 0) deg += 360;
    if (deg > 180) deg -= 360;
    deg = -deg;
    return deg;
}

int8_t BNO055::getVelocity() {
    get_accel();
    accel_X = (int)(accel.x * 100);
    accel_Y = (int)(accel.y * 100);
    accel_R = sqrt(accel_X * accel_X + accel_Y * accel_Y);
cal:
    // time_ = integral_timer.read_ms();
    // timeSpan = time_- time_old;
    // time_old = time_;
    timeSpan = 0.1;
    if (accel_count < 5) {
        acceldata_X[accel_count] = accel_X; // 代入X
        acceldata_Y[accel_count] = accel_Y; // 代入X
        acceldata_R[accel_count] = accel_R; // 代入X

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

    } else {
        accel_count = 0;
        goto cal;
    }
    speed_R = sqrt(speed_X * speed_X + speed_Y * speed_Y);
    return speed_R;
}

void BNO055::setZero() {
    get_angles();
    front = (int16_t)(euler.yaw);
}
