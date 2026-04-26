#include "speakers.h"
#include "../../arch/i686/io.h"

static void pit_prepare(uint32_t freq)
{
    uint32_t div = 1193180 / freq;

    // PIT channel 2, square wave
    i686_outb(0x43, 0xB6);

    i686_outb(0x42, (uint8_t)(div));
    i686_outb(0x42, (uint8_t)(div >> 8));
}

void speaker_on(uint32_t freq)
{
    pit_prepare(freq);

    uint8_t tmp = i686_inb(0x61);

    i686_outb(0x61, tmp | 3);
}

void speaker_off()
{
    uint8_t tmp = i686_inb(0x61) & 0xFC;
    i686_outb(0x61, tmp);
}

void speaker_beep(uint32_t freq, uint32_t duration)
{
    speaker_on(freq);

    // primitive delay loop
    for (volatile uint32_t i = 0; i < duration * 10000; i++);

    speaker_off();
}
