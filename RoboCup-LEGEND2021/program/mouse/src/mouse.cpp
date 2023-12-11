#include "mbed.h"

#define REG_Product_ID                           0x00
#define REG_Revision_ID                          0x01
#define REG_Motion                               0x02
#define REG_Delta_X_L                            0x03
#define REG_Delta_X_H                            0x04
#define REG_Delta_Y_L                            0x05
#define REG_Delta_Y_H                            0x06
#define REG_SQUAL                                0x07
#define REG_Pixel_Sum                            0x08
#define REG_Maximum_Pixel                        0x09
#define REG_Minimum_Pixel                        0x0a
#define REG_Shutter_Lower                        0x0b
#define REG_Shutter_Upper                        0x0c
#define REG_Frame_Period_Lower                   0x0d
#define REG_Frame_Period_Upper                   0x0e
#define REG_Configuration_I                      0x0f
#define REG_Configuration_II                     0x10
#define REG_Frame_Capture                        0x12
#define REG_SROM_Enable                          0x13
#define REG_Run_Downshift                        0x14
#define REG_Rest1_Rate                           0x15
#define REG_Rest1_Downshift                      0x16
#define REG_Rest2_Rate                           0x17
#define REG_Rest2_Downshift                      0x18
#define REG_Rest3_Rate                           0x19
#define REG_Frame_Period_Max_Bound_Lower         0x1a
#define REG_Frame_Period_Max_Bound_Upper         0x1b
#define REG_Frame_Period_Min_Bound_Lower         0x1c
#define REG_Frame_Period_Min_Bound_Upper         0x1d
#define REG_Shutter_Max_Bound_Lower              0x1e
#define REG_Shutter_Max_Bound_Upper              0x1f
#define REG_LASER_CTRL0                          0x20
#define REG_Observation                          0x24
#define REG_Data_Out_Lower                       0x25
#define REG_Data_Out_Upper                       0x26
#define REG_SROM_ID                              0x2a
#define REG_Lift_Detection_Thr                   0x2e
#define REG_Configuration_V                      0x2f
#define REG_Configuration_IV                     0x39
#define REG_Power_Up_Reset                       0x3a
#define REG_Shutdown                             0x3b
#define REG_Inverse_Product_ID                   0x3f
#define REG_Motion_Burst                         0x50
#define REG_SROM_Load_Burst                      0x62
#define REG_Pixel_Burst                          0x64

SPI spi(A6,A5,A4);
RawSerial pc(USBTX,USBRX);
DigitalOut ncs(A3);

Ticker mouse;//マウス読み取りのタイマー割り込み

char initComplete = 0;
char testctr = 0;
volatile int xydat[2];

char MoveTO[2];//1ミリ秒ごとの速度(送信用)
int MoveTO0[2];//1ミリ秒ごとの速度
char Moving0[2];//積算移動距離(送信用)
int Moving[2];//積算移動距離：移動距離を加算し、値を送信したらクリアされる
int MoveX = 0;
int MoveY = 0;

//become able to use constrain in mbed
template<class T>
const T& constrain(const T& x, const T& a, const T& b) {
        if(x < a) return a;
        else if(b < x) return b;
        else return x;
}

void adns_com_begin() {
        ncs = 0;
}

void adns_com_end() {
        ncs = 1;
}

char adns_read_reg(char reg_addr) {
        adns_com_begin();

        // send adress of the register, with MSBit = 0 to indicate it's a read
        spi.write(reg_addr & 0x7f );
        wait_us(100); // tSRAD
        // read data
        char data = spi.write(0);
        wait_us(1); // tSCLK-NCS for read operation is 120ns

        adns_com_end();
        wait_us(19); //  tSRW/tSRR (=20us) minus tSCLK-NCS

        return data;
}

char adns_write_reg(char reg_addr, char data) {
        adns_com_begin();

        //send adress of the register, with MSBit = 1 to indicate it's a write
        spi.write(reg_addr | 0x80 );
        //sent data
        spi.write(data);
        wait_us(20); // tSCLK-NCS for write operation

        adns_com_end();
        wait_us(100); // tSWW/tSWR (=120us) minus tSCLK-NCS. Could be shortened, but is looks like a safe lower bound
}

void performStartup(void) {
        adns_com_end(); // ensure that the serial port is reset
        adns_com_begin(); // ensure that the serial port is reset
        adns_com_end(); // ensure that the serial port is reset
        adns_write_reg(REG_Power_Up_Reset, 0x5a); // force reset
        wait_ms(50); // wait for it to reboot
        // read registers 0x02 to 0x06 (and discard the data)
        adns_read_reg(REG_Motion);
        adns_read_reg(REG_Delta_X_L);
        adns_read_reg(REG_Delta_X_H);
        adns_read_reg(REG_Delta_Y_L);
        adns_read_reg(REG_Delta_Y_H);
        // upload the firmwareするときはコメントアウトを解除
        // adns_upload_firmware();
        wait_ms(10);
        /*enable laser(bit 0 = 0b), in normal mode (bits 3,2,1 = 000b)
           reading the actual value of the register is important because the real
           default value is different from what is said in the datasheet, and if you
           change the reserved chars (like by writing 0x00...) it would not work.*/
        char laser_ctrl0 = adns_read_reg(REG_LASER_CTRL0);
        adns_write_reg(REG_LASER_CTRL0, laser_ctrl0 & 0xf0 );

        wait_ms(1);
}

void UpdatePointer(void) {
        if (initComplete == 9) {
                ncs = 0;
                xydat[0] = (int)adns_read_reg(REG_Delta_X_L);
                xydat[1] = (int)adns_read_reg(REG_Delta_Y_L);
                ncs = 1;
        }
}

int convTwosComp(int b) {
        //Convert from 2's complement
        if (b & 0x80) {
                b = -1 * ((b ^ 0xff) + 1);
        }
        return b;
}

void readmouse(){//1ミリ秒ごとに実行される
        UpdatePointer();//前回のUpdatePointerからの移動距離を取得
        MoveY = constrain(convTwosComp(xydat[0]), -99, 99);//Y軸の移動距離を取得
        MoveX = constrain(convTwosComp(xydat[1]), -99, 99);//X軸の移動距離を取得
        MoveTO0[0] = (MoveY) *0.5;//±50のY軸の1ミリ秒ごとの移動速度
        MoveTO0[1] = (MoveX) *0.5;//±50のX軸の1ミリ秒ごとの移動速度
        Moving0[0] = Moving[0]+100;//正の範囲にずらした送信用移動距離(Y)
        Moving0[1] = Moving[1]+100;//正の範囲にずらした送信用移動距離(X)
        pc.printf("mouseX:%d\tmouseY%d\r\n",MoveX,MoveY);
}

int main() {
        pc.baud(230400);
        spi.format(8, 3);
        spi.frequency(1000000);

        MoveTO[0] = 0;
        MoveTO[1] = 0;

        //マウスの初期設定
        performStartup();
        wait_ms(5000);
        initComplete = 9;

        mouse.attach_us(&readmouse, 1000);
        //1ミリ秒ごとに移動距離を取得

        while(1) {
                wait_ms(1);
                pc.printf("mouseX:%d\tmouseY%d\r\n",MoveX,MoveY);
                Moving[0]=0;//送信したら値をリセット
                Moving[1]=0;
                MoveTO[0]=MoveTO0[0]+50;//正の範囲にずらした送信用移動距離(Y)
                MoveTO[1]=MoveTO0[1]+50;//正の範囲にずらした送信用移動距離(X)
                wait_ms(1);
        }

}