#pragma once
#include <stdint.h>

void text_set_color(uint8_t color);
void text_clear();
void text_putc(char c);
void text_print(const char* str);
