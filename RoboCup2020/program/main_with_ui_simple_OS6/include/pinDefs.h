#pragma once
//LED
#define LED1 PB_8
#define LED2 PB_9
#define LED3 PF_13

//LINE Sensor
#define LINE_SIG PA_0
#define LINE_A0 PG_2
#define LINE_A1 PG_3
#define LINE_A2 PG_4
#define LINE_A3 PG_5
#define LINE_A4 PG_6
#define LINE_PWM PD_15

//Button
#define BUTTON_LEFT PD_7
#define BUTTON_CENTRE PG_9
#define BUTTON_RIGHT PG_10

//Buzzer
#define BUZZER PC_6

//Dribbler
#define DRIB_PWM PE_5

//Kicker
#define KICKER_PHOTO1 PA_2
#define KICKER_PHOTO2 PA_3
#define KICKER_KICK PF_2

//EEPROM
#define EEPROM_SDA PF_0
#define EEPROM_SCL PF_1

//INA219
#define INA219_SDA PC_9
#define INA219_SCL PA_8

//OLED
#define LCD_SDA PB_7
#define LCD_SCL PB_6

//I2C (FREE)
#define I2C_SDA PF_15
#define I2C_SCL PF_14

//CN (ADC)
#define CN1 PF_3
#define CN2 PF_4
#define CN3 PF_5
#define CN4 PF_6
#define CN5 PF_7
#define CN6 PF_8
#define CN7 PF_9
#define CN8 PF_10
#define CN9 PC_0
#define CN10 PC_1
#define CN11 PC_2

//Bridge
#define BRIDGE_TX PA_9
#define BRIDGE_RX PA_10
#define BRIDGE_IO1 PA_4
#define BRIDGE_IO2 PA_5

//UART
#define UART_A_TX PG_14
#define UART_A_RX PC_7

#define UART_B_TX PE_8
#define UART_B_RX PE_7

#define UART_C_TX PE_1
#define UART_C_RX PE_0

//IMU
#define IMU_TX PC_10
#define IMU_RX PC_11

//MOUSE
#define MOUSE_TX PC_12
#define MOUSE_RX PD_2

//CAM
#define CAM_TX PD_5
#define CAM_RX PD_6

//PC&IR
#define PC_TX PD_8
#define IR_RX PD_9
#define IR_INT PE_15

//RGB
#define RGB_R PD_14
#define RGB_G PD_13
#define RGB_B PD_12

//DIPSW
#define DIPSW1 PF_11
#define DIPSW2 PF_12
#define DIPSW3 PG_0
#define DIPSW4 PG_1

//Motor
#define MD_SLP PE_10
//[A]
#define MD_A_EN1 PA_1
#define MD_A_PH1 PE_9
#define MD_A_EN2 PE_11
#define MD_A_PH2 PA_15

//[B]
#define MD_B_EN1 PE_13
#define MD_B_PH1 PE_14
#define MD_B_EN2 PB_10
#define MD_B_PH2 PB_11

//SPI
#define SPI_SCK PB_3
#define SPI_MISO PB_4
#define SPI_MOSI PB_5
