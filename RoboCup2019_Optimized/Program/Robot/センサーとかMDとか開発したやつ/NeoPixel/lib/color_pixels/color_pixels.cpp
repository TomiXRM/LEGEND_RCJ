/** Color pixels library using WS2812B and nRF51822 (16MHz)
 *  It's for
 *    + http://www.seeedstudio.com/depot/Digital-RGB-LED-FlexiStrip-60-LED-1-Meter-p-1666.html
 *    + http://www.seeedstudio.com/depot/WS2812B-Digital-RGB-LED-Waterproof-FlexiStrip-144-LEDmeter-2-meter-p-1869.html
 *    + http://www.seeedstudio.com/depot/WS2812B-RGB-LED-with-Integrated-Driver-Chip-10-PCs-pack-p-1675.html
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Seeed Technology Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "color_pixels.h"
#include "nrf51.h"
#include "nrf51_bitfields.h"

#if !defined ( __GNUC__ )
// Generate a high level pulse (0.81us) of WS2812B's 1 code (0.9us +- 0.15us)
#define COLOR_PIXELS_ONE_HIGH(mask)     \
            NRF_GPIO->OUTSET = (mask);  \
            __ASM ( \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                ); \
            NRF_GPIO->OUTCLR = (mask)

// Generate a high level pulse (0.31us) of WS2812B's 0 code (0.35us +- 0.15us)
#define COLOR_PIXELS_ZERO_HIGH(mask)    \
            NRF_GPIO->OUTSET = (mask);  \
            __ASM (  \
                    " NOP\n\t"  \
                );  \
            NRF_GPIO->OUTCLR = (mask);  \
            __ASM ( \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                )
#else
// Generate a high level pulse (0.94us) of WS2812B's 1 code (0.9us +- 0.15us)
#define COLOR_PIXELS_ONE_HIGH(mask)     \
            NRF_GPIO->OUTSET = (mask);  \
            __ASM ( \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                ); \
            NRF_GPIO->OUTCLR = (mask)

// Generate a high level pulse (0.44us) of WS2812B's 0 code (0.35us +- 0.15us)
#define COLOR_PIXELS_ZERO_HIGH(mask)    \
            NRF_GPIO->OUTSET = (mask);  \
            __ASM (  \
                    " NOP\n\t"  \
                );  \
            NRF_GPIO->OUTCLR = (mask);  \
            __ASM ( \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                    " NOP\n\t" \
                )
#endif            

static void delay_us(uint32_t us)
{
    do {
        __ASM volatile (
            "NOP\n\t"
            "NOP\n\t"
            "NOP\n\t"
            "NOP\n\t"
            "NOP\n\t"
            "NOP\n\t"
            "NOP\n\t"
            "NOP\n\t"
            "NOP\n\t"
            "NOP\n\t"
            "NOP\n\t"
            "NOP\n\t"
        );
    } while (--us);
}

ColorPixels::ColorPixels(uint8_t pin, uint16_t num)
{
    this->pin = pin;
    this->num = num;

    NRF_GPIO->PIN_CNF[pin] = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
                             | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
                             | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
                             | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
                             | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
    NRF_GPIO->OUTCLR = (1UL << pin);

    colors = new grb_t[num];
    for (int i = 0; i < num; i++) {
        colors[i].grb = 0;
    }
}

ColorPixels::~ColorPixels()
{
    delete colors;
}


void ColorPixels::set_color(uint16_t index, uint8_t r, uint8_t g, uint8_t b)
{
    if (index < num) {
        colors[index].r = r;
        colors[index].g = g;
        colors[index].b = b;
    }
}

void ColorPixels::set_color(uint16_t index, uint32_t rgb) {
    color_t c = *(color_t *)&rgb;
    set_color(index, c.r, c.g, c.b);
}

void ColorPixels::set_all_color(uint8_t r, uint8_t g, uint8_t b)
{
    for (int i = 0; i < num; i++) {
        colors[i].r = r;
        colors[i].g = g;
        colors[i].b = b;
    }

    update();
}

void ColorPixels::update()
{
    uint32_t mask =  1 << pin;
    NRF_GPIO->OUTCLR = mask;
    delay_us(50);
    
    grb_t *pcolor = colors;
    for (int i = 0; i < num; i++) {
        uint32_t grb = (*pcolor).grb;
        for (int bit = 0; bit < 24; bit++) {
            if (grb & 1) {
                COLOR_PIXELS_ONE_HIGH(mask);
            } else {
                COLOR_PIXELS_ZERO_HIGH(mask);
            }
            grb >>= 1;
        }
        pcolor++;
    }
}

void ColorPixels::clear()
{
    for (int i = 0; i < num; i++) {
        colors[i].grb = 0;
    }

    update();
}

uint32_t toRGB(uint8_t red, uint8_t green, uint8_t blue)
{
    color_t c;
    
    c.r = red;
    c.g = green;
    c.b = blue;
    
    return c.rgb;
};

uint32_t wheel(float position, uint8_t min = 0, uint8_t max = 255) 
{
    uint8_t d = max - min;
    uint32_t c;

    if(position < 0.166) {
        c = toRGB(max, min + position * 6 * d, min);
    } else if(position < 0.332) {
        position -= 0.166;
        c = toRGB(max - position * 6 * d, max, min);
    } else if(position < 0.498) {
        position -= 0.332;
        c = toRGB(min, max, min + position * 6 * d);
    } else if(position < 0.664) {
        position -= 0.498;
        c = toRGB(min, max - position * 6 * d, max);
    } else if(position < 0.83) {
        position -= 0.664;
        c = toRGB(min + d * position * 6, min, max);
    } else {
        position -= 0.83;
        c = toRGB(max, min, max - position * 6 * d);
    }
    
    return c;
}

void find_wheel_options(uint32_t rgb, float *position, uint8_t *min, uint8_t *max)
{
    color_t c;
    c.rgb = rgb;
    
    if (c.r > c.g) {
        if (c.g >= c.b) {
            *max = c.r;
            *min = c.b;
            *position = (float)(c.g - c.b) / (c.r - c.b) / 6.0;
        } else if (c.b >= c.r) {
            *max = c.b;
            *min = c.g;
            *position = (float)(c.r - c.g) / (c.b - c.g) / 6.0 + 0.664;
        } else {
            *max = c.r;
            *min = c.g;
            *position = (float)(c.b - c.g) / (c.r - c.g) / 6.0 + 0.83;
        }
    } else {
        if (c.r > c.b) {
            *max = c.g;
            *min = c.b;
            *position = (float)(c.r - c.b) / (c.g - c.b) / 6.0 + 0.166;
        } else if (c.b > c.g) {
            *max = c.b;
            *min = c.r;
            *position = (float)(c.g - c.r) / (c.b - c.r) / 6.0 + 0.498;
        } else {
            *max = c.g;
            *min = c.r;
            *position = (float)(c.b - c.r) / (c.g - c.r) / 6.0 + 0.332;
        }
    }
}

void ColorPixels::rainbow(uint32_t rgb)
{
    float position;
    uint8_t min, max;
    
    find_wheel_options(rgb, &position, &min, &max);
    for (int i = 0; i < num; i++) {
        float current = ((int)(i + num * position) % num) / (float)num;
        set_color(i, wheel(current, min, max));
    }
    
    update();
}

void ColorPixels::rainbow(uint8_t r, uint8_t g, uint8_t b)
{
    color_t c;
    
    c.r = r;
    c.g = g;
    c.b = b;
    
    rainbow(c.rgb);
}

