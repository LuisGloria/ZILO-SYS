#pragma once
#include <stdint.h>

#define ZDSFS_MAGIC        0x4653445A   // "ZDSF"
#define ZDSFS_VERSION      1

#define ZDSFS_MAX_DATASETS 64
#define ZDSFS_NAME_LEN     44

#define ZDSFS_DEFAULT_SIZE  10
#define ZDSFS_SUPERBLOCK_LBA 1
#define ZDSFS_VTOC_LBA       2
#define ZDSFS_VTOC_SECTORS   8
#define ZDSFS_DATA_START_LBA (ZDSFS_VTOC_LBA + ZDSFS_VTOC_SECTORS)

typedef struct
{
    uint32_t magic;
    uint32_t version;

    uint32_t vtoc_start;
    uint32_t vtoc_size;

    uint32_t total_datasets;
} zdsfs_superblock_t;

typedef struct {
    const char* name;
} zdsfs_list_entry_t;

/* Format-1 DSCB entry */
typedef struct
{
    char name[ZDSFS_NAME_LEN];

    uint8_t format_id;      // always '1'

    uint32_t start_lba;
    uint32_t size_lba;

    uint32_t file_size;

    uint8_t flags;

} zdsfs_dscb_t;

/* core */
void zdsfs_init();
void zdsfs_format();


/* dataset operations */
int zdsfs_create(const char* name);
int zdsfs_delete(const char* name);

int zdsfs_write(
    const char* name,
    const uint8_t* data,
    uint32_t size_bytes
);

int zdsfs_read(
    const char* name,
    uint8_t* buffer
);

void zdsfs_list();

int zdsfs_count();

zdsfs_dscb_t* zdsfs_get(int index);
typedef zdsfs_dscb_t dataset_t; //old alias
