#include <stdint.h>
#include "vga.h"

static uint8_t* const VGA = (uint8_t*)0xA0000;

void vga_putpixel(int x, int y, uint8_t color)
{
    if ((unsigned)x >= 320 || (unsigned)y >= 200)
        return;

    VGA[y * 320 + x] = color;
}
