#pragma once
#include <stdint.h>

void pit_init(uint32_t frequency);
uint32_t pit_get_ticks();
