#pragma once
#include <stdint.h>

// Core text output
void text_set_color(uint8_t color);
void text_clear();
void text_putc(char c);
void text_print(const char* str);

void text_write(const char* str, uint8_t color);   // force color print
void text_print_hex(uint32_t value);               // debug helper
void text_print_dec(uint32_t value);               // debug helper
void text_print_hex8(uint8_t value);
void text_scroll_up();
void text_scroll_down();
void text_follow_cursor();
