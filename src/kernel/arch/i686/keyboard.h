#pragma once
#include "irq.h"

extern int input_mode;
char* keyboard_get_line();
const char* keyboard_get_buffer();
void keyboard_init();
void keyboard_handler(Registers* regs);
int keyboard_poll_line(char* out, int max);
