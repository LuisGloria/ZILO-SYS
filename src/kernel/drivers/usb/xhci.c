#include "xhci.h"
#include "../../text.h"
#include "../../arch/i686/pci.h"

#define XHCI_CLASS     0x0C
#define XHCI_SUBCLASS  0x03
#define XHCI_PROGIF    0x30
#define XHCI_CAPLENGTH     0x00
#define XHCI_HCIVERSION    0x02
#define XHCI_HCSPARAMS1    0x04
#define XHCI_DBOFF         0x14
#define XHCI_RTSOFF        0x18
#define XHCI_USBCMD        0x00
#define XHCI_USBSTS        0x04
#define XHCI_PAGESIZE      0x08
#define XHCI_CRCR          0x18
#define XHCI_DCBAAP        0x30
#define XHCI_CONFIG        0x38
#define XHCI_PORTSC        0x400

static volatile uint8_t* xhci_base = 0;

static inline uint32_t mmio_read32(uint32_t offset)
{
    return *(volatile uint32_t*)(xhci_base + offset);
}

static inline void mmio_write32(uint32_t offset, uint32_t value)
{
    *(volatile uint32_t*)(xhci_base + offset) = value;
}

static inline uint8_t mmio_read8(uint32_t offset)
{
    return *(volatile uint8_t*)(xhci_base + offset);
}

void xhci_init(uint8_t bus, uint8_t slot, uint8_t func)
{
    text_print("[{0x08}xHCI{0x0F}]        Initializing xHCI controller...\n");

    // BAR0 + BAR1 form a 64-bit MMIO address
    uint32_t bar0 = pci_read_dword(bus, slot, func, 0x10);
    uint32_t bar1 = pci_read_dword(bus, slot, func, 0x14);

    uint64_t mmio_addr =
    ((uint64_t)bar1 << 32) |
    (bar0 & 0xFFFFFFF0);

    xhci_base = (volatile uint8_t*)(uint32_t)mmio_addr;

    text_print("[{0x08}xHCI{0x0F}]        MMIO Base: ");
    text_print_hex((uint32_t)mmio_addr);
    text_print("\n");

    //Read capability registers
    uint8_t caplength = mmio_read8(XHCI_CAPLENGTH);
    uint32_t hcsparams1 = mmio_read32(XHCI_HCSPARAMS1);
    uint32_t dboff = mmio_read32(XHCI_DBOFF);
    uint32_t rtsoff = mmio_read32(XHCI_RTSOFF);

    uint8_t max_slots = hcsparams1 & 0xFF;
    uint8_t max_ports = (hcsparams1 >> 24) & 0xFF;

    text_print("[{0x08}xHCI{0x0F}]        CAPLENGTH: ");
    text_print_hex8(caplength);
    text_print("\n");

    text_print("[{0x08}xHCI{0x0F}]        Max Slots: ");
    text_print_dec(max_slots);
    text_print("\n");

    text_print("[{0x08}xHCI{0x0F}]        Max Ports: ");
    text_print_dec(max_ports);
    text_print("\n");

    text_print("[{0x08}xHCI{0x0F}]        DBOFF: ");
    text_print_hex(dboff);
    text_print("\n");

    text_print("[{0x08}xHCI{0x0F}]        RTSOFF: ");
    text_print_hex(rtsoff);
    text_print("\n");

    //Operational registers start after CAPLENGTH
    uint32_t op_base = caplength;

    uint32_t usbsts = mmio_read32(op_base + XHCI_USBSTS);

    text_print("[{0x08}xHCI{0x0F}]        USBSTS: ");
    text_print_hex(usbsts);
    text_print("\n");

    //Port detection test
    for (uint32_t i = 0; i < max_ports; i++)
    {
        uint32_t port_offset =
        op_base + XHCI_PORTSC + (i * 0x10);

        uint32_t portsc = mmio_read32(port_offset);

        text_print("[{0x08}xHCI{0x0F}]        Port ");
        text_print_dec(i + 1);
        text_print(": ");

        if (portsc & 1)
            text_print("Device Connected\n");
        else
            text_print("Empty\n");
    }

    text_print("[{0x08}xHCI{0x0F}]        Basic probe complete\n");
}
