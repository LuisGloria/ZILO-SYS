#include "pci.h"
#include "io.h"
#include <stdio.h>
#include "../../text.h"

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC

uint32_t pci_get_bar0(uint8_t bus, uint8_t slot, uint8_t func)
{
    return pci_read_dword(bus, slot, func, 0x10);
}

uint32_t pci_read_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t address =
    (1 << 31) |
    (bus << 16) |
    (slot << 11) |
    (func << 8) |
    (offset & 0xFC);

    i686_outl(PCI_CONFIG_ADDRESS, address);
    return i686_inl(PCI_CONFIG_DATA);
}

uint16_t pci_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset)
{
    uint32_t data = pci_read_dword(bus, slot, func, offset);
    return (data >> ((offset & 2) * 8)) & 0xFFFF;
}

void pci_scan()
{
    text_print("[{0x08}PCI{0x0F}]         Scanning PCI...\n");

    for (uint16_t bus = 0; bus < 256; bus++)
    {
        for (uint8_t slot = 0; slot < 32; slot++)
        {
            uint16_t vendor = pci_read_word(bus, slot, 0, 0x00);

            if (vendor == 0xFFFF)
                continue;

            uint16_t device = pci_read_word(bus, slot, 0, 0x02);

            text_print("[{0x08}PCI{0x0F}]         PCI: bus=");
            text_print_dec(bus);

            text_print(" slot=");
            text_print_dec(slot);

            text_print(" vendor=");
            text_print_hex(vendor);

            text_print(" device=");
            text_print_hex(device);

            text_print("\n");

            // Detect RTL8139 specifically
            if (vendor == 0x10EC && device == 0x8139)
            {
                printf("[{0x08}PCI{0x0F}]         RTL8139 FOUND at bus=%d slot=%d\n", bus, slot);
            }
        }
    }
}
