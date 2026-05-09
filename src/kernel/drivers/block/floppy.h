/* floppy.h */
#pragma once
#include <stdint.h>

#define FLOPPY_SECTOR_SIZE 512

void floppy_init();
int floppy_detect();

// later
/*int floppy_read(
    uint8_t cylinder,
    uint8_t head,
    uint8_t sector,
    uint8_t* buffer
);

int floppy_write(
    uint8_t cylinder,
    uint8_t head,
    uint8_t sector,
    uint8_t* buffer
);*/
