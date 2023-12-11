/**
 * @author Alexander Entinger, MSc / LXRobotics
 * @brief this class acts as interface for accessing the adns-9800 sensor - based on https://github.com/mrjohnk/ADNS-9800
 * @file adns_9800.cpp
 */
 
#include "adns_9800.h"
#include "adns_9800_regs.h"
#include "adns_9800_srom_a4.h"
 
/**
 * @brief Constructor
 */
adns_9800::adns_9800(PinName mosi, PinName miso, PinName sclk, PinName ncs) : m_spi(mosi, miso, sclk), m_ncs_pin(ncs)
{
    m_spi.format(8, 3); // 8 bits with mode 3 =>Polarity = 1, Phase = 1
    m_spi.frequency(1000000); // 1 MHz
    
    startup();
}
    
/**
 * @brief Destructor
 */
adns_9800::~adns_9800()
{
    
}

/**
 * @brief returns true if a motion has occured since the last readout
 */
bool adns_9800::new_motion_data_available()
{
    uint8_t const motion_reg = read_reg(REG_Motion);
    bool const new_data_available = (motion_reg & 0x80) > 0;
    return new_data_available;
}
    
/**
 * @brief retrieves the latest delta values
 */
void adns_9800::get_delta_x_y(int16_t &delta_x, int16_t &delta_y)
{
    uint16_t delta_x_l = (uint16_t)(read_reg(REG_Delta_X_L));
    uint16_t delta_x_h = (uint16_t)(read_reg(REG_Delta_X_H)) << 8;
    delta_x = (int16_t)(delta_x_h | delta_x_l);
    
    uint16_t delta_y_l = (uint16_t)(read_reg(REG_Delta_Y_L));
    uint16_t delta_y_h = (uint16_t)(read_reg(REG_Delta_Y_H)) << 8;
    delta_y = (int16_t)(delta_y_h | delta_y_l);
}

/**
 * @brief start and stop communication with the sensor by clearing/setting the ncs pin
 */
void adns_9800::com_begin()
{
    m_ncs_pin = 0;
}
void adns_9800::com_end()
{
    m_ncs_pin = 1;
}

/**
 * @brief provide read/write access to a adns register
 */
uint8_t adns_9800::read_reg(uint8_t const address)
{
  com_begin();
  
  // send adress of the register, with MSBit = 0 to indicate it's a read
  m_spi.write(address & 0x7f );
  wait_us(100); // tSRAD
  // read data
  uint8_t data = m_spi.write(0);
  
  wait_us(1); // tSCLK-NCS for read operation is 120ns
  com_end();
  wait_us(19); //  tSRW/tSRR (=20us) minus tSCLK-NCS

  return data; 
}
void adns_9800::write_reg(uint8_t const address, uint8_t const data)
{
  com_begin();
  
  //send adress of the register, with MSBit = 1 to indicate it's a write
  m_spi.write(address | 0x80 );
  //sent data
  m_spi.write(data);
  
  wait_us(20); // tSCLK-NCS for write operation
  com_end();
  wait_us(100); // tSWW/tSWR (=120us) minus tSCLK-NCS. Could be shortened, but is looks like a safe lower bound 
}

/**
 * @brief upload the firmware
 */
void adns_9800::upload_firmware()
{
  // set the configuration_IV register in 3k firmware mode
  write_reg(REG_Configuration_IV, 0x02); // bit 1 = 1 for 3k mode, other bits are reserved 
  
  // write 0x1d in SROM_enable reg for initializing
  write_reg(REG_SROM_Enable, 0x1d); 
  
  // wait for more than one frame period
  wait_ms(10); // assume that the frame rate is as low as 100fps... even if it should never be that low
  
  // write 0x18 to SROM_enable to start SROM download
  write_reg(REG_SROM_Enable, 0x18); 
  
  // write the SROM file (=firmware data) 
  com_begin();
  m_spi.write(REG_SROM_Load_Burst | 0x80); // write burst destination adress
  wait_us(15);
  
  // send all bytes of the firmware
  for(int i = 0; i < firmware_length; i++)
  { 
    m_spi.write(firmware_data[i]);
    wait_us(15);
  }
  
  com_end(); 
}

/**
 * @brief starts the sensor up
 */
void adns_9800::startup()
{
  com_end(); // ensure that the serial port is reset
  com_begin(); // ensure that the serial port is reset
  com_end(); // ensure that the serial port is reset
  write_reg(REG_Power_Up_Reset, 0x5a); // force reset
  wait_ms(50); // wait for it to reboot
  // read registers 0x02 to 0x06 (and discard the data)
  read_reg(REG_Motion);
  read_reg(REG_Delta_X_L);
  read_reg(REG_Delta_X_H);
  read_reg(REG_Delta_Y_L);
  read_reg(REG_Delta_Y_H);
  // upload the firmware
  upload_firmware();
  wait_ms(10);
  //enable laser(bit 0 = 0b), in normal mode (bits 3,2,1 = 000b)
  // reading the actual value of the register is important because the real
  // default value is different from what is said in the datasheet, and if you
  // change the reserved bytes (like by writing 0x00...) it would not work.
  uint8_t laser_ctrl0 = read_reg(REG_LASER_CTRL0);
  write_reg(REG_LASER_CTRL0, laser_ctrl0 & 0xf0 );
  
  wait_ms(1);
}
