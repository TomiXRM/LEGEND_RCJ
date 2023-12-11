/*
 * mbed library program
 *  Control AT24C1024 EEPROM
 *
 * Copyright (c) 2014,'15 Kenji Arai / JH1PJL
 *  http://www.page.sannet.ne.jp/kenjia/index.html
 *  http://mbed.org/users/kenjiArai/
 *      Created: June      17th, 2014
 *      Revised: Feburary  21st, 2015
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "mbed.h"
#include "AT24C1024.h"

AT24C1024::AT24C1024 (PinName p_sda, PinName p_scl) : _i2c(p_sda, p_scl)
{
        AT24C1024_addr = AT24C1024ADDR;
}


uint8_t AT24C1024::read(uint32_t addr)
{
        uint8_t eep_dt[2];
        uint8_t tmp_addr;

        tmp_addr = AT24C1024_addr;
        if ( addr & 0x10000 ) { // addr > 0xffff then P0 sets 1
                tmp_addr += 0x02; // P0=1
        }
        eep_dt[0] = ( uint8_t )( (addr & 0xff00) >> 8 );
        eep_dt[1] = ( uint8_t )( (addr & 0xff) );
        _i2c.write((uint8_t)tmp_addr, (char *)eep_dt, 2);
        _i2c.read((uint8_t)tmp_addr, (char *)eep_dt, 1);
        return eep_dt[0];
}

void AT24C1024::write(uint32_t addr, uint8_t dt)
{
        uint8_t eep_dt[3];
        uint8_t tmp_addr;

        tmp_addr = AT24C1024_addr;
        if ( addr & 0x10000 ) { // addr > 0xffff then P0 sets 1
                tmp_addr += 0x02; // P0=1
        }
        eep_dt[0] = ( uint8_t )( (addr & 0xff00) >> 8 );
        eep_dt[1] = ( uint8_t )( (addr & 0xff) );
        eep_dt[2] = dt;
        _i2c.write((uint8_t)tmp_addr, (char *)eep_dt, 3);
}

AT24C_STATUS AT24C1024::read_page(uint32_t addr_page_top, uint8_t *dt, int size)
{
        uint8_t eep_dt[2];
        uint8_t tmp_addr;

        if (size >= 256 + 2) {
                if ((addr_page_top & 0xff) == 0) {
                        tmp_addr = AT24C1024_addr;
                        if ( addr_page_top & 0x10000 ) { // addr > 0xffff then P0 sets 1
                                tmp_addr += 0x02; // P0=1
                        }
                        eep_dt[0] = ( uint8_t )( (addr_page_top & 0xff00) >> 8 );
                        eep_dt[1] = 0;
                        _i2c.write((uint8_t)tmp_addr, (char *)eep_dt, 2);
                        _i2c.read((uint8_t)tmp_addr, (char *)dt, 256);
                        return AT24C_OK;
                } else {
                        return AT24C_WRONG_TOP_ADDR;
                }
        } else {
                return AT24C_WRONG_BF_SIZE;
        }
}

AT24C_STATUS AT24C1024::write_page(uint32_t addr_page_top, uint8_t *dt, int size)
{
        uint8_t tmp_addr;
        int i;

        if (size >= 256 + 2) {
                if ((addr_page_top & 0xff) == 0) {
                        for (i = 255 + 2; i > 1; i--) { // shift data
                                dt[i] = dt[i-2];
                        }
                        tmp_addr = AT24C1024_addr;
                        if ( addr_page_top & 0x10000 ) { // addr > 0xffff then P0 sets 1
                                tmp_addr += 0x02; // P0=1
                        }
                        dt[0] = ( uint8_t )( (addr_page_top & 0xff00) >> 8 );
                        dt[1] = 0;
                        _i2c.write((uint8_t)tmp_addr, (char *)dt, 256 + 2);
                        return AT24C_OK;
                } else {
                        return AT24C_WRONG_TOP_ADDR;
                }
        } else {
                return AT24C_WRONG_BF_SIZE;
        }
}

void AT24C1024::frequency(int hz)
{
        _i2c.frequency(hz);
}
