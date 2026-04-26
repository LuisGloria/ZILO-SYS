#include <stdint.h>
#include "power.h"
#include "arch/i686/io.h"

void system_reset()
{
    // Wait until keyboard controller input buffer is clear
    while (i686_inb(0x64) & 0x02);

    i686_outb(0x64, 0xFE);
    for (;;)
        __asm__ volatile("hlt");
}

void system_shutdown()
{
    // QEMU / Bochs / some VMs
    i686_outw(0x604, 0x2000);

    for (;;)
        __asm__ volatile("hlt");
}
