#pragma once
#include <stdint.h>

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;

    uint8_t day;
    uint8_t month;
    uint32_t year;
} cmos_time_t;

void cmos_read_rtc(cmos_time_t* time);
