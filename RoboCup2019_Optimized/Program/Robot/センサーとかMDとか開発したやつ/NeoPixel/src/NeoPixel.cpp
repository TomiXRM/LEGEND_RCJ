#include "mbed.h"
#include "neopixel.h"

// This must be an SPI MOSI pin.
#define DATA_PIN D11


int main()
{


        // Create a temporary DigitalIn so we can configure the pull-down resistor.
        // (The mbed API doesn't provide any other way to do this.)
        // An alternative is to connect an external pull-down resistor.
        DigitalIn(DATA_PIN, PullDown);

        // The pixel array control class.
        neopixel::PixelArray array(DATA_PIN);
        // Declare array/buffer of type Pixel.
        uint16_t numPixels = 4;
        neopixel::Pixel pixels[numPixels];
        // Use buffer method of updating pixels.
        pixels[0].red = 255;
        pixels[0].green = 0;
        pixels[0].blue = 0;
        pixels[1].red = 255;
        pixels[1].green = 140;
        pixels[1].blue = 0;
        pixels[2].red = 0;
        pixels[2].green = 255;
        pixels[2].blue = 0;
        pixels[3].red = 0;
        pixels[3].green = 0;
        pixels[3].blue = 255;
        while (1) {
                array.update(pixels, numPixels);

        }
}
