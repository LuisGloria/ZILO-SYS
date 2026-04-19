#include "text.h"
#include "drivers/video/fb_terminal.h"
#include "arch/i686/io.h"
#include "terminal_backend_vga.h"

#define TERM_W 80
#define TERM_H 25
#define TERM_SCROLLBACK 200

typedef struct {
    char c;
    uint8_t color;
} cell_t;

static cell_t buffer[TERM_SCROLLBACK][TERM_W];
static int scroll_offset = 0; // top of visible window
static int cursor_row = 0;
static int cursor_col = 0;

static int mode = 1;
// 0 = VGA text
// 1 = framebuffer

static uint8_t current_color = 0x0F;

static inline void clamp_cursor()
{
    if (cursor_row < 0) cursor_row = 0;
    if (cursor_col < 0) cursor_col = 0;

    if (cursor_row >= TERM_SCROLLBACK)
        cursor_row = TERM_SCROLLBACK - 1;

    if (cursor_col >= TERM_W)
        cursor_col = TERM_W - 1;
}

static void text_update_cursor()
{
    int screen_y = cursor_row - scroll_offset;

    // only show cursor if it's in visible window
    if (screen_y < 0 || screen_y >= TERM_H)
        return;

    uint16_t pos = screen_y * TERM_W + cursor_col;

    i686_outb(0x3D4, 0x0F);
    i686_outb(0x3D5, (uint8_t)(pos & 0xFF));

    i686_outb(0x3D4, 0x0E);
    i686_outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

static void text_redraw()
{
    for (int y = 0; y < TERM_H; y++)
    {
        int buf_y = y + scroll_offset;

        if (buf_y >= TERM_SCROLLBACK)
            break;

        for (int x = 0; x < TERM_W; x++)
        {
            cell_t *cell = &buffer[buf_y][x];
            backend_putc_at(y, x, cell->c, cell->color);
        }
    }

    text_update_cursor();
}

void term_putc(char c)
{
    if (mode == 0)
        text_putc(c);
    else
        fb_terminal_putc(c);
}

void text_set_color(uint8_t color)
{
    current_color = color;
}

// clear screen
void text_clear()
{
    for (int y = 0; y < TERM_SCROLLBACK; y++)
    {
        for (int x = 0; x < TERM_W; x++)
        {
            buffer[y][x].c = ' ';
            buffer[y][x].color = current_color;
        }
    }

    cursor_row = 0;
    cursor_col = 0;
    scroll_offset = 0;

    text_redraw();
}

// scroll (buffer based)
static void text_scroll()
{
    if (cursor_row - scroll_offset >= TERM_H)
    {
        if (scroll_offset < TERM_SCROLLBACK - TERM_H)
            scroll_offset++;
    }
    text_redraw();
}

// put char (buffer + vga)
void text_putc(char c)
{
    if (c == '\n')
    {
        cursor_col = 0;
        cursor_row++;

        if (cursor_row >= TERM_SCROLLBACK)
            cursor_row = TERM_SCROLLBACK - 1;

        if (cursor_row - scroll_offset >= TERM_H)
            text_scroll();

        text_redraw();
        return;
    }

    if (c == '\b')
    {
        if (cursor_col > 0)
        {
            cursor_col--;

            buffer[cursor_row][cursor_col].c = ' ';
            buffer[cursor_row][cursor_col].color = current_color;
        }

        text_redraw();
        return;
    }

    buffer[cursor_row][cursor_col].c = c;
    buffer[cursor_row][cursor_col].color = current_color;

    cursor_col++;

    if (cursor_col >= TERM_W)
    {
        cursor_col = 0;
        cursor_row++;
    }

    if (cursor_row >= TERM_SCROLLBACK)
        cursor_row = TERM_SCROLLBACK - 1;

    if (cursor_row - scroll_offset >= TERM_H)
        text_scroll();

    text_redraw();
    clamp_cursor();
}

// print string
void text_print(const char* str)
{
    for (int i = 0; str[i]; i++)
    {
        if (str[i] == '{' && str[i+1] == '0' && str[i+2] == 'x')
        {
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

// print decimal
void text_print_dec(uint32_t value)
{
    char buf[16];
    int i = 0;

    if (value == 0)
    {
        text_putc('0');
        return;
    }

    while (value > 0)
    {
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }

    for (int j = i - 1; j >= 0; j--)
        text_putc(buf[j]);
}

// print hex
void text_print_hex(uint32_t value)
{
    const char* hex = "0123456789ABCDEF";

    text_print("0x");

    for (int i = 28; i >= 0; i -= 4)
    {
        uint8_t digit = (value >> i) & 0xF;
        text_putc(hex[digit]);
    }
}

void text_print_hex8(uint8_t value)
{
    const char* hex = "0123456789ABCDEF";

    text_putc(hex[(value >> 4) & 0xF]);
    text_putc(hex[value & 0xF]);
}

void text_scroll_up()
{
    if (scroll_offset > 0)
        scroll_offset--;

    text_redraw();
}

void text_scroll_down()
{
    if (scroll_offset < TERM_SCROLLBACK - TERM_H)
        scroll_offset++;

    text_redraw();
}

void text_follow_cursor()
{
    if (cursor_row < scroll_offset)
        scroll_offset = cursor_row;

    if (cursor_row >= scroll_offset + TERM_H)
        scroll_offset = cursor_row - TERM_H + 1;

    text_redraw();
}
