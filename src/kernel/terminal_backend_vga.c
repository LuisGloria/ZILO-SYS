#include "terminal_backend_vga.h"

static volatile uint16_t* const VGA = (uint16_t*)0xB8000;

void backend_putc_at(int row, int col, char c, uint8_t color)
{
    VGA[row * 80 + col] = ((uint16_t)c | ((uint16_t)color << 8));
}
