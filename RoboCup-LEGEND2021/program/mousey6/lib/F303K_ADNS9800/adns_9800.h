/**
 * @author Alexander Entinger, MSc / LXRobotics
 * @brief this class acts as interface for accessing the adns-9800 sensor - based on https://github.com/mrjohnk/ADNS-9800
 * @file adns_9800.h
 */
 
#ifndef ADNS_9800_H_
#define ADNS_9800_H_

#include "mbed.h"

class adns_9800
{
public:
    /**
     * @brief Constructor
     */
    adns_9800(PinName mosi, PinName miso, PinName sclk, PinName ncs);
    
    /**
     * @brief Destructor
     */
    ~adns_9800();

    /**
     * @brief returns true if a motion has occured since the last readout
     */
    bool new_motion_data_available();
    
    /**
     * @brief retrieves the latest delta values
     */
    void get_delta_x_y(int16_t &delta_x, int16_t &delta_y);
        
private:
    SPI m_spi;
    DigitalOut m_ncs_pin;
    
    /**
     * @brief start and stop communication with the sensor by clearing/setting the ncs pin
     */
    void com_begin();
    void com_end();
    /**
     * @brief provide read/write access to a adns register
     */
    uint8_t read_reg(uint8_t const address);
    void write_reg(uint8_t const address, uint8_t const data);
    /**
     * @brief upload the firmware
     */
    void upload_firmware();
    /**
     * @brief starts the sensor up
     */
    void startup();
};

#endif
