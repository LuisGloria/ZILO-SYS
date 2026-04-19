#include "rtl8139.h"
#include "../../arch/i686/pci.h"
#include "../../arch/i686/io.h"
#include "../../text.h"

static uint16_t io_base;

#define RTL_CMD        0x37
#define RTL_RBSTART    0x30
#define RTL_IMR        0x3C
#define RTL_ISR        0x3E
#define RTL_RCR        0x44
#define RTL_TCR        0x40

#define RTL_TX_STATUS0 0x10
#define RTL_TX_ADDR0   0x20

static uint8_t tx_buffer[1500] __attribute__((aligned(4)));
static uint8_t rx_buffer[8192 + 16 + 1500];
static uint8_t mac[6];

void rtl8139_get_mac(uint8_t out[6])
{
    for (int i = 0; i < 6; i++)
        out[i] = mac[i];
}

static void rtl8139_print_mac()
{
    uint8_t mac[6];

    for (int i = 0; i < 6; i++)
        mac[i] = i686_inb(io_base + i);

    text_print("MAC: ");

    for (int i = 0; i < 6; i++)
    {
        text_print_hex8(mac[i]);

        if (i != 5)
            text_putc(':');
    }

    text_print("\n");
}

void rtl8139_send(void* data, uint32_t len)
{
    if (len > 1500)
        return;

    // copy packet into TX buffer
    for (uint32_t i = 0; i < len; i++)
        tx_buffer[i] = ((uint8_t*)data)[i];

    // write buffer address n' stuff
    i686_outl(io_base + RTL_TX_ADDR0, (uint32_t)tx_buffer);
    i686_outl(io_base + RTL_TX_STATUS0, len);
    i686_outb(io_base + RTL_CMD, 0x0C);
}

int rtl8139_init(uint8_t bus, uint8_t slot)
{
    uint32_t bar0 = pci_get_bar0(bus, slot, 0);
    io_base = bar0 & ~0x3; // mask flags

    text_print("RTL8139 IO BASE: ");
    text_print_hex(io_base);
    text_print("\n");

    uint16_t command = pci_read_word(bus, slot, 0, 0x04);
    command |= 0x4;

    i686_outb(io_base + RTL_CMD, 0x10);
    while (i686_inb(io_base + RTL_CMD) & 0x10);

    text_print("RTL8139 reset OK\n");

    // Set RX buffer
    i686_outl(io_base + RTL_RBSTART, (uint32_t)rx_buffer);

    // Enable RX + TX
    i686_outb(io_base + RTL_CMD, 0x0C);

    // Accept all packets (promiscuous for now)
    i686_outl(io_base + RTL_RCR, 0xF | (1 << 7));

    text_print("RTL8139 initialized\n");
    rtl8139_print_mac();

    return 0;
}
//am I even doing this rigth?
