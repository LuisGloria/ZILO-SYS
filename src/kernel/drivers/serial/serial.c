#include "serial.h"
#include "../../text.h"
#include "../../arch/i686/io.h"

#define COM1 0x3F8

void serial_init()
{
    i686_outb(COM1 + 1, 0x00); // Disable interrupts
    i686_outb(COM1 + 3, 0x80); // Enable DLAB
    i686_outb(COM1 + 0, 0x03); // Baud low byte (38400)
    i686_outb(COM1 + 1, 0x00); // Baud high byte
    i686_outb(COM1 + 3, 0x03); // 8 bits, no parity, 1 stop
    i686_outb(COM1 + 2, 0xC7); // Enable FIFO
    i686_outb(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
    text_print("[{0x08}SERIAL{0x0F}]      Serial ready at COM1\n");
}

static int serial_ready()
{
    return i686_inb(COM1 + 5) & 0x20;
}

void serial_write(char c)
{
    while (!serial_ready());

    i686_outb(COM1, c);
}

void serial_print(const char* str)
{
    while (*str)
    {
        if (*str == '\n')
            serial_write('\r');

        serial_write(*str++);
    }
}
