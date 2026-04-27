#include "mouse.h"
#include "../../arch/i686/io.h"
#include "../../terminal_backend_vga.h"
#include "../../text.h"

#define MOUSE_DATA      0x60
#define MOUSE_STATUS    0x64
#define MOUSE_COMMAND   0x64

volatile int mouse_updated = 0;

static int mouse_x = 40;
static int mouse_y = 12;

static uint8_t mouse_cycle = 0;
static int8_t mouse_byte[3];

static void mouse_wait(uint8_t type)
{
    uint32_t timeout = 100000;

    if (type == 0)
    {
        while (timeout--)
        {
            if ((i686_inb(MOUSE_STATUS) & 1) == 1)
                return;
        }
    }
    else
    {
        while (timeout--)
        {
            if ((i686_inb(MOUSE_STATUS) & 2) == 0)
                return;
        }
    }
}

static void mouse_write(uint8_t data)
{
    mouse_wait(1);
    i686_outb(MOUSE_COMMAND, 0xD4);

    mouse_wait(1);
    i686_outb(MOUSE_DATA, data);
}

static uint8_t mouse_read()
{
    mouse_wait(0);
    return i686_inb(MOUSE_DATA);
}

void mouse_draw_cursor()
{
    vga_draw_cursor(mouse_x, mouse_y);
}

void mouse_handler()
{
    uint8_t status = i686_inb(MOUSE_STATUS);

    while (status & 1)
    {
        int8_t data = i686_inb(MOUSE_DATA);

        if (mouse_cycle == 0)
        {
            if (!(data & 0x08))
                return;

            mouse_byte[0] = data;
            mouse_cycle++;
        }
        else if (mouse_cycle == 1)
        {
            mouse_byte[1] = data;
            mouse_cycle++;
        }
        else if (mouse_cycle == 2)
        {
            mouse_byte[2] = data;

            mouse_x += mouse_byte[1];
            mouse_y -= mouse_byte[2];

            if (mouse_x < 0) mouse_x = 0;
            if (mouse_y < 0) mouse_y = 0;
            if (mouse_x > 79) mouse_x = 79;
            if (mouse_y > 24) mouse_y = 24;

            mouse_updated = 1;

            mouse_cycle = 0;
        }

        status = i686_inb(MOUSE_STATUS);
    }
}

void mouse_init()
{
    text_print("[{0x08}MOUSE{0x0F}]       Initializing PS/2 mouse...\n");

    mouse_wait(1);
    i686_outb(MOUSE_COMMAND, 0xA8); // enable aux device

    mouse_wait(1);
    i686_outb(MOUSE_COMMAND, 0x20);

    mouse_wait(0);
    uint8_t status = i686_inb(MOUSE_DATA);

    status |= 2;

    mouse_wait(1);
    i686_outb(MOUSE_COMMAND, 0x60);

    mouse_wait(1);
    i686_outb(MOUSE_DATA, status);

    mouse_write(0xF6); // defaults
    mouse_read();

    mouse_write(0xF4); // enable streaming
    mouse_read();

    mouse_updated = 1;

    text_print("[{0x08}MOUSE{0x0F}]       Ready\n");
}

int mouse_get_x()
{
    return mouse_x;
}

int mouse_get_y()
{
    return mouse_y;
}
