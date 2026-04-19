#pragma once
#include <stdint.h>

typedef struct
{
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;

    uint32_t interrupt;
    uint32_t error;

    uint32_t eip, cs, eflags, useresp, ss;
} __attribute__((packed)) Registers;

typedef void (*ISRHandler)(Registers* regs);

void i686_ISR_Initialize();
void i686_ISR_RegisterHandler(int interrupt, ISRHandler handler);
