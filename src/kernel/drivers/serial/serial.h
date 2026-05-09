#pragma once
#include <stdint.h>

void serial_init();
void serial_write(char c);
void serial_print(const char* str);
