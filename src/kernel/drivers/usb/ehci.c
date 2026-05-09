#include "ehci.h"
#include "../../arch/i686/pci.h"
#include "../../text.h"

#define EHCI_USBCMD      0x00
#define EHCI_USBSTS      0x04
#define EHCI_USBINTR     0x08
#define EHCI_CONFIGFLAG  0x40
#define EHCI_PORTSC      0x44

static volatile uint8_t* ehci_base = 0;
static volatile uint32_t* ehci_op = 0;
static uint8_t caplength = 0;
static uint32_t ports = 0;

static uint32_t mmio_read32(uint32_t offset)
{
    return *(volatile uint32_t*)(ehci_op + (offset / 4));
}

static void mmio_write32(uint32_t offset, uint32_t value)
{
    *(volatile uint32_t*)(ehci_op + (offset / 4)) = value;
}

void ehci_init(uint8_t bus, uint8_t slot, uint8_t func)
{
    text_print("[{0x08}EHCI{0x0F}]        Initializing EHCI...\n");

    uint32_t bar0 = pci_get_bar0(bus, slot, func);

    if (!bar0)
    {
        text_print("[{0x04}EHCI{0x0F}]        BAR0 missing\n");
        return;
    }

    //EHCI uses MMIO, so mask lower 4 bits
    uint32_t mmio = bar0 & 0xFFFFFFF0;

    text_print("[{0x08}EHCI{0x0F}]        MMIO Base: ");
    text_print_hex(mmio);
    text_print("\n");

    ehci_base = (volatile uint8_t*)mmio;

    //Capability Registers
    caplength = ehci_base[0];

    uint16_t hciversion =
    *(volatile uint16_t*)(ehci_base + 0x02);

    uint32_t hcsparams =
    *(volatile uint32_t*)(ehci_base + 0x04);

    ports = hcsparams & 0x0F;

    text_print("[{0x08}EHCI{0x0F}]        CAPLENGTH: ");
    text_print_dec(caplength);
    text_print("\n");

    text_print("[{0x08}EHCI{0x0F}]        HCIVERSION: ");
    text_print_hex(hciversion);
    text_print("\n");

    text_print("[{0x08}EHCI{0x0F}]        PORTS: ");
    text_print_dec(ports);
    text_print("\n");

    //Operation Registers start after CAPLENGTH
    ehci_op = (volatile uint32_t*)(mmio + caplength);

    //Stop controller first
    uint32_t cmd = mmio_read32(EHCI_USBCMD);
    cmd &= ~1;
    mmio_write32(EHCI_USBCMD, cmd);

    text_print("[{0x08}EHCI{0x0F}]        Controller stopped\n");

    //Reset controller
    cmd |= (1 << 1);
    mmio_write32(EHCI_USBCMD, cmd);

    while (mmio_read32(EHCI_USBCMD) & (1 << 1));

    text_print("[{0x08}EHCI{0x0F}]        Controller reset complete\n");

    //Route all ports to EHCI
    mmio_write32(EHCI_CONFIGFLAG, 1);

    text_print("[{0x08}EHCI{0x0F}]        Ports routed to EHCI\n");

    //Print port status
    for (uint32_t i = 0; i < ports; i++)
    {
        uint32_t port =
        mmio_read32(EHCI_PORTSC + (i * 4));

        text_print("[{0x08}EHCI{0x0F}]        Port ");
        text_print_dec(i + 1);
        text_print(": ");

        if (port & 1)
            text_print("Device Connected");
        else
            text_print("Empty");

        text_print("\n");
    }
}
