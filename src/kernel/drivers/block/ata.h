#pragma once
#include <stdint.h>

#define ATA_SECTOR_SIZE 512

void ata_init();
int ata_detect();

int ata_read28(uint32_t lba, uint8_t sector_count, void* buffer);
int ata_write28(uint32_t lba, uint8_t sector_count, uint8_t* buf);
