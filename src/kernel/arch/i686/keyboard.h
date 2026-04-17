#pragma once
#include "irq.h"

void keyboard_init();
void keyboard_handler(Registers* regs);
