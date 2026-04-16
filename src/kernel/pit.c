#include "pit.h"
#include "arch/i686/io.h"
#include "arch/i686/irq.h"

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43

static volatile uint32_t ticks = 0;

void pit_handler(Registers* regs)
{
    ticks++;
}

uint32_t pit_get_ticks()
{
    return ticks;
}

void pit_init(uint32_t frequency)
{
    // Register IRQ0 handler
    i686_IRQ_RegisterHandler(0, pit_handler);

    uint32_t divisor = 1193180 / frequency;

    // command byte:
    // channel 0, lobyte/hibyte, mode 3, binary
    i686_outb(PIT_COMMAND, 0x36);

    i686_outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
    i686_outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));
}
