#include "cmos.h"
#include "arch/i686/io.h"

// CMOS ports
#define CMOS_ADDRESS 0x70
#define CMOS_DATA    0x71

static uint8_t cmos_get_register(int reg)
{
    i686_outb(CMOS_ADDRESS, reg);
    return i686_inb(CMOS_DATA);
}

static int bcd_to_bin(int val)
{
    return (val & 0x0F) + ((val / 16) * 10);
}

static int is_update_in_progress()
{
    i686_outb(CMOS_ADDRESS, 0x0A);
    return i686_inb(CMOS_DATA) & 0x80;
}

void cmos_read_rtc(cmos_time_t* t)
{
    uint8_t last_second;
    uint8_t last_minute;
    uint8_t last_hour;
    uint8_t last_day;
    uint8_t last_month;
    uint8_t last_year;

    while (is_update_in_progress());

    t->second = cmos_get_register(0x00);
    t->minute = cmos_get_register(0x02);
    t->hour   = cmos_get_register(0x04);
    t->day    = cmos_get_register(0x07);
    t->month  = cmos_get_register(0x08);
    t->year   = cmos_get_register(0x09);

    last_second = t->second;
    last_minute = t->minute;
    last_hour   = t->hour;
    last_day    = t->day;
    last_month  = t->month;
    last_year   = t->year;

    while (is_update_in_progress());

    if (last_second == t->second &&
        last_minute == t->minute &&
        last_hour   == t->hour &&
        last_day    == t->day &&
        last_month  == t->month &&
        last_year   == t->year)
    {
        // BCD mode check (Status Register B)
        i686_outb(CMOS_ADDRESS, 0x0B);
        uint8_t regB = i686_inb(CMOS_DATA);

        int bcd = !(regB & 0x04);

        if (bcd)
        {
            t->second = bcd_to_bin(t->second);
            t->minute = bcd_to_bin(t->minute);
            t->hour   = bcd_to_bin(t->hour & 0x7F);
            t->day    = bcd_to_bin(t->day);
            t->month  = bcd_to_bin(t->month);
            t->year   = bcd_to_bin(t->year);
        }

        t->year += 2000; // good enough lol
    }
}
