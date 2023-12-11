/*
 * mbed library program
 *  Control AT24C1024 EEPROM
 *
 * Copyright (c) 2014,'15 Kenji Arai / JH1PJL
 *  http://www.page.sannet.ne.jp/kenjia/index.html
 *  http://mbed.org/users/kenjiArai/
 *      Created: June      17th, 2014
 *      Revised: June       7th, 2015
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef AT24C1024_H
#define AT24C1024_H

#include "mbed.h"

/** Interface for 1Mbit Serial EEPROM (I2C Interface) Atmel Corp. AT24C1024B
 *
 * @code
 * #include "mbed.h"
 *
 * // I2C Communication
 *  AT24C1024   at24c1024(dp5,dp27);// SDA, SCL
 * // If you connected I2C line not only this device but also other devices,
 * //     you need to declare following method.
 *  I2C         i2c(dp5,dp27);      // SDA, SCL
 *  AT24C1024   at24c1024(i2c);     // Atmel 1Mbit EE-PROM
 *
 * int main() {
 *   at24c1024.write(0,dt);     // write addr=0 data=dt
 *   dt = at24c1024.read(0);    // read addr=0
 *   // page write mode, addr top=0x1ff00, 256bytes data (save it in eep_buf[])
 *   status = at24c1024.write_page(0x1ff00, eep_buf, sizeof(eep_buf));
 * }
 * @endcode
 */

// EEPROM Atmel AT24C1024
// Address b7=1,b6=0,b5=1,b4=0,b3=A2pin,b2=A1pin,b1=page addr, b0=R/W
#define AT24C1024ADDR  0xa0        //  as initial data (A2=A1=GND)

typedef enum {
    AT24C_OK = 0,
    AT24C_BUSY,
    AT24C_WRONG_BF_SIZE,
    AT24C_WRONG_TOP_ADDR,
    AT24C_ERROR
} AT24C_STATUS;

class AT24C1024
{
public:
    /** Configure data pin
      * @param data SDA and SCL pins
      */
    AT24C1024(PinName p_sda, PinName p_scl);

    /** Configure data pin (with other devices on I2C line)
      * @param I2C previous definition
      */
    //AT24C1024(I2C& p_i2c);

    /** Read one data from EEPROM
      * @param dat_address (0 to 0x1ffff)
      * @return read data
      */
    uint8_t read(uint32_t addr);

    /** Write one data to EEPROM
      * @param data_address (o to 0x1ffff), data (byte)
      * @return none
      */
    void write(uint32_t addr, uint8_t dt);

    /** Page read (256 bytes) from EEPROM
      * @param top_address (o to 0x1ffxx) e.g. 0x00,0x100,0x200 --- 0x1fe00, 0x1ff00
      * @param dt -> buffer (at least need to keep 258 bytes), data top = buffer[0]
      * @param size -> sizeof(buffer)
      * @return status 0=OK, others=Error
      */
    AT24C_STATUS read_page(uint32_t addr_page_top, uint8_t *dt, int size);

    /** Page write (256 bytes) to EEPROM
      * @param top_address (o to 0x1ffxx) e.g. 0x00,0x100,0x200 --- 0x1fe00, 0x1ff00
      * @param dt -> buffer (at least need to keep 258 bytes), data top = buffer[0]
      * @param size -> sizeof(buffer)
      * @return status 0=OK, others=Error
      */
    AT24C_STATUS write_page(uint32_t addr_page_top, uint8_t *dt, int size);

    /** Set I2C clock frequency
      * @param freq.
      * @return none
      */
    void frequency(int hz);

protected:
    I2C _i2c;

private:
    uint8_t AT24C1024_addr;
};

#endif // AT24C1024_H
