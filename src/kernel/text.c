#include <stdint.h>
#include "arch/i686/io.h"

static volatile uint16_t* const VGA = (uint16_t*)0xB8000;

static int row = 0;
static int col = 0;

static uint8_t current_color = 0x0F;

void text_set_color(uint8_t color)
{
    current_color = color;
}

static void text_update_cursor()
{
    uint16_t pos = row * 80 + col;

    i686_outb(0x3D4, 0x0F);          // low byte
    i686_outb(0x3D5, (uint8_t)(pos & 0xFF));

    i686_outb(0x3D4, 0x0E);          // high byte
    i686_outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

static uint16_t make_entry(char c, uint8_t color)
{
    return (uint16_t)c | ((uint16_t)color << 8);
}

static void text_scroll()
{
    // move everything up by 1 row
    for (int y = 1; y < 25; y++)
    {
        for (int x = 0; x < 80; x++)
        {
            VGA[(y - 1) * 80 + x] = VGA[y * 80 + x];
        }
    }

    // clear last row
    for (int x = 0; x < 80; x++)
    {
        VGA[(24) * 80 + x] = make_entry(' ', current_color);
    }

    row = 24;
}

void text_clear()
{
    for (int i = 0; i < 80 * 25; i++)
    {
        VGA[i] = make_entry(' ', 0x0F);
    }
    row = 0;
    col = 0;

    text_update_cursor();
}

void text_putc(char c)
{
    if (c == '\n')
    {
        col = 0;
        row++;
        if (row >= 25)
            text_scroll();

        text_update_cursor();
        return;
    }

    if (c == '\b')
    {
        if (col > 0)
        {
            col--;
            VGA[row * 80 + col] = make_entry(' ', current_color);
        }
        text_update_cursor();
        return;
    }

    VGA[row * 80 + col] = make_entry(c, current_color);

    col++;
    if (row >= 25)
    {
        text_scroll();
    }

    text_update_cursor();
}

void text_print(const char* str)
{
    for (int i = 0; str[i]; i++)
    {
        if (str[i] == '{' && str[i+1] == '0' && str[i+2] == 'x')
        {
            // parse hex color {0x..}
            uint8_t color = 0;
            i += 3;

            while (str[i] && str[i] != '}')
            {
                char c = str[i];

                color <<= 4;

                if (c >= '0' && c <= '9')
                    color |= (c - '0');
                else if (c >= 'A' && c <= 'F')
                    color |= (c - 'A' + 10);
                else if (c >= 'a' && c <= 'f')
                    color |= (c - 'a' + 10);

                i++;
            }

            text_set_color(color);
            continue;
        }

        text_putc(str[i]);
    }
}
// Black        0x0
// Blue         0x1
// Green        0x2
// Cyan         0x3
// Red          0x4
// Magenta      0x5
// Brown        0x6
// Light Gray   0x7
// Dark Grey    0x8
// Light Blue   0x9
// Light Green  0xA
// Light Cyan	0xB
// Light Red	0xC
// Pink         0xD
// Yellow	    0xE
// White    	0xF

// Example: Red on black: 0x04
// Background | Foreground
