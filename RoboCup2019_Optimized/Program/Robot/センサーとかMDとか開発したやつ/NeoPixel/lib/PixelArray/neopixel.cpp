#include <stdint.h>
#include "mbed.h"
#include "neopixel.h"

namespace neopixel
{

PixelArray::PixelArray(PinName mosi, PinName miso, PinName sclk, ByteOrder byte_order)
        : spi_(mosi, miso, sclk), byte_order_(byte_order)
{
        // 500kHz bit encodings:
        //  '0': 00010000      mark: 0.25us
        //  '1': 01110000      mark: 0.75us
        spi_.frequency(4000000);  // 500kh x 8
        spi_.format(8,0);
        spi_.set_dma_usage((DMAUsage)DMA_USAGE_ALLWAYS);
        code0 = 0x20;
        code1 = 0x70;
        spi_busy = false;
        spi_callback.attach(this, &PixelArray::spi_event_handler);
}


void PixelArray::spi_event_handler(int event){
        spi_busy = false;
}

void PixelArray::send_pixel(Pixel& pixel)
{
        uint8_t color0 = (byte_order_ == BYTE_ORDER_RGB) ? pixel.red : pixel.green;
        uint8_t color1 = (byte_order_ == BYTE_ORDER_RGB) ? pixel.green : pixel.red;
        uint8_t color2 = pixel.blue;

        while(spi_busy == true) {}
        spi_busy = true;

        uint8_t i = 0;
        for (; i<8; i++)
                rawbit_buffer[i] = (color0 & bitmask[i]) ? code1 : code0;
        for (; i<16; i++)
                rawbit_buffer[i] = (color1 & bitmask[i-8]) ? code1 : code0;
        for (; i<24; i++)
                rawbit_buffer[i] = (color2 & bitmask[i-16]) ? code1 : code0;

        spi_.transfer((void*)rawbit_buffer, 24, (void*)NULL, 0, spi_callback, SPI_EVENT_COMPLETE);
}

}
