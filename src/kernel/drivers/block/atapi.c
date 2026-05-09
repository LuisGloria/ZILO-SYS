#include "atapi.h"
#include "../../arch/i686/io.h"
#include "../../text.h"

#define ATA_PRIMARY_IO    0x1F0
#define ATA_PRIMARY_CTRL  0x3F6

#define ATA_DRIVE   (ATA_PRIMARY_IO + 6)
#define ATA_STATUS  (ATA_PRIMARY_IO + 7)

#define ATA_LBA1    (ATA_PRIMARY_IO + 4)
#define ATA_LBA2    (ATA_PRIMARY_IO + 5)

#define ATA_DRIVE_MASTER 0xA0

int atapi_detect()
{
    /* Select master drive */
    i686_outb(ATA_DRIVE, ATA_DRIVE_MASTER);

    /* 400ns delay */
    i686_inb(ATA_STATUS);
    i686_inb(ATA_STATUS);
    i686_inb(ATA_STATUS);
    i686_inb(ATA_STATUS);

    uint8_t status = i686_inb(ATA_STATUS);

    if (status == 0x00 || status == 0xFF)
    {
        text_print("[{0x08}ATAPI{0x0F}]       No device\n");
        return 0;
    }

    uint8_t cl = i686_inb(ATA_LBA1);
    uint8_t ch = i686_inb(ATA_LBA2);

    if (cl == 0x14 && ch == 0xEB)
    {
        text_print("[{0x08}ATAPI{0x0F}]       CD-ROM detected\n");
        return 1;
    }

    text_print("[{0x08}ATAPI{0x0F}]       Not ATAPI\n");
    return 0;
}
