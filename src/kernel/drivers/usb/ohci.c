#include "ohci.h"
#include "../../text.h"
#include "../../arch/i686/pci.h"

#define OHCI_HC_REVISION          0x00
#define OHCI_HC_CONTROL           0x04
#define OHCI_HC_COMMAND_STATUS    0x08
#define OHCI_HC_INTERRUPT_STATUS  0x0C
#define OHCI_HC_HCCA              0x18
#define OHCI_HC_CONTROL_HEAD_ED   0x20
#define OHCI_HC_BULK_HEAD_ED      0x28
#define OHCI_HC_RH_DESCRIPTOR_A   0x48
#define OHCI_HC_RH_DESCRIPTOR_B   0x4C
#define OHCI_HC_RH_STATUS         0x50

static volatile uint32_t* ohci_regs = 0;

// MMIO read/write

static uint32_t ohci_read(uint32_t reg)
{
    return *(volatile uint32_t*)((uint8_t*)ohci_regs + reg);
}

static void ohci_write(uint32_t reg, uint32_t value)
{
    *(volatile uint32_t*)((uint8_t*)ohci_regs + reg) = value;
}

void ohci_init(uint8_t bus, uint8_t slot, uint8_t func)
{
    text_print("[{0x08}OHCI{0x0F}]        Initializing OHCI...\n");

    //OHCI uses BAR0 (MMIO)
    uint32_t bar0 = pci_get_bar0(bus, slot, func);

    if (!bar0)
    {
        text_print("[{0x04}OHCI{0x0F}]        No BAR0 found\n");
        return;
    }

    //Mask flags from BAR0
    uint32_t mmio_base = bar0 & 0xFFFFFFF0;

    text_print("[{0x08}OHCI{0x0F}]        MMIO Base: ");
    text_print_hex(mmio_base);
    text_print("\n");

    ohci_regs = (volatile uint32_t*)mmio_base;

    // Read revision register
    uint32_t revision = ohci_read(OHCI_HC_REVISION);

    text_print("[{0x08}OHCI{0x0F}]        Revision: ");
    text_print_hex(revision);
    text_print("\n");

    // Root hub info
    uint32_t rh_a = ohci_read(OHCI_HC_RH_DESCRIPTOR_A);

    uint32_t ports = rh_a & 0xFF;

    text_print("[{0x08}OHCI{0x0F}]        Root Hub Ports: ");
    text_print_dec(ports);
    text_print("\n");

    text_print("[{0x08}OHCI{0x0F}]        OHCI basic init complete\n");
}
