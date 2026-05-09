#pragma once
#include <stdint.h>

void term_init();
void term_putc(char c);
void term_write(const char* str);
void term_clear();
void term_set_color(uint8_t color);
