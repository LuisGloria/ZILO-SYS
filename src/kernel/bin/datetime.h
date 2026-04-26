#pragma once
#include <stdint.h>

typedef struct
{
    uint8_t hour;
    uint8_t minute;
    uint8_t day;
    uint8_t month;
    uint16_t year;
} datetime_t;

void datetime_prompt();
datetime_t datetime_get();
