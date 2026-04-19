#include "panic.h"
#include "text.h"
#include "arch/i686/io.h"

static void dump_registers(Registers* regs)
{
    text_print("EAX=");
    text_print_hex(regs->eax);
    text_print(" EBX=");
    text_print_hex(regs->ebx);

    text_print("\nECX=");
    text_print_hex(regs->ecx);
    text_print(" EDX=");
    text_print_hex(regs->edx);

    text_print("\nEIP=");
    text_print_hex(regs->eip);
    text_print(" ESP=");
    text_print_hex(regs->esp);

    text_print("\n");
}

void panic(const char* message, Registers* regs)
{
    i686_DisableInterrupts();

    text_set_color(0x4F); // red bg, white text

    text_print("\n\n!!! KERNEL PANIC !!!\n\n");
    text_print(message);
    text_print("\n\n");

    dump_registers(regs);

    text_print("\nSystem halted.\n");

    for (;;)
        i686_Panic(); // hlt loop
}

void panic_now(const char* message)
{
    panic(message, (Registers*)0);
}
