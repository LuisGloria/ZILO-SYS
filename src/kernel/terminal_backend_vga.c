#include "terminal_backend_vga.h"

static volatile uint16_t* const VGA = (uint16_t*)0xB8000;

static int prev_x = 0;
static int prev_y = 0;
static uint8_t prev_char = ' ';
static uint8_t prev_color = 0x07;

void backend_putc_at(int row, int col, char c, uint8_t color)
{
    VGA[row * 80 + col] = (uint16_t)c | ((uint16_t)color << 8);
}

void vga_draw_cursor(int x, int y)
{
    // restore old cell
    backend_putc_at(prev_y, prev_x, prev_char, prev_color);

    // save new cell
    prev_char = ' ';
    prev_color = 0x07;

    // draw cursor
    backend_putc_at(y, x, 0xDB, 0x0F);

    // update state
    prev_x = x;
    prev_y = y;
}
