#include "time.h"
#include "pit.h"

void sleep(uint32_t ms)
{
    uint32_t start = pit_get_ticks();

    // assuming PIT is 1000 Hz → 1 tick = 1 ms
    while ((pit_get_ticks() - start) < ms)
    {
        // halt CPU until next interrupt
        __asm__ volatile ("hlt"); // Might be broken...?
    }
}

uint32_t get_ticks()
{
    return pit_get_ticks();
}
