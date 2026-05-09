#include "usb.h"
#include "../../text.h"

void usb_init(void)
{
    text_print("[{0x08}USB{0x0F}]         USB subsystem initialized\n");
}
