#pragma once
#include <stdint.h>
#include "arch/i686/isr.h"

void panic(const char* message, Registers* regs);

// optional helper for forced crashes cuz' why not?
void panic_now(const char* message);
void panic_now_silent(const char* message);

void dump_registers_rtfm1(Registers* regs);
void dump_registers_rtfm2(Registers* regs);
