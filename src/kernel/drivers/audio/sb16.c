#include "sb16.h"
#include "../../text.h"
#include "../../arch/i686/io.h"

#define SB16_RESET     0x226
#define SB16_READ      0x22A
#define SB16_WRITE     0x22C
#define SB16_STATUS    0x22E

static void sb16_wait_write()
{
    while (i686_inb(SB16_WRITE) & 0x80);
}

static void sb16_write(uint8_t value)
{
    sb16_wait_write();
    i686_outb(SB16_WRITE, value);
}

static int sb16_read_ready()
{
    return (i686_inb(SB16_STATUS) & 0x80);
}

static uint8_t sb16_read()
{
    while (!sb16_read_ready());
    return i686_inb(SB16_READ);
}

int sb16_init()
{
    text_print("[{0x08}SB16{0x0F}]        Initializing SB16...\n");

    // Reset DSP
    i686_outb(SB16_RESET, 1);

    for (volatile int i = 0; i < 10000; i++);

    i686_outb(SB16_RESET, 0);

    if (!sb16_read_ready())
    {
        text_print("[{0x08}SB16{0x0F}]        No response\n");
        return 0;
    }

    uint8_t val = sb16_read();

    if (val != 0xAA)
    {
        text_print("[{0x08}SB16{0x0F}]        Invalid response\n");
        return 0;
    }

    text_print("[{0x08}SB16{0x0F}]        Detected successfully\n");
    return 1;
}

void sb16_beep_test()
{
    text_print("SB16 test placeholder\n");

    /* later:
     *      setup DMA
     *      send PCM
     *      trigger IRQ
     */
}
