#include "panic.h"
#include "text.h"
#include "arch/i686/io.h"

static void dump_registers(Registers* regs)
{
//    if (!regs)
//    {
//        text_print("No register state available.\n");
//        return;
//    }

    text_print("EAX="); text_print_hex(regs->eax);
    text_print(" EBX="); text_print_hex(regs->ebx);
    text_print(" ECX="); text_print_hex(regs->ecx);
    text_print(" EDX="); text_print_hex(regs->edx);

    text_print("\nESI="); text_print_hex(regs->esi);
    text_print(" EDI="); text_print_hex(regs->edi);
    text_print(" EBP="); text_print_hex(regs->ebp);
    text_print(" ESP="); text_print_hex(regs->esp);

    text_print("\nEIP="); text_print_hex(regs->eip);
    text_print(" CS ="); text_print_hex(regs->cs);
    text_print(" DS ="); text_print_hex(regs->ds);

    text_print(" EFLAGS=");
    text_print_hex(regs->eflags);

    text_print("\nINT=");
    text_print_hex(regs->interrupt);

    text_print(" ERR=");
    text_print_hex(regs->error);
}

static void dump_registers_doodad()
{
    // doodad...doodad where...?

    text_print("EAX="); text_print("0xD00DAD");
    text_print(" EBX="); text_print("0xD00DAD");
    text_print(" ECX="); text_print("0xD00DAD");
    text_print(" EDX="); text_print("0xD00DAD");

    text_print("\nESI="); text_print("0xD00DAD");
    text_print(" EDI="); text_print("0xD00DAD");
    text_print(" EBP="); text_print("0xD00DAD");
    text_print(" ESP="); text_print("0xD00DAD");

    text_print("\nEIP="); text_print("0xD00DAD");
    text_print(" CS ="); text_print("0xD00DAD");
    text_print(" DS ="); text_print("0xD00DAD");

    text_print(" EFLAGS=");
    text_print("0xD00DAD");

    text_print("\nINT=");
    text_print("0xD00DAD");

    text_print(" ERR=");
    text_print("0xD00DAD");
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

void panic_now_silent(const char* message)
{
    text_set_color(0x4F);
    text_print("\n\n!!! KERNEL PANIC !!!\n\n");
    text_print(message);
    text_print("\n\n");
    dump_registers_doodad();
    text_print("\nSystem halted.\n");

    for (;;)
        i686_Panic(); // hlt loop
}
