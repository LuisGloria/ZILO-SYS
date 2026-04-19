#include "fb_terminal.h"
#include "fb.h"

static int cursor_x = 0;
static int cursor_y = 0;

#define CHAR_W 8
#define CHAR_H 16

void fb_terminal_putc(char c)
{
    // TEMP: draw a colored block per char (did this even get used in the first place...?)
    uint32_t color = 0xFFFFFF;

    for (int y = 0; y < CHAR_H; y++)
    {
        for (int x = 0; x < CHAR_W; x++)
        {
            fb_putpixel(cursor_x * CHAR_W + x,
                        cursor_y * CHAR_H + y,
                        color);
        }
    }

    cursor_x++;

    if (cursor_x >= 80)
    {
        cursor_x = 0;
        cursor_y++;
    }
}
