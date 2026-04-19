#include <stdint.h>
#include "stdio.h"
#include "memory.h"
#include "hal/hal.h"
#include "arch/i686/irq.h"
#include "text.h"
#include "vfs/ramfs.h"
#include "arch/i686/pci.h"
#include "drivers/network/rtl8139.h"
#include "drivers/network/arp.h"
#include "drivers/video/fb.h"
#include "drivers/block/ata.h"

extern uint8_t __bss_start;
extern uint8_t __end;

void crash_me();

void timer(Registers* regs)
{
    printf(".");
}

void __attribute__((section(".entry"))) start(uint16_t bootDrive, framebuffer_t* fb_info)
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));

    HAL_Initialize();
    ramfs_init();

    //clrscr();
    text_clear();
    pci_scan();
    rtl8139_init(0, 3);
    arp_init();

    text_print("Testing ARP...\n");
    arp_request(0xC0A80101); // 192.168.1.1

    uint8_t sector[512];

    text_print("ATA Initialization...\n");
    ata_read28(0, 1, sector);
    text_print("First byte: ");
    text_print_hex8(sector[0]);
    text_print("\n");
    text_clear();

    text_print("{0x01}ZZZZZZZZZZZZZZ IIIIII LL            OOOOOO   \n");
    text_print("{0x01}ZZ          ZZ   II   LL          OO      OO \n");
    text_print("{0x01}          ZZ     II   LL         OO        OO\n");
    text_print("{0x01}        ZZ       II   LL         OO        OO\n");
    text_print("{0x01}      ZZ         II   LL         OO        OO\n");
    text_print("{0x09}  SSSSSSSSSSSS  YY{0x01}I   LL    {0x09}YY SSSSSSSSSSSS{0x01}OO\n");
    text_print("{0x09} SS        {0x01}ZZ{0x09}SS  {0x01}I{0x09}YY  {0x01}LL  {0x09}YY  SS  {0x01}OO      {0x09}SS \n");
    text_print("{0x01}Z{0x09}SS{0x01}ZZZZZZZZZZ  IIIII{0x09}YY{0x01}LL{0x09}YY{0x01}LLLL{0x09}SS    {0x01}OOOOOO   \n");
    text_print("{0x09} SS                  YYYY     SS             \n");
    text_print("{0x09}  SSSSSSSSSSSS        YY       SSSSSSSSSSSS  \n");
    text_print("{0x09}             SS       YY                  SS \n");
    text_print("{0x09}             SS       YY                  SS \n");
    text_print("{0x09} SS          SS       YY      SS          SS \n");
    text_print("{0x09}  SSSSSSSSSSSS        YY       SSSSSSSSSSSS  \n");
    text_print("{0x0F}\n");
    text_print("{0x0F}READY.\n");
    text_print("> ");
    //remember to uncomment "text_print("Unhandled IRQ %d  ISR=%x  IRR=%x...\n", irq, pic_isr, pic_irr);" from irq.c later smh

    //i686_IRQ_RegisterHandler(0, timer);

    //crash_me();

    //uint8_t packet[60] = {
    //    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // destination (broadcast)
    //    0x52,0x54,0x00,0x12,0x34,0x56,  // source (fake QEMU MAC)
    //    0x08,0x00,                      // type = IPv4
    //
    //    // payload (junk)
    //    0x48,0x45,0x4C,0x4C,0x4F
    //};


end:
    for (;;);
}
