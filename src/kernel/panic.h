#pragma once
#include <stdint.h>
#include "arch/i686/isr.h"

void panic(const char* message, Registers* regs);

// optional helper for forced crashes cuz' why not?
void panic_now(const char* message);
