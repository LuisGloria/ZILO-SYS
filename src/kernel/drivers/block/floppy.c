#include "floppy.h"
#include "../../arch/i686/io.h"
#include "../../text.h"

#define FLOPPY_MSR  0x3F4
#define FLOPPY_DOR  0x3F2

int floppy_detect()
{
    uint8_t msr = i686_inb(FLOPPY_MSR);

    if (msr == 0xFF || msr == 0x00)
        return 0;

    return 1;
}

void floppy_init()
{
    if (!floppy_detect())
    {
        text_print("[{0x08}FLOPPY{0x0F}]      No controller\n");
        return;
    }

    text_print("[{0x08}FLOPPY{0x0F}]      Controller detected\n");
}
