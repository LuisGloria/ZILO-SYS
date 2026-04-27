#include "pci.h"
#include "io.h"
#include <stdio.h>
#include "../../text.h"
#include "../../drivers/block/ahci.h"

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC

ahci_device_t ahci_dev;
int ahci_found = 0;

gpu_device_t gpu_dev;
int gpu_found = 0;

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


uint8_t pci_get_class(uint8_t bus, uint8_t slot, uint8_t func)
{
    return pci_read_word(bus, slot, func, 0x0A) >> 8;
}

uint8_t pci_get_subclass(uint8_t bus, uint8_t slot, uint8_t func)
{
    return pci_read_word(bus, slot, func, 0x0A) & 0xFF;
}

uint8_t pci_get_prog_if(uint8_t bus, uint8_t slot, uint8_t func)
{
    return pci_read_word(bus, slot, func, 0x09) >> 8;
}

uint32_t pci_get_bar0(uint8_t bus, uint8_t slot, uint8_t func)
{
    return pci_read_dword(bus, slot, func, 0x10);
}


void pci_scan()
{
    text_print("[{0x08}PCI{0x0F}]         Scanning PCI...\n");

    for (uint16_t bus = 0; bus < 256; bus++)
    {
        for (uint8_t slot = 0; slot < 32; slot++)
        {
            for (uint8_t func = 0; func < 8; func++)
            {
                uint16_t vendor = pci_read_word(bus, slot, func, 0x00);

                if (vendor == 0xFFFF)
                    continue;

                uint16_t device = pci_read_word(bus, slot, func, 0x02);

                uint8_t class = pci_get_class(bus, slot, func);
                uint8_t subclass = pci_get_subclass(bus, slot, func);
                uint8_t prog_if = pci_get_prog_if(bus, slot, func);

                text_print("[{0x08}PCI{0x0F}]         bus=");
                text_print_dec(bus);

                text_print(" slot=");
                text_print_dec(slot);

                text_print(" func=");
                text_print_dec(func);

                text_print(" vendor=");
                text_print_hex(vendor);

                text_print(" device=");
                text_print_hex(device);

                text_print("\n");

                // RTL8139
                if (vendor == 0x10EC && device == 0x8139)
                {
                    printf("[{0x08}PCI{0x0F}]         RTL8139 FOUND at %d:%d:%d\n", bus, slot, func);
                }

                // AHCI SATA
                if (class == 0x01 && subclass == 0x06 && prog_if == 0x01)
                {
                    if (!ahci_found)
                    {
                        text_print("[{0x08}PCI{0x0F}]         AHCI SATA controller found\n");

                        ahci_dev.bus = bus;
                        ahci_dev.slot = slot;
                        ahci_dev.func = func;
                        ahci_found = 1;

                        ahci_init(bus, slot, func);
                    }
                }

                if (class == 0x03 && !gpu_found)
                {
                    gpu_dev.bus = bus;
                    gpu_dev.slot = slot;
                    gpu_dev.func = func;
                    gpu_dev.subclass = subclass;
                    gpu_found = 1;
                }
            }
        }
    }
    if (gpu_found)
    {
        text_print("[{0x08}PCI{0x0F}]         GPU detected: ");

        if (gpu_dev.subclass == 0x00)
            text_print("VGA compatible controller\n");
        else if (gpu_dev.subclass == 0x01)
            text_print("XGA controller\n");
        else if (gpu_dev.subclass == 0x02)
            text_print("3D controller\n");
        else
            text_print("Unknown display controller\n");
    }
}
