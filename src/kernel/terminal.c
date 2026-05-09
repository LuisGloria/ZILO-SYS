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
static int scroll_offset = 0;
static int cursor_row = 0;
static int cursor_col = 0;

static int mode = 1;

static uint8_t current_color = 0x0F;

//dirty flag
static int dirty = 1;

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
    if (!dirty)
        return;

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
    dirty = 0;
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

    dirty = 1;
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

    dirty = 1;
}

// put char (buffer only)
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

        dirty = 1;
        text_update_cursor();
        return;
    }

    if (c == '\b')
    {
        if (cursor_col > 0)
        {
            cursor_col--;

            buffer[cursor_row][cursor_col].c = ' ';
            buffer[cursor_row][cursor_col].color = current_color;

            int screen_y = cursor_row - scroll_offset;

            if (screen_y >= 0 && screen_y < TERM_H)
            {
                backend_putc_at(screen_y, cursor_col, ' ', current_color);
            }
        }

        dirty = 1;
        text_update_cursor();
        return;
    }

    buffer[cursor_row][cursor_col].c = c;
    buffer[cursor_row][cursor_col].color = current_color;

    int screen_y = cursor_row - scroll_offset;

    if (screen_y >= 0 && screen_y < TERM_H)
    {
        backend_putc_at(screen_y, cursor_col, c, current_color);
    }

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

    clamp_cursor();
    text_update_cursor();
    dirty = 1;
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

    text_redraw(); // redraw once after full string
}

// print decimal
void text_print_dec(uint32_t value)
{
    char buf[16];
    int i = 0;

    if (value == 0)
    {
        text_putc('0');
        text_redraw();
        return;
    }

    while (value > 0)
    {
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }

    for (int j = i - 1; j >= 0; j--)
        text_putc(buf[j]);

    text_redraw();
}

void text_print_int(int value)
{
    char buf[16];
    int i = 0;
    int is_negative = 0;

    if (value == 0)
    {
        text_putc('0');
        return;
    }

    if (value < 0)
    {
        is_negative = 1;
        value = -value;
    }

    while (value > 0 && i < 15)
    {
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }

    if (is_negative)
        text_putc('-');

    while (i--)
        text_putc(buf[i]);

    text_redraw();
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

    text_redraw();
}

void text_print_hex8(uint8_t value)
{
    const char* hex = "0123456789ABCDEF";

    text_putc(hex[(value >> 4) & 0xF]);
    text_putc(hex[value & 0xF]);

    text_redraw();
}

void text_scroll_up()
{
    if (scroll_offset > 0)
        scroll_offset--;

    dirty = 1;
    text_redraw();
}

void text_scroll_down()
{
    if (scroll_offset < TERM_SCROLLBACK - TERM_H)
        scroll_offset++;

    dirty = 1;
    text_redraw();
}

void text_follow_cursor()
{
    if (cursor_row < scroll_offset)
        scroll_offset = cursor_row;

    if (cursor_row >= scroll_offset + TERM_H)
        scroll_offset = cursor_row - TERM_H + 1;

    dirty = 1;
    text_redraw();
}

// Colors
//
// $00 Black
// $01 Blue
// $02 Green
// $03 Cyan
// $04 Red
// $05 Magenta
// $06 Brown
// $07 White
// $08 Gray
// $09 Light Blue
// $0A Light Green
// $0B Light Cyan
// $0C Light Red
// $0D Light Magenta
// $0E Yellow
// $0F Bright White
