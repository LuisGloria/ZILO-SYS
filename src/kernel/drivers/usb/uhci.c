#include "uhci.h"
#include "../../text.h"
#include "../../arch/i686/pci.h"
#include "../../arch/i686/io.h"

#define UHCI_USBCMD      0x00
#define UHCI_USBSTS      0x02
#define UHCI_USBINTR     0x04
#define UHCI_FRNUM       0x06
#define UHCI_FRBASEADD   0x08
#define UHCI_SOFMOD      0x0C
#define UHCI_PORTSC1     0x10

#define UHCI_CMD_RS          (1 << 0)
#define UHCI_CMD_HCRESET     (1 << 1)
#define UHCI_CMD_GRESET      (1 << 2)
#define UHCI_CMD_CF          (1 << 6)
#define UHCI_CMD_MAXP        (1 << 7)

#define FRAME_LIST_COUNT 1024
#define FRAME_INVALID    0x00000001

static uint16_t uhci_io_base;


// Must be 4KB aligned.
// 1024 entries × 4 bytes = 4096 bytes exactly.

static uint32_t frame_list[FRAME_LIST_COUNT]
__attribute__((aligned(4096)));

static void uhci_wait()
{
    for (volatile int i = 0; i < 100000; i++);
}

static void uhci_reset()
{
    text_print("[{0x08}UHCI{0x0F}]        Resetting controller...\n");

    // Host Controller Reset
    i686_outw(uhci_io_base + UHCI_USBCMD, UHCI_CMD_HCRESET);

    while (i686_inw(uhci_io_base + UHCI_USBCMD) & UHCI_CMD_HCRESET);

    // Global Reset
    i686_outw(uhci_io_base + UHCI_USBCMD, UHCI_CMD_GRESET);
    uhci_wait();
    i686_outw(uhci_io_base + UHCI_USBCMD, 0x0000);

    text_print("[{0x08}UHCI{0x0F}]        Reset complete\n");
}

static void uhci_setup_frame_list()
{
    text_print("[{0x08}UHCI{0x0F}]        Setting up frame list...\n");

    for (int i = 0; i < FRAME_LIST_COUNT; i++)
        frame_list[i] = FRAME_INVALID;

    i686_outl(
        uhci_io_base + UHCI_FRBASEADD,
        (uint32_t)frame_list
    );

    i686_outw(
        uhci_io_base + UHCI_FRNUM,
        0
    );

    i686_outb(
        uhci_io_base + UHCI_SOFMOD,
        0x40
    );

    text_print("[{0x08}UHCI{0x0F}]        Frame list ready\n");
}

static void uhci_start()
{
    text_print("[{0x08}UHCI{0x0F}]        Starting controller...\n");

    // Enable interrupts (basic)
    i686_outw(
        uhci_io_base + UHCI_USBINTR,
        0x000F
    );

    // Run + 64-byte packets + Configure Flag
    i686_outw(
        uhci_io_base + UHCI_USBCMD,
        UHCI_CMD_RS |
        UHCI_CMD_MAXP |
        UHCI_CMD_CF
    );

    text_print("[{0x08}UHCI{0x0F}]        Controller running\n");
}

void uhci_init(uint8_t bus, uint8_t slot, uint8_t func)
{
    text_print("[{0x08}UHCI{0x0F}]        Initializing UHCI...\n");

     //   UHCI uses BAR4 for I/O ports
     //   offset = 0x20
    uint32_t bar4 = pci_read_dword(
        bus,
        slot,
        func,
        0x20
    );

    if (bar4 == 0 || bar4 == 0xFFFFFFFF)
    {
        text_print("[{0x04}UHCI{0x0F}]        Invalid BAR4\n");
        return;
    }

    //I/O BAR → mask lower bits
    uhci_io_base = bar4 & 0xFFFC;

    text_print("[{0x08}UHCI{0x0F}]        IO Base: ");
    text_print_hex(uhci_io_base);
    text_print("\n");

    uhci_reset();
    uhci_setup_frame_list();
    uhci_start();

    text_print("[{0x08}UHCI{0x0F}]        UHCI initialized successfully\n");
}
