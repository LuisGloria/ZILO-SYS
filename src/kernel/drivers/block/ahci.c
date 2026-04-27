#include "ahci.h"
#include "../../arch/i686/pci.h"
#include "../../text.h"

void ahci_init(uint8_t bus, uint8_t slot, uint8_t func)
{
    uint32_t bar5 = pci_read_dword(bus, slot, func, 0x24);

    if (bar5 == 0 || bar5 == 0xFFFFFFFF)
    {
        text_print("[{0x08}AHCI{0x0F}]        Invalid BAR5\n");
        return;
    }

    uint32_t abar = bar5 & 0xFFFFFFF0;

    text_print("[{0x08}AHCI{0x0F}]        Controller found at: ");
    text_print_hex(abar);
    text_print("\n");

    // TODO: parse HBA memory here smh
}


