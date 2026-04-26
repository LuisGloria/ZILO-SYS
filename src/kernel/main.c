#include <stdint.h>

#include "text.h"
#include "cmos.h"
#include "stdio.h"
#include "panic.h"
#include "memory.h"
#include "hal/hal.h"
#include "memdetect.h"
#include "vfs/ramfs.h"
#include "fs/dataset.h"
#include "bin/datetime.h"
#include "arch/i686/pci.h"
#include "arch/i686/irq.h"
#include "drivers/video/fb.h"
#include "drivers/block/ata.h"
#include "drivers/audio/sb16.h"
#include "drivers/network/arp.h"
#include "drivers/audio/speakers.h"
#include "drivers/network/rtl8139.h"
#include "thingamabob/thingamabob.h"
extern uint8_t __bss_start;
extern uint8_t __end;

void crash_me();

void timer(Registers* regs)
{
    printf(".");
}

void __attribute__((section(".entry"))) start(uint16_t bootDrive, framebuffer_t* fb_info, BootParams* boot)
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));

    cmos_time_t rtc;
    cmos_read_rtc(&rtc);

    HAL_Initialize();
    memdetect_init(boot->Memory.Regions, boot->Memory.RegionCount);
    ramfs_init();

    clrscr();
    text_clear();
    pci_scan();
    memdetect_print();
    rtl8139_init(0, 3);
    sb16_init();
    text_print("[{0x08}SPEAKERS{0x0F}]    Testing sound...\n");
    speaker_beep(200, 300);
    arp_init();

    text_print("[{0x08}ARP{0x0F}]         Testing ARP...\n");
    arp_request(0xC0A80101); // 192.168.1.1

    thingamabob_init();
    thingamabob_status();
    thingamabob_do_stuff();

    if (!thingamabob_is_initialized())
    {
        text_print("{0x04}WHERE THE FUCK IS MY DOODAD?!!?!!?!, RETC=06");
        panic_now_silent("NOOOOOO, WHERE THE DOODAD?!?!?!!!!?!?!!!");
    }

    uint8_t sector[512];

    ata_init();
    text_print("[{0x08}ATA{0x0F}]         ATA Initialization...\n");
    ata_read28(0, 1, sector);
    text_print("[{0x08}ATA{0x0F}]         First byte: ");
    text_print_hex8(sector[0]);
    text_print("\n");
    dataset_init();
    text_print("[{0x08}CMOS{0x0F}]        Found time in CMOS clock battery.\n");
    text_print("[{0x08}CMOS{0x0F}]        RTC Time: ");
    text_print_dec(rtc.hour);
    text_print(":");
    text_print_dec(rtc.minute);
    text_print(" ");
    text_print_dec(rtc.day);
    text_print("-");
    text_print_dec(rtc.month);
    text_print("-");
    text_print_dec(rtc.year);
    text_print("\n");
    datetime_prompt();
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
    text_print("{0x0F}ZILO/SYS version 1.5b\n");
    text_print("{0x0F}READY.\n");
    text_print("> ");

    //i686_IRQ_RegisterHandler(0, timer);

    //crash_me(); is this even used anymore?

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
