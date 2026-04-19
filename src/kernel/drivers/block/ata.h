#pragma once
#include <stdint.h>

#define ATA_SECTOR_SIZE 512

void ata_init();

// Read sectors from disk, dk if it works or not lol
int ata_read28(uint32_t lba, uint8_t sector_count, void* buffer);
