#include "syscall.h"
#include "stdio.h"
#include "time.h"

#define SYSCALL_PRINT   1
#define SYSCALL_TIME    2

static void syscall_handler(Registers* regs)
{
    switch (regs->eax)
    {
        case SYSCALL_PRINT:
        {
            const char* str = (const char*)regs->ebx;

            if (str)
                printf("[{0x08}SYSCALL{0x0F}]     %s\n", str);

            regs->eax = 0;
            break;
        }

        case SYSCALL_TIME:
        {
            regs->eax = get_ticks(); // if
            break;
        }

        default:
        {
            printf("Unknown syscall: %d\n", regs->eax);
            regs->eax = (uint32_t)-1;
            break;
        }
    }
}

void syscall_initialize()
{
    i686_ISR_RegisterHandler(0x80, syscall_handler);
}
