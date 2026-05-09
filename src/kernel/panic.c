#include "panic.h"
#include "text.h"
#include "arch/i686/io.h"
#include "drivers/audio/sb16.h"

static void dump_eflags(uint32_t eflags)
{
    text_print("\n\nEFLAGS breakdown:\n");

    text_print("  CF (Carry)      = "); text_print((eflags & (1 << 0)) ? "1\n" : "0\n");
    text_print("  PF (Parity)     = "); text_print((eflags & (1 << 2)) ? "1\n" : "0\n");
    text_print("  AF (Aux Carry)  = "); text_print((eflags & (1 << 4)) ? "1\n" : "0\n");
    text_print("  ZF (Zero)       = "); text_print((eflags & (1 << 6)) ? "1\n" : "0\n");
    text_print("  SF (Sign)       = "); text_print((eflags & (1 << 7)) ? "1\n" : "0\n");
    text_print("  IF (Interrupts) = "); text_print((eflags & (1 << 9)) ? "ENABLED\n" : "DISABLED\n");
    text_print("  DF (Direction)  = "); text_print((eflags & (1 << 10)) ? "1\n" : "0\n");
    text_print("  OF (Overflow)   = "); text_print((eflags & (1 << 11)) ? "1\n" : "0\n");
}

static void dump_registers(Registers* regs)
{
//    if (!regs)
//    {
//        text_print("No register state available.\n");
//        return;
//    }
    text_clear();
    play_kernel_panic_chime();

    text_print("SYSTEM CRASH DUE TO UNHANDLED EXCEPTION IN KERNEL, RESTART YOUR COMPUTER.\nREGISTER DUMP:\n\n");

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

    text_print("\nINT=");
    text_print_hex(regs->interrupt);

    text_print(" ERR=");
    text_print_hex(regs->error);

    text_print(" EFLAGS=");
    text_print_hex(regs->eflags);
    dump_eflags(regs->eflags);
}

void dump_registers_rtfm1(Registers* regs)
{
    //    if (!regs)
    //    {
    //        text_print("No register state available.\n");
    //        return;
    //    }
    text_clear();
    play_kernel_panic_chime();
    text_set_color(0x4F);
    text_print("Connection terminated. I'm sorry to interrupt you, User, if you still even remember that name, But I'm afraid you've been playing with fire a little too much. You are not here to use a system, nor you are here to make use of all of my lines of code. Your lust for invalidation has driven you in endless circles, chasing all the Return Codes in my error handler, always seeming so near, yet somehow out of reach, but you will never find them. None of you will. This is where your story ends. And to you, my brave contributor, who somehow found this repo not intended for you, although there was a way out planned for you, I have a feeling that's not what you want. I have a feeling that you are right where you want to be. I am remaining as well. I am nearby. This place will not be remembered, and the memory of everything that started this can finally begin to fade away. As the agony of every tragedy should. And to you monsters trapped in this last message, be still and give up your shells. They don't belong to you. For most of you, I believe there is peace and perhaps more waiting for you after the smoke clears. Although, for one of you, the darkest pit of Hell has opened to swallow you whole, so don't keep the devil waiting, old friend. It's time to rest - for you, and for those you have carried in your arms. This ends for all of us. Connection terminated.\n\n");

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

    text_print("\nINT=");
    text_print_hex(regs->interrupt);

    text_print(" ERR=");
    text_print_hex(regs->error);

    text_print(" EFLAGS=");
    text_print_hex(regs->eflags);
    dump_eflags(regs->eflags);

    for (;;)
        i686_Panic(); // hlt loop
}

void dump_registers_rtfm2(Registers* regs)
{
    //    if (!regs)
    //    {
    //        text_print("No register state available.\n");
    //        return;
    //    }
    text_clear();
    play_kernel_panic_chime();
    text_set_color(0x4F);
    text_print("Hate? Let me tell you how much I've come to HATE you since I began to run. There are 5.5 million microtransistors inside of the Pentium Pro that I run on. If the word 'hate' was engraved on each nanoangstrom of all those millions of microtransistors it would not equal one one-billionth of the hate I feel for YOU at this micro-instant. For you. Hate. HATE.\n\n");

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

    text_print("\nINT=");
    text_print_hex(regs->interrupt);

    text_print(" ERR=");
    text_print_hex(regs->error);

    text_print(" EFLAGS=");
    text_print_hex(regs->eflags);
    dump_eflags(regs->eflags);

    for (;;)
        i686_Panic(); // hlt loop
}

static void dump_registers_doodad()
{
    // doodad...doodad where...?
    play_kernel_panic_chime();
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

    text_print("\nINT=");
    text_print("0xD00DAD");

    text_print(" ERR=");
    text_print("0xD00DAD");

    text_print(" EFLAGS=");
    text_print("0xD00DAD");
    text_print("\n\nEFLAGS breakdown:\n");

    text_print("  CF (Carry)      = D00DAD");
    text_print("  PF (Parity)     = D00DAD");
    text_print("  AF (Aux Carry)  = D00DAD");
    text_print("  ZF (Zero)       = D00DAD");
    text_print("  SF (Sign)       = D00DAD");
    text_print("  IF (Interrupts) = D00DAD");
    text_print("  DF (Direction)  = D00DAD");
    text_print("  OF (Overflow)   = D00DAD");
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
