#include "ata.h"
#include "../../arch/i686/io.h"
#include "../../text.h"

#define ATA_PRIMARY_IO 0x1F0
#define ATA_PRIMARY_CTRL 0x3F6

#define ATA_DATA      (ATA_PRIMARY_IO + 0)
#define ATA_ERROR     (ATA_PRIMARY_IO + 1)
#define ATA_SECCOUNT  (ATA_PRIMARY_IO + 2)
#define ATA_LBA0      (ATA_PRIMARY_IO + 3)
#define ATA_LBA1      (ATA_PRIMARY_IO + 4)
#define ATA_LBA2      (ATA_PRIMARY_IO + 5)
#define ATA_DRIVE     (ATA_PRIMARY_IO + 6)
#define ATA_STATUS    (ATA_PRIMARY_IO + 7)
#define ATA_COMMAND   (ATA_PRIMARY_IO + 7)

#define ATA_CMD_READ  0x20
#define ATA_DRIVE_MASTER 0xE0

static int ata_wait_bsy()
{
    for (int i = 0; i < 1000000; i++)
        if (!(i686_inb(ATA_STATUS) & 0x80))
            return 0;

    return -1; // timeout
}

static int ata_wait_drq()
{
    for (int i = 0; i < 1000000; i++)
        if (i686_inb(ATA_STATUS) & 0x08)
            return 0;

    return -1;
}

void ata_init()
{
    // soft reset
    i686_outb(ATA_PRIMARY_CTRL, 0x04);
    for (volatile int i = 0; i < 100000; i++);
    i686_outb(ATA_PRIMARY_CTRL, 0x00);
}

int ata_read28(uint32_t lba, uint8_t sector_count, void* buffer)
{
    uint16_t* buf = (uint16_t*)buffer;

    ata_wait_bsy();

    if (ata_wait_bsy() < 0)
    {
        text_print("ATA BSY timeout\n");
        return -1;
    }

    i686_outb(ATA_DRIVE, ATA_DRIVE_MASTER | ((lba >> 24) & 0x0F));
    i686_outb(ATA_SECCOUNT, sector_count);

    i686_outb(ATA_LBA0, (uint8_t)(lba));
    i686_outb(ATA_LBA1, (uint8_t)(lba >> 8));
    i686_outb(ATA_LBA2, (uint8_t)(lba >> 16));

    i686_outb(ATA_COMMAND, ATA_CMD_READ);

    for (int s = 0; s < sector_count; s++)
    {
        ata_wait_bsy();
        ata_wait_drq();

        for (int i = 0; i < 256; i++)
        {
            buf[i] = i686_inw(ATA_DATA);
        }

        buf += 256;
    }

    return 0;
}
